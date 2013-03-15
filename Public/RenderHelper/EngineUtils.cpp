#include "EngineUtils.h"

#include "Helpers/threadpool.hpp"

#include <OgreEntity.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgreSceneManager.h>
#include <OgreMovableObject.h>
#include <OgreSubEntity.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>

#define THREAD_NUM        4


namespace RenderHelper
{
  namespace tp = boost::threadpool;

  static tp::pool gThreadPool;

  void initializePool()
  {
    static bool bInit = false;

    if (!bInit)
    {
      gThreadPool.size_controller().resize(THREAD_NUM);

      bInit = true;
    }
  }

  struct s_getVertex
  {
    const Ogre::VertexElement*            posElem;
    uchar*                                vertex;
    Ogre::HardwareVertexBufferSharedPtr   vbuf;
    Ogre::Vector3*                        vertices;
  };

  void getVertex(s_getVertex& getVertexDatas,
                 size_t begin,
                 size_t end,
                 size_t current_offset,
                 const Ogre::Vector3 &position,
                 const Ogre::Quaternion &orient,
                 const Ogre::Vector3 &scale)
  {
		float* pReal;
    
    const Ogre::VertexElement*            posElem = getVertexDatas.posElem;
    uchar*                                vertex = getVertexDatas.vertex;
    Ogre::HardwareVertexBufferSharedPtr   vbuf = getVertexDatas.vbuf;
    Ogre::Vector3*                        vertices = getVertexDatas.vertices;

    vertex += begin * vbuf->getVertexSize();
    
    for(size_t j = begin; j < end; ++j, vertex += vbuf->getVertexSize())
    {
		  posElem->baseVertexPointerToElement(vertex, &pReal);

		  Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

		  vertices[current_offset + j] = (orient * (pt * scale)) + position;
    }
  }

  void
  GetMeshInformation(Ogre::Entity *entity,
                     size_t &vertex_count,
                     Ogre::Vector3* &vertices,
                     size_t &index_count,
                     unsigned long* &indices,
                     const Ogre::Vector3 &position,
                     const Ogre::Quaternion &orient,
                     const Ogre::Vector3 &scale)
  {
	  bool added_shared = false;
	  size_t current_offset = 0;
	  size_t shared_offset = 0;
	  size_t next_offset = 0;
	  size_t index_offset = 0;
	  vertex_count = index_count = 0;

	  Ogre::MeshPtr mesh = entity->getMesh();


	  bool useSoftwareBlendingVertices = entity->hasSkeleton();

	  if (useSoftwareBlendingVertices)
	  {
		  entity->_updateAnimation();
	  }

	  // Calculate how many vertices and indices we're going to need
	  for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	  {
		  Ogre::SubMesh* submesh = mesh->getSubMesh(i );

		  // We only need to add the shared vertices once
		  if (submesh->useSharedVertices)
		  {
			  if(!added_shared )
			  {
				  vertex_count += mesh->sharedVertexData->vertexCount;
				  added_shared = true;
			  }
		  }
		  else
		  {
			  vertex_count += submesh->vertexData->vertexCount;
		  }

		  // Add the indices
		  index_count += submesh->indexData->indexCount;
	  }


	  // Allocate space for the vertices and indices
	  vertices = new Ogre::Vector3[vertex_count];
	  indices = new unsigned long[index_count];

	  added_shared = false;

	  // Run through the submeshes again, adding the data into the arrays
	  for (unsigned short i = 0; i < mesh->getNumSubMeshes(); ++i)
	  {
		  Ogre::SubMesh* submesh = mesh->getSubMesh(i);

		  //----------------------------------------------------------------
		  // GET VERTEXDATA
		  //----------------------------------------------------------------

		  //Ogre::VertexData* vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;
		  Ogre::VertexData* vertex_data;

		  //When there is animation:
		  if(useSoftwareBlendingVertices)
  #ifdef BUILD_AGAINST_AZATHOTH
			  vertex_data = submesh->useSharedVertices ? entity->_getSharedBlendedVertexData() : entity->getSubEntity(i)->_getBlendedVertexData();
  #else
			  vertex_data = submesh->useSharedVertices ? entity->_getSkelAnimVertexData() : entity->getSubEntity(i)->_getSkelAnimVertexData();
  #endif
		  else
			  vertex_data = submesh->useSharedVertices ? mesh->sharedVertexData : submesh->vertexData;


		  if((!submesh->useSharedVertices)||(submesh->useSharedVertices && !added_shared))
		  {
			  if(submesh->useSharedVertices)
			  {
				  added_shared = true;
				  shared_offset = current_offset;
			  }

			  const Ogre::VertexElement* posElem =
				  vertex_data->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);

			  Ogre::HardwareVertexBufferSharedPtr vbuf =
				  vertex_data->vertexBufferBinding->getBuffer(posElem->getSource());

			  unsigned char* vertex =
				  static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));

			  // There is _no_ baseVertexPointerToElement() which takes an Ogre::Real or a double
			  //  as second argument. So make it float, to avoid trouble when Ogre::Real will
			  //  be comiled/typedefed as double:
			  //      Ogre::Real* pReal;

        int perThread = vertex_data->vertexCount / THREAD_NUM;

        s_getVertex getVertexDatas = { posElem, vertex, vbuf, vertices };

        for (size_t j = 0; j < THREAD_NUM - 1; j++)
          tp::schedule(gThreadPool, boost::bind(getVertex, getVertexDatas,
            perThread * j, perThread * (j + 1), current_offset, position, orient, scale));

        tp::schedule(gThreadPool, boost::bind(getVertex, getVertexDatas,
          perThread * (THREAD_NUM - 1), vertex_data->vertexCount, current_offset, position, orient, scale));

//void getVertex(const Ogre::VertexElement* posElem,
//               void* vertex,
//               Ogre::HardwareVertexBufferSharedPtr vbuf,
//               Ogre::Vector3* vertices,
//               size_t begin,
//               size_t end,
//               size_t current_offset,

//        for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
//          tp::schedule(gThreadPool, boost::bind(getVertex, posElem, vertex,
//            vertices[current_offset + j], position, orient, scale));

        gThreadPool.wait();

#if 0
			  for(size_t j = 0; j < vertex_data->vertexCount; ++j, vertex += vbuf->getVertexSize())
			  {
				  posElem->baseVertexPointerToElement(vertex, &pReal);

				  Ogre::Vector3 pt(pReal[0], pReal[1], pReal[2]);

				  vertices[current_offset + j] = (orient * (pt * scale)) + position;
			  }
#endif // !0

			  vbuf->unlock();
			  next_offset += vertex_data->vertexCount;
		  }


		  Ogre::IndexData* index_data = submesh->indexData;
		  size_t numTris = index_data->indexCount / 3;
		  Ogre::HardwareIndexBufferSharedPtr ibuf = index_data->indexBuffer;

		  bool use32bitindexes = (ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT);

		  unsigned long*  pLong = static_cast<unsigned long*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
		  unsigned short* pShort = reinterpret_cast<unsigned short*>(pLong);


		  size_t offset = (submesh->useSharedVertices)? shared_offset : current_offset;
		  size_t index_start = index_data->indexStart;
		  size_t last_index = numTris*3 + index_start;

		  if (use32bitindexes)
			  for (size_t k = index_start; k < last_index; ++k)
			  {
				  indices[index_offset++] = pLong[k] + static_cast<unsigned long>(offset );
			  }

		  else
			  for (size_t k = index_start; k < last_index; ++k)
			  {
				  indices[ index_offset++ ] = static_cast<unsigned long>(pShort[k] ) +
					  static_cast<unsigned long>(offset );
			  }

			  ibuf->unlock();
			  current_offset = next_offset;
	  }
  }

  void testIntersection(Ogre::Ray& ray, Ogre::Vector3* vertices, unsigned long* indices, uint i, uint end, float* fOut)
  {
		// check for a hit against this triangle
    for (; i < end; i += 3)
    {
		  std::pair<bool, Ogre::Real> hit = Ogre::Math::intersects(ray, vertices[indices[i]],
		  	vertices[indices[i + 1]], vertices[indices[i + 2]], true, false);

		  // if it was a hit check if its the closest
      if (hit.first && hit.second < *fOut)
        *fOut = hit.second;
    }
  }
  
  ISelectableTarget* PickSelectableTarget(float x, float y, Ogre::SceneManager* pSceneMgr, IViewport* pViewport, float& fDist, Ogre::Vector3& vecPickedPos)
  {
	  Ogre::Ray								ray = pViewport->getViewportRay(x, y);
	  Ogre::RaySceneQuery*		rayQuery = pSceneMgr->createRayQuery(ray, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	  if (rayQuery == NULL)
		  return NULL;

	  rayQuery->setSortByDistance(true);
	  rayQuery->setQueryMask(1);
  
	  if (rayQuery->execute().size() <= 0)
	    return NULL;

    initializePool();

	  Ogre::RaySceneQueryResult &rayQueryResult = rayQuery->getLastResults();
	  uint rayQuerySize = rayQueryResult.size();

    ISelectableTarget* ret = NULL;
    fDist = FLT_MAX;

	  for (uint i = 0; i < rayQuerySize; i++)
	  {
		  if ((rayQueryResult[i].movable != NULL) &&
			  (rayQueryResult[i].movable->getMovableType().compare("Entity") == 0))
		  {
			  // Get the entity to check
			  Ogre::Entity*				pEnt = dynamic_cast<Ogre::Entity*>(rayQueryResult[i].movable);    

			  if (!pEnt)
				  continue;

        ISelectableTarget* pTarget = NULL;

        try
        {
          pTarget = Ogre::any_cast<ISelectableTarget*>(pEnt->getUserObjectBindings().getUserAny());
        }
        catch (...)
        {
          continue;
        }

			  // Mesh data to retrieve
			  size_t							vertex_count = 0;
			  size_t							index_count = 0;
			  Ogre::Vector3*			vertices = NULL;
			  unsigned long*			indices = NULL;

			  // get the mesh information
			  GetMeshInformation(pEnt, vertex_count, vertices, index_count, indices,             
				  pEnt->getParentNode()->getPosition(),
				  pEnt->getParentNode()->getOrientation(),
				  pEnt->getParentNode()->_getDerivedScale());

        float*                fResults = new float[THREAD_NUM];
        float                 fCurMin = FLT_MAX;

        for (size_t j = 0; j < THREAD_NUM; j++)
          fResults[j] = FLT_MAX;

			  // test for hitting individual triangles on the mesh
        
        int perThread = std::floor((float)index_count / (float)THREAD_NUM);

        for (size_t j = 0; j < THREAD_NUM - 1; j++)
          tp::schedule(gThreadPool, boost::bind(&testIntersection, ray, vertices, indices, j * perThread, (j + 1) * perThread, &fResults[j]));
        
        tp::schedule(gThreadPool, boost::bind(&testIntersection, ray, vertices, indices, (THREAD_NUM - 1) * perThread, index_count, &fResults[THREAD_NUM - 1]));

			  //for (uint j = 0; j < index_count; j += 3)
        //  testIntersection(ray, vertices, indices, j, fResults[--size]);

        gThreadPool.wait();

        for (size_t j = 0; j < THREAD_NUM; j++)
          fCurMin = std::min(fCurMin, fResults[j]);

        if (fCurMin < fDist)
        {
          fDist = fCurMin;
          vecPickedPos = ray.getPoint(fDist);
          ret = pTarget;
        }

			  // free the verticies and indicies memory
        delete[] fResults;
			  delete[] vertices;
			  delete[] indices;
		  }
	  }

	  return ret;
  }

  
  ISelectableTarget* PickSelectableTarget(float x, float y, Ogre::SceneManager* pSceneMgr, IViewport* pViewport, float& fDist, Ogre::Vector3& vecPickedPos, const std::string& sType)
  {
	  Ogre::Ray								ray = pViewport->getViewportRay(x, y);
	  Ogre::RaySceneQuery*		rayQuery = pSceneMgr->createRayQuery(ray, Ogre::SceneManager::WORLD_GEOMETRY_TYPE_MASK);

	  if (rayQuery == NULL)
		  return NULL;

	  rayQuery->setSortByDistance(true);
	  rayQuery->setQueryMask(1);
  
	  if (rayQuery->execute().size() <= 0)
	    return NULL;

    initializePool();

	  Ogre::RaySceneQueryResult &rayQueryResult = rayQuery->getLastResults();
	  uint rayQuerySize = rayQueryResult.size();

    ISelectableTarget* ret = NULL;
    fDist = FLT_MAX;

	  for (uint i = 0; i < rayQuerySize; i++)
	  {
		  if ((rayQueryResult[i].movable != NULL) &&
			  (rayQueryResult[i].movable->getMovableType().compare("Entity") == 0))
		  {
			  // Get the entity to check
			  Ogre::Entity*				pEnt = dynamic_cast<Ogre::Entity*>(rayQueryResult[i].movable);

			  if (!pEnt)
				  continue;

        ISelectableTarget* pTarget = NULL;

        try
        {
          pTarget = Ogre::any_cast<ISelectableTarget*>(pEnt->getUserObjectBindings().getUserAny());

          if (pTarget->getType() != sType)
            continue;
        }
        catch (...)
        {
          continue;
        }

			  // Mesh data to retrieve         
			  size_t							vertex_count = 0;
			  size_t							index_count = 0;
			  Ogre::Vector3*			vertices = NULL;
			  unsigned long*			indices = NULL;

			  // get the mesh information
			  GetMeshInformation(pEnt, vertex_count, vertices, index_count, indices,             
				  pEnt->getParentNode()->getPosition(),
				  pEnt->getParentNode()->getOrientation(),
				  pEnt->getParentNode()->_getDerivedScale());

        float*                fResults = new float[THREAD_NUM];
        float                 fCurMin = FLT_MAX;

        for (size_t j = 0; j < THREAD_NUM; j++)
          fResults[j] = FLT_MAX;

			  // test for hitting individual triangles on the mesh
        
        int perThread = std::floor((float)index_count / (float)THREAD_NUM);

        for (size_t j = 0; j < THREAD_NUM - 1; j++)
          tp::schedule(gThreadPool, boost::bind(&testIntersection, ray, vertices, indices, j * perThread, (j + 1) * perThread, &fResults[j]));
        
        tp::schedule(gThreadPool, boost::bind(&testIntersection, ray, vertices, indices, (THREAD_NUM - 1) * perThread, index_count, &fResults[THREAD_NUM - 1]));

			  //for (uint j = 0; j < index_count; j += 3)
        //  testIntersection(ray, vertices, indices, j, fResults[--size]);

        gThreadPool.wait();

        for (size_t j = 0; j < THREAD_NUM; j++)
          fCurMin = std::min(fCurMin, fResults[j]);

        if (fCurMin < fDist)
        {
          fDist = fCurMin;
          vecPickedPos = ray.getPoint(fDist);
          ret = pTarget;
        }

			  // free the verticies and indicies memory
        delete[] fResults;
			  delete[] vertices;
			  delete[] indices;
		  }
	  }

	  return ret;
  }



  void
  DestroyAllAttachedMovableObjects(Ogre::SceneNode* pSceneNode)
  {
    if (!pSceneNode )
    {
       //ASSERT(false ); // TODO: Replace me
       return;
    }

    // Destroy all the attached objects
    Ogre::SceneNode::ObjectIterator itObject = pSceneNode->getAttachedObjectIterator();

    while (itObject.hasMoreElements())
    {
       Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
       pSceneNode->getCreator()->destroyMovableObject(pObject);
    }

    // Recurse to child SceneNodes
    Ogre::SceneNode::ChildNodeIterator itChild = pSceneNode->getChildIterator();

    while (itChild.hasMoreElements() )
    {
       Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
       DestroyAllAttachedMovableObjects(pChildNode );
    }
  }
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

CRaycastResult::CRaycastResult(Ogre::Ray ray, Ogre::Plane plane)
{
	castRay(ray, ray.intersects(plane));
}

void
CRaycastResult::castRay(Ogre::Ray ray, std::pair<bool, Ogre::Real> result)
{
	m_bHit = result.first;
	m_fDist = result.second;
	m_vecPosition = ray.getPoint(m_fDist);
}

Ogre::Vector3
CRaycastResult::getPosition()
{
	return m_vecPosition;
}

bool
CRaycastResult::hasHit()
{
	return m_bHit;
}










#if 0


static const Ogre::String c_InternMeshGroup		= "Assets";
static const Ogre::String c_InterMeshPermGroup	= "Assets";

static Ogre::AnimationState* pState = NULL;
static Ogre::Entity* m_loadedEntity = NULL;
static Ogre::SceneManager		*m_sceneManager = NULL;

static float					m_visulBoneSize;
static std::vector<VisualBone> m_visualBones;
static std::vector<VisualLink> m_visualLinks;

static VisualBone				m_axisOnBone;
static Ogre::ColourValue		m_boneNameColour;

static int m_entitiesId = 0;

void updateBoneTips()
{
	std::vector<VisualLink>::const_iterator itor = m_visualLinks.begin();
	std::vector<VisualLink>::const_iterator end  = m_visualLinks.end();
	while(itor != end )
	{
		Ogre::Node *parentBone	= itor->visualBone.tagNode->getParent();
		Ogre::Bone *childBone	= itor->linkedChild;

		Ogre::Vector3 vDif(parentBone->_getDerivedOrientation().Inverse() *
							(childBone->_getDerivedPosition() - parentBone->_getDerivedPosition()) );
		const float diffLength = vDif.normalise();
		const Ogre::Quaternion qRot(Ogre::Vector3::UNIT_Y.getRotationTo(vDif ) );

		itor->visualBone.tagNode->setOrientation(qRot );
		itor->visualBone.tagNode->setScale(m_loadedEntity->getBoundingRadius() * m_visulBoneSize,
											 diffLength,
											 m_loadedEntity->getBoundingRadius() * m_visulBoneSize );
		
		++itor;
	}
}


void onRender(float delta)
{
  pState->addTime(delta);
  updateBoneTips();
}


void createBoneTip(Ogre::Bone *parentBone )
{
	Ogre::Bone::ChildNodeIterator childIt = parentBone->getChildIterator();
	while(childIt.hasMoreElements() )
	{
		VisualLink visualLink;

		//Ugh, unsafe upcast
		Ogre::Bone *childBone	= static_cast<Ogre::Bone*>(childIt.getNext());
		visualLink.linkedChild	= childBone;

		//TODO: Does anyone know a better way to skip (our own?) TagPoints?
		if(dynamic_cast<Ogre::TagPoint*>(childBone) != 0 )
			continue;

		//Link children of children
		if(childBone->numChildren() )
			createBoneTip(childBone );

		//Create an entity between parentNode and childBone
		Ogre::Vector3 vDif(parentBone->_getDerivedOrientation().Inverse() *
							(childBone->_getDerivedPosition() - parentBone->_getDerivedPosition()) );
		const float diffLength = vDif.normalise();
		const Ogre::Quaternion qRot(Ogre::Vector3::UNIT_Y.getRotationTo(vDif ) );

		//Skip Bones in the same position (flat tips only cause confusion)
		if(diffLength > 1e-6f )
		{
			visualLink.visualBone.entity = m_sceneManager->createEntity("BoneTip" +
															Ogre::StringConverter::toString(m_entitiesId++),
															"BoneTip.mesh", c_InterMeshPermGroup );
			visualLink.visualBone.tagNode = m_loadedEntity->attachObjectToBone(
															parentBone->getName(),
															visualLink.visualBone.entity, qRot );
			visualLink.visualBone.tagNode->setScale(m_loadedEntity->getBoundingRadius() * m_visulBoneSize,
													 diffLength,
													 m_loadedEntity->getBoundingRadius() * m_visulBoneSize );
			visualLink.visualBone.tagNode->setInheritScale(false );

			//Tips are rendered before the globe to make globe's colours sharper
			visualLink.visualBone.entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE - 1 );
			m_visualLinks.push_back(visualLink );
		}
	}
}


void showBones(bool withNames )
{
	if(!m_loadedEntity )
		return;

	Ogre::SkeletonInstance *skeletonInstance = m_loadedEntity->getSkeleton();

	if(skeletonInstance )
	{
		//Create BoneTips first, because TagPoints are listed as children of the bones too
		Ogre::Skeleton::BoneIterator itor = skeletonInstance->getRootBoneIterator();
		while(itor.hasMoreElements() )
			createBoneTip(itor.getNext() );

		itor = skeletonInstance->getBoneIterator();
		while(itor.hasMoreElements() )
		{
			Ogre::Bone *bone = itor.getNext();
			VisualBone visualBone;
			visualBone.entity  = m_sceneManager->createEntity("Bone" +
															Ogre::StringConverter::toString(m_entitiesId++),
															"BoneGlobe.mesh", c_InterMeshPermGroup );

			visualBone.tagNode = m_loadedEntity->attachObjectToBone(bone->getName(), visualBone.entity );
      float fRad = m_loadedEntity->getBoundingRadius();
			visualBone.tagNode->setScale(Ogre::Vector3(fRad *
														m_visulBoneSize) );

			visualBone.entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_SKIES_LATE );
			m_visualBones.push_back(visualBone );
		}
	}
}

void testAnimationPush()
{
  static bool b = true;
  
  b = !b;
  pState->setEnabled(b);
}

void testAnimationCmdUI(bool& a, bool& b)
{
  a = true;
  b = false;
}
#endif