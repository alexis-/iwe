#ifndef VIEWPORTGRID_H
# define VIEWPORTGRID_H

namespace Ogre
{
	class ManualObject;
}

class CViewport;

class CViewportGrid : public IViewportGrid
{
public:
	CViewportGrid(int									iColumnCount,
								int									iRowCount,
								float								fUnit,
								const std::string&	sName,
								Ogre::Vector3				vecNormal,
								uint								viewportFlag);

public:
	void										OnCameraMoved(const Ogre::Vector3& vecCameraPosition);
	void										ResizeGrid(int iColumnDelta, int iRowDelta);

	inline Ogre::Vector3		getNormal()
	{
		return m_vecNormal;
	}

private:
	void										Render(bool update);

private:
	int											m_iColumnCount;
	int											m_iRowCount;
	float										m_fUnit;
	float										m_fGridBlockUnit;
	Ogre::Vector3						m_vecNormal;
	Ogre::SceneNode*				m_pSceneNode;
	Ogre::ManualObject*			m_pGridObject;
};

#endif // !VIEWPORTGRID_H