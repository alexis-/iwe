// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once


#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#include <afxsock.h>            // MFC socket extensions


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#undef max
#undef min

#include <stdexcept>
#include <string>
#include <list>

# include <limits>

#include "iwebase.h"
#include "Interface.h"

#include "CMFCRibbonFixedButton.h"

namespace Ogre
{
	class SceneNode;
	class Vector3;
	class Ray;
	class Plane;
	class TerrainGroup;
	class Terrain;
	class ManualObject;
};


#ifdef _DEBUG

  // Memory leak stuffs ...
  #define _CRTDBG_MAP_ALLOC
  #include <stdlib.h>
  #include <crtdbg.h>

  #define NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
  #define NEW new
#endif  // _DEBUG

#define WM_OUTPUT_ADD										WM_APP + 10

#define abstract_class			class
#define static_class				class

typedef unsigned int				uint;

const char									cRenderQueueViewGrid = 0x60;
const int										iMaxFPS = 30;
const int										iMaxFPSDelay = 1000 / iMaxFPS;

const char*									getUniqueName(const std::string&						name);
std::wstring								s2ws(const std::string&											s);
bool												listContains(const std::list<std::string>&	list,
																				std::string											name);
bool												hasEnding(std::string const&								fullString,
																			std::string const&								ending);
bool												hasCIEnding(std::string const&							fullString,
																				std::string const&							ending);

CImage*                     getOgreBitmap(const std::string& sName, const std::string& sGroupName);

HICON                       HICONFromCBitmap(CBitmap& bitmap);
HICON                       HICONFromData(const s_IWEImageData& imgData);
HICON                       HICONFromData(const s_IWEImageData& imgData, int newWidth, int newHeight);
void                        CBitmapFromData(CBitmap& bmpOut, const s_IWEImageData& imgData);
void                        CBitmapFromData(CBitmap& bmpOut, const s_IWEImageData& imgData, int newWidth, int newHeight);

int                         SearchDirectory(std::vector<std::string> &refvecFiles,
                                            const std::string        &refcstrRootDirectory,
                                            const std::string        &refcstrExtension,
                                            bool                     bSearchSubdirectories = true);

inline DROPEFFECT getDropEffect(e_dropEffect e)
{
  return (DROPEFFECT)(e == UI_DROPEFFECT_LINK) ? DROPEFFECT_LINK : e;
}


#include "ViewTree.h"
#include <afxdlgs.h>

BOOL												checkPathExists(LPCTSTR szPath);
BOOL												isDirectory(LPCTSTR szPath);

template<typename T>
bool												vecItemExists(const std::vector<T>& vector, const T& item)
{
	for (std::vector<T>::const_iterator it = vector.begin();
			 it != vector.end();
			 it++)
		if (*it == item)
			return true;

	return false;
}

template<typename T>
bool												vecItemExists(const std::vector<T>& vector, std::function<bool(T& e)> f)
{
	for (std::vector<T>::const_iterator it = vector.begin();
			 it != vector.end();
			 it++)
		if (f(*it))
			return true;

	return false;
}

template<typename T>
typename std::vector<T>::iterator		vecGetItemIterator(std::vector<T>& vector, const T& item)
{
	for (std::vector<T>::iterator it = vector.begin();
			 it != vector.end();
			 it++)
		if (*it == item)
			return it;

	return vector.end();
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#define SYNCHRONIZED(slock, cs) if (CSynchronizer slock = CSynchronizer(cs))

class CSynchronizer : public CSingleLock
{
public:
    CSynchronizer(CSyncObject *syncobj) : CSingleLock(syncobj, TRUE) {}
    operator BOOL() { return CSingleLock::IsLocked(); }
    BOOL Wait(DWORD dwTimeOut = INFINITE);
    BOOL Notify();
    virtual ~CSynchronizer() {}

};

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

const DWORD MS_VC_EXCEPTION=0x406D1388;

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
   DWORD dwType; // Must be 0x1000.
   LPCSTR szName; // Pointer to name (in user addr space).
   DWORD dwThreadID; // Thread ID (-1=caller thread).
   DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)

void SetThreadName(DWORD dwThreadID, const char* threadName);
