#ifndef ENGINE_UTILS_H
# define ENGINE_UTILS_H


class CViewport;

class CEngineUtils
{
public:

	static Ogre::Vector3					GetPlaneCoord(float x, float y, CViewport* pViewport, const Ogre::Vector3& vecNormal, const Ogre::Vector3& vecPos);
	static Ogre::Vector3					GetHeightCoord(float x, float y, CViewport* pViewport, const float fHeight);

	static void*									getBitmapMaterial(const std::string& matName, const std::string& groupName, uint& width, uint& height, uchar& bpp);

  static void                   ReloadMaterial(const std::string& materialName, const std::string& groupName, const std::string& filename, bool parseMaterialScript);
	static void										updateOnEveryRenderable();
};

#endif // !ENGINE_UTILS_H