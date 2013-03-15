/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2012 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
same license as the rest of the engine.
-----------------------------------------------------------------------------
*/

/**
Implementation of a Deferred Shading engine in OGRE, using Multiple Render Targets and
CG high level language shaders.
	// W.J. :wumpus: van der Laan 2005 / Noam Gat 2009 //

Deferred shading renders the scene to a 'fat' texture format, using a shader that outputs colour, 
normal, depth, and possible other attributes per fragment. Multi Render Target is required as we 
are dealing with many outputs which get written into multiple render textures in the same pass.

After rendering the scene in this format, the shading (lighting) can be done as a post process. 
This means that lighting is done in screen space, using light-representing geometry (sphere for
point light, cone for spot light and quad for directional) to render their contribution.

The wiki article explaining this demo can be found here :
  http://www.ogre3d.org/wiki/index.php/Deferred_Shading
*/

#ifndef RENDER_SETTINGS_H
#define RENDER_SETTINGS_H

#include "Ogre.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#include <OgreCompositorInstance.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMaterial.h>
#include <OgreRenderTargetListener.h>
#include <OgreCompositorLogic.h>


/** System to manage Deferred Shading for a camera/render target.
 *  @note With the changes to the compositor framework, this class just
 *		selects which compositors to enable.
 */
class RenderSettings
{
public:
	~RenderSettings();
  static RenderSettings* getInstance();

	//The first render queue that does get rendered into the GBuffer
	//place objects (like skies) that should be before gbuffer before this one.
	static const Ogre::uint8 PRE_GBUFFER_RENDER_QUEUE;
	
	//The first render queue that does not get rendered into the GBuffer
	//place transparent (or other non gbuffer) objects after this one
	static const Ogre::uint8 POST_GBUFFER_RENDER_QUEUE;

	void initialize(Ogre::Viewport* pViewport, Ogre::SceneManager* pSceneMgr, Ogre::Camera* pCamera);
  void setCompositorState(const std::string& sComp, bool bState);

protected:
	Ogre::Viewport*               m_pViewport;
	Ogre::SceneManager*           m_pSceneMgr;
	Ogre::Camera*                 m_pCamera;
	
	Ogre::CompositorInstance*     m_pGBufferInstance;
	// Filters
	Ogre::CompositorInstance*     m_pShowLitInstance;
	Ogre::CompositorInstance*     m_pSSAOInstance;
	Ogre::CompositorInstance*     m_pSSAAInstance;
	Ogre::CompositorInstance*     m_pBloomInstance;

	// Active/inactive
	bool                          m_bDefferedActive;
	bool                          m_bSSAO;

	//Used to unregister compositor logics and free memory
	typedef std::map<std::string, Ogre::CompositorLogic*> CompositorLogicMap;
	CompositorLogicMap            m_mapCompositorLogics;

	void createResources();

private:
  static RenderSettings*        m_pInst;
	RenderSettings();
};

#endif
