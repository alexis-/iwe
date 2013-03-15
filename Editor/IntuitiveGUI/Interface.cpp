#include "stdafx.h"

#include <physfs.h>

#include "Interface.h"
#include "UIManager.h"
#include "MainFrm.h"


#undef min

typedef IEditorInterface* (*f_getEditorInterface)();
typedef IEngineInterface* (*f_getEngineInterface)();
typedef IPlugin* (*f_getPluginInterface)();

#ifdef WIN32

# include <Windows.h>

typedef HMODULE				lib_type;

# define LIBOPEN(Library)		LoadLibrary(Library)
# define LIBSYM(Handle, Symbol)		GetProcAddress(Handle, Symbol)
# define LIBCLOSE(Handle)		FreeLibrary(Handle)

#else // LINUX

# include <dlfcn.h>

typedef void*				lib_type;

# define LIBOPEN(Library)		dlopen(Library, RTLD_LAZY)
# define LIBSYM(Handle, Symbol)		dlsym(Handle, Symbol)
# define LIBCLOSE(Handle)		dlclose(Handle)

#endif // !WIN32


CGUIInterface* CGUIInterface::m_pInst = NULL;


CGUIInterface*
CGUIInterface::getInstance()
{
  if (!m_pInst)
    m_pInst = new CGUIInterface();

  return m_pInst;
}

CGUIInterface::~CGUIInterface()
{
  for (int i = 0; i < m_vecModules.size(); i++)
    LIBCLOSE(m_vecModules[i]);

  for (int i = 0; i < m_vecMemModules.size(); i++)
    MemoryFreeLibrary(m_vecMemModules[i]);
}

void
CGUIInterface::Init()
{
  lib_type editorLib = LIBOPEN(_T("IntuitiveEditor.dll"));

  if (!editorLib)
    exit(1);

  lib_type engineLib = LIBOPEN(_T("IntuitiveEngine.dll")); //ClientEngine.dll"));

  if (!engineLib)
  {
    LIBCLOSE(editorLib);
    exit(1);
  }

  f_getEditorInterface fGetEditorInterf = (f_getEditorInterface)LIBSYM(editorLib, "getEditorInterface");
  f_getEngineInterface fGetEngineInterf = (f_getEngineInterface)LIBSYM(engineLib, "getEngineInterface");

  if (!fGetEditorInterf || !fGetEngineInterf)
  {
    LIBCLOSE(editorLib);
    LIBCLOSE(engineLib);
    exit(1); // TODO : LOG THIS
  }

  m_pEditorInterface = fGetEditorInterf();
  m_pEngineInterface = fGetEngineInterf();

  m_pEngineInterface->initialize(m_pEditorInterface, this);
  m_pEditorInterface->initialize(m_pEngineInterface, this);

  m_vecModules.push_back(editorLib);
  m_vecModules.push_back(engineLib);

  PHYSFS_init(NULL);
}

CGUIInterface::CGUIInterface()
  : m_pEngineInterface(NULL),
    m_pEditorInterface(NULL)
{
}

IUIManager*
CGUIInterface::getUIManager()
{
  return CUIManager::getInstance();
}

void
CGUIInterface::showCursor(bool bShow)
{
  ShowCursor(bShow);
}

CIWEPoint<int>
CGUIInterface::getCursorPos()
{
  CIWEPoint<int> ret;
	POINT pt;

	GetCursorPos(&pt);

  ret.x = pt.x;
  ret.y = pt.y;

  return ret;
}

void
CGUIInterface::setCursorPos(int x, int y)
{
	SetCursorPos(x, y);
}
  

void
CGUIInterface::lockRender()
{
  CMainFrame::getInstance()->getCriticalSection()->Lock();
}


void
CGUIInterface::unlockRender()
{
  CMainFrame::getInstance()->getCriticalSection()->Unlock();
}

void
findPlugins(const std::string&                  sPluginFolder,
            std::vector<std::string>&           vecLibDatas)
{
  // Regular Libs
  std::vector<std::string> vecFoundFiles;

  int ret = SearchDirectory(vecFoundFiles, sPluginFolder, "dll", false);

  for each (std::string s in vecFoundFiles)
    vecLibDatas.push_back("././" + s);

  // Get zipped libs
  char** searchPathes = PHYSFS_getSearchPath();
  char** i = searchPathes;

  for (; *i != NULL; i++)
    PHYSFS_removeFromSearchPath(*i);

  PHYSFS_freeList(searchPathes);
  PHYSFS_addToSearchPath(sPluginFolder.c_str(), 0);

  char** files = PHYSFS_enumerateFiles(".");
  i = files;

  bool bUpdateRes = false;

  for (; *i != NULL; i++)
    if (hasCIEnding(*i, ".zip"))
    {
      PHYSFS_addToSearchPath((sPluginFolder + "/" + *i).c_str(), 1);
      
      GetResourceMgr()->addResourceLocation(sPluginFolder + "/" + *i, "Zip", "Plugins", true);

      bUpdateRes = true;
    }

  if (bUpdateRes)
    GetResourceMgr()->update();

  PHYSFS_freeList(files);
  PHYSFS_removeFromSearchPath(sPluginFolder.c_str());

  files = PHYSFS_enumerateFiles("");
  i = files;

  for (; *i != NULL; i++)
    if (hasCIEnding(*i, ".dll"))
      vecLibDatas.push_back(*i);

  PHYSFS_freeList(files);
}

void*
CGUIInterface::loadPluginModule(const std::string& sFileName, bool bMemory)
{
  if (!bMemory)
  {
    lib_type ret = LIBOPEN(sFileName.c_str());
    m_vecModules.push_back(ret);

    return ret;
  }
  else
  {
    PHYSFS_file* pFile = PHYSFS_openRead(sFileName.c_str());

    if (!pFile)
      return NULL;

    PHYSFS_sint64 size = PHYSFS_fileLength(pFile);

    if (size <= 0)
      return NULL;

    char* pDatas = new char[size];
    PHYSFS_sint64 length_read = PHYSFS_read(pFile, pDatas, size, 1);

    PHYSFS_close(pFile);
    pFile = NULL;

    if (length_read < 1)
    {
      delete [] pDatas;
      return NULL;
    }
  
    HMEMORYMODULE module = MemoryLoadLibrary(pDatas);

    if (!module)
    {
      delete [] pDatas;
      return NULL;
    }

    m_vecMemModules.push_back(module);

    return module;
  }
}

f_getPluginInterface
getPluginFunc(HMEMORYMODULE module)
{
  if (!module)
    return NULL;

  f_getPluginInterface fGetPluginInterf = (f_getPluginInterface)MemoryGetProcAddress(module, "getPluginInterface");

  return fGetPluginInterf;
}

f_getPluginInterface
getPluginFunc(lib_type module)
{
  if (!module)
    return NULL;

  f_getPluginInterface fGetPluginInterf = (f_getPluginInterface)LIBSYM(module, "getPluginInterface");

  return fGetPluginInterf;
}

void
CGUIInterface::findPlugins(const std::string&           sPluginFolder,
                           std::vector<std::string>&    vecFiles)
{
  std::vector<std::string> vecPluginsFiles;
  ::findPlugins(sPluginFolder, vecPluginsFiles);

  if (vecPluginsFiles.empty())
    return;

  TRACE("CGUIInterface::findPlugins\n");

  for (std::vector<std::string>::const_iterator it = vecPluginsFiles.begin();
       it != vecPluginsFiles.end();
       it++)
  {
    bool bMemory = (*it).substr(0, std::min((size_t)4, (*it).length())) != "././";
    TRACE(("Loading: " + *it + " | Memory: " + (bMemory ? "True" : "False") + "\n").c_str());

    void* module = loadPluginModule((*it), bMemory);

    if (module)
    {
      f_getPluginInterface fGetPluginInterf;
    
      if (bMemory)
        fGetPluginInterf = getPluginFunc(module);
      else
        fGetPluginInterf = getPluginFunc((lib_type)module);

      if (fGetPluginInterf)
        vecFiles.push_back(*it);
      
      if (bMemory)
        MemoryFreeLibrary(module);
      else
        LIBCLOSE((lib_type)module);
    }
  }

#if 0
  for (std::vector<std::string>::const_iterator it = vecFoundFiles.begin();
       it != vecFoundFiles.end();
       it++)
  {
    lib_type pluginLib = LIBOPEN((*it).c_str());

    if (!pluginLib)
      continue;

    f_getPluginInterface fGetPluginInterf = (f_getPluginInterface)LIBSYM(pluginLib, "getPluginInterface");

    if (fGetPluginInterf)
      vecFiles.push_back(*it);

    LIBCLOSE(pluginLib);
  }
#endif // !0
}

IPlugin*
CGUIInterface::loadPlugin(const std::string&            sPluginPath)
{
  bool bMemory = sPluginPath.substr(0, std::min((size_t)4, sPluginPath.length())) != "././";
  TRACE(("Loading Plugin: " + sPluginPath + " | Memory: " + (bMemory ? "True" : "False") + "\n").c_str());

  void* module = loadPluginModule(sPluginPath, bMemory);

  if (module)
  {
    f_getPluginInterface fGetPluginInterf;
    
    if (bMemory)
      fGetPluginInterf = getPluginFunc(module);

    else
      fGetPluginInterf = getPluginFunc((lib_type)module);

    if (fGetPluginInterf)
      return fGetPluginInterf();
    
    if (bMemory)
      MemoryFreeLibrary(module);
    else
      LIBCLOSE((lib_type)module);
  }

  return NULL;

#if 0
  lib_type pluginLib = LIBOPEN(sPluginPath.c_str());

  if (!pluginLib)
    return NULL;

  f_getPluginInterface fGetPluginInterf = (f_getPluginInterface)LIBSYM(pluginLib, "getPluginInterface");

  if (fGetPluginInterf)
    return fGetPluginInterf();

  LIBCLOSE(pluginLib);
  return NULL;
#endif // !0
}