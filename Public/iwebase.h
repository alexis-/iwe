#ifndef IWEBASE_H
# define IWEBASE_H

//////////////////////////////////////////////////////////////////////////
// Helper stuffs

typedef unsigned char       uchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;

#define interface_class     class
#define abstract_class			class
#define static_class				class

// FastDelegates & Events
# include "Helpers/FastDelegate.h"
# include "Helpers/Event.h"
  
# include "Helpers/TagUserData.h"


using namespace fastdelegate;
using namespace DelegateEvent;



//////////////////////////////////////////////////////////////////////////
// Core stuffs

# if !defined (_IWE_GUI ) && !defined (_DISABLE_OGRE )
  
#  include <OgreVector2.h>
#  include <OgreVector3.h>
#  include <OgreQuaternion.h>
#  include <OgreCommon.h>

# else

namespace Ogre
{
  class Vector3;
  class Vector2;
  class Quaternion;
}

# endif // _IWE_GUI



//////////////////////////////////////////////////////////////////////////
// Interfaces

# ifdef _WIN32
#  ifdef IWELIBRARY_EXPORT
#     define IWEAPI __declspec(dllexport)
#  else
#     define IWEAPI __declspec(dllimport)
#  endif // !IWELIBRARY_EXPORT
# else
#  define IWEAPI
#endif



///////////////////////////////////
// Common
# include "IWECommon.h"
# include "CoreMessages.h"
# include "IWEKeys.h"


///////////////////////////////////
// Plugin & Modable stuffs
# include "IPlugin.h"

// Define camera controls.. This allow user-defined controls
# include "ICameraController.h"

# include "ISelectableTarget.h"
# include "IPropertyHolder.h"

# include "IMementoObject.h"


///////////////////////////////////
// GUI Interface
# include "IUIMainMenu.h"
# include "IUIProjectTree.h"
# include "IUIPropertyGrid.h"
# include "IUISceneTree.h"

# include "IUIManager.h"

# include "IGUI.h"


///////////////////////////////////
// Main Engine Interface
# include "ICamera.h"
# include "IViewport.h"

# include "IResourceManager.h"

# include "IRenderEngine.h"

# include "IEngineInterface.h"


///////////////////////////////////
// Editor Interface
# include "ITool.h"

# include "IViewportGrid.h"

# include "IEditorInteraction.h"
# include "IProjectManager.h"
# include "IToolManager.h"

# include "IEditorInterface.h"


///////////////////////////////////
// Public helper

//# if !defined (_IWE_GUI ) && !defined (_DISABLE_OGRE )
//#  include "RenderHelper/EngineUtils.h"
//# endif


#endif // !IWEBASE_H