#include "stdafx.h"


#include "ProjectManager.h"
#include "EditorDatas.h"
#include "SaveSystemManager.h"

CProjectManager*							CProjectManager::m_pInst = NULL;

CProjectManager*
CProjectManager::getInstance()
{
	if (!m_pInst)
		m_pInst = NEW CProjectManager();

	return m_pInst;
}

CProjectManager::CProjectManager()
	: m_sProjectName(""),
		m_sBaseDir(""),
		m_sProjectFile(""),
		m_sExt(""),
    m_bProjectLoaded(false)
{
}

CProjectManager::~CProjectManager()
{
	//GetEntityMgr()->clear();
	//CTerrainManager::getInstance()->clear(); // TODO: Notify module

	//CMainFrame::getInstance()->clearTerrainGroups(); // TODO: Notify UI ?

	CEditorDatas::m_vecPickedUnits.clear();

	m_bProjectLoaded = false;
}

void
CProjectManager::initProject()
{
	//CMainFrame::getInstance()->getFileView()->FillFileView(m_sProjectName); // TODO: Notify UI?
	GetResourceMgr()->update(); // TODO: Use resourceMgr ptr

	m_bProjectLoaded = true;

  eventProjectLoaded(getProjectName());
}

void
CProjectManager::newProject(const std::string& projectName, const std::string& baseDir)
{
	if (m_bProjectLoaded)
	{
		// TODO: Save if needed
		closeProject();
	}

	m_sProjectName = projectName;
	m_sBaseDir = baseDir;

	if (!hasEnding(m_sBaseDir, "\\"))
		m_sBaseDir += "\\";

	initProject();
}

bool
CProjectManager::loadProject(const std::string& projectFilePath, const std::string& ext)
{
	if (m_bProjectLoaded == true)
	{
		// If there's a project loaded, ask if the user want to save, continue, or cancel
		closeProject();
	}

	CSaveSystem* pSystem = CSaveSystemManager::getInstance()->getFromExt(ext);

	if (!pSystem)
	{
		TRACE(("CProjectManager::loadProject() could not find load system for extension \"" + ext + "\"\n").c_str());
		return false;
	}

	if (!pSystem->OnLoad(projectFilePath))
		return false;

	m_sProjectFile = m_sProjectFilePath = projectFilePath;
	m_sExt = ext;

	initProject();

	return true;
}

void
CProjectManager::saveProject()
{
	if (m_bProjectLoaded == false)
		return;

	CSaveSystem* pSystem = CSaveSystemManager::getInstance()->getFromExt(m_sExt);

	if (!pSystem)
	{
		TRACE(("CProjectManager::saveProject() could not find save system for extension \"" + m_sExt + "\"\n").c_str());
		return;
	}

	pSystem->OnSave(m_sProjectFilePath);

  eventProjectSaved();
}

void
CProjectManager::saveProject(const std::string& projectFile, const std::string& projectFilePath, const std::string& ext)
{
	m_sProjectFile = projectFile;
	m_sExt = ext;
	m_sProjectFilePath = projectFilePath;

	saveProject();
}

void
CProjectManager::closeProject()
{
	//GetEntityMgr()->clear();
	//CTerrainManager::getInstance()->clear(); // TODO: Notify module

	//CMainFrame::getInstance()->clearTerrainGroups(); // TODO: Notify UI?
	//CMainFrame::getInstance()->getFileView()->ClearFileView();

	CEditorDatas::m_vecPickedUnits.clear();
	m_bProjectLoaded = false;
	
	m_sProjectName = "";
	m_sBaseDir = "";

  eventProjectClosed();
}