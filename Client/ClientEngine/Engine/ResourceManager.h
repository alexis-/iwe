#ifndef RESOURCE_MANAGER_H
# define RESOURCE_MANAGER_H

# include <vector>

namespace Ogre
{
	struct FileInfo;
}

class CResourceManager : public IResourceManager
{
public:
	static CResourceManager*				getInstance();
	~CResourceManager();

public:
	virtual void										update();
  virtual const std::vector<std::pair<const Ogre::FileInfo, const std::string>>&  getFiles();
  virtual void                    addResourceLocation(const std::string& pLocation, char* pType = "FileSystem", char* pGroup = "", bool bRecursive = false);

  virtual void                    ReloadMaterial(const std::string& materialName, const std::string& groupName,
                                                 const std::string& filename, bool parseMaterialScript);
	virtual void									  updateOnEveryRenderable();

  virtual void*                   getBitmapMaterial(const std::string& matName, const std::string& groupName,
                                                    uint& width, uint& height, uchar& bpp);

private:
	CResourceManager();
	static CResourceManager*				m_pInst;

  std::vector<std::pair<const Ogre::FileInfo, const std::string>> m_vecResources;
};

#endif // !RESOURCE_MANAGER_H