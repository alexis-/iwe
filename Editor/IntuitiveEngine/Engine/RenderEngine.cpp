#include "stdafx.h"

#include "RenderEngine.h"

template<> CRenderEngine* Ogre::Singleton<CRenderEngine>::msSingleton = NULL;

CRenderEngine*	CRenderEngine::getInstance()
{
	if (CRenderEngine::getSingletonPtr() == NULL)
		return NEW CRenderEngine;

	return CRenderEngine::getSingletonPtr();
}

CRenderEngine*
GetRenderEngine()
{
	return dynamic_cast<CRenderEngine*>(CRenderEngine::getInstance());
}



/////////////////////////////////////
// Constructor & Destructor

CRenderEngine::CRenderEngine(void)
  : m_pRoot(NULL),
    m_pCamera(NULL),
    m_pSceneMgr(NULL),
		m_pLogListener(NULL)
{
}

CRenderEngine::~CRenderEngine(void)
{

  Ogre::LogManager::getSingletonPtr()->getDefaultLog()->removeListener((Ogre::LogListener*)m_pLogListener);
  delete (Ogre::LogListener*)m_pLogListener;

  //for (int i = 0; i < m_pViewports.size(); i++)
  //  delete m_pViewports[i];

	if (m_pRoot)
		OGRE_DELETE m_pRoot;

  m_pRoot = NULL;
}

void
CRenderEngine::render()
{
	float delta = m_FrameTimer.getMicroseconds() * 0.000001;

  GetEditorInteraction()->tick();
  eventRender(delta);

	if (m_pRoot)
		m_pRoot->renderOneFrame();

	m_FrameTimer.reset();
}

void
CRenderEngine::logMessage(const char*	fmt,
													...)
{
	char			buffer[2048];
	va_list		ap;

	va_start(ap, fmt);
	vsnprintf(buffer, 2048, fmt, ap);
	va_end(ap);

	Ogre::LogManager::getSingletonPtr()->logMessage(buffer);
}

const std::vector<IViewport*>&
CRenderEngine::getViewports()
{
	return m_pViewports;
}

void
CRenderEngine::customCmd(int nArgs, ...)
{
}