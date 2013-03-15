#ifndef IRESOURCE_MANAGER_H
# define IRESOURCE_MANAGER_H

# if !defined (_IWE_GUI ) && !defined (_DISABLE_OGRE )
#  include <OgreArchive.h>
# else
namespace Ogre
{
  class Archive;

  struct FileInfo {
	/// The archive in which the file has been found (for info when performing
	/// multi-Archive searches, note you should still open through ResourceGroupManager)
	    const Archive* archive;
      /// The file's fully qualified name
      std::string filename;
      /// Path name; separated by '/' and ending with '/'
      std::string path;
      /// Base filename
      std::string basename;
      /// Compressed size
      size_t compressedSize;
      /// Uncompressed size
      size_t uncompressedSize;
  };
}
# endif

interface_class IResourceManager
{
public:
  virtual ~IResourceManager() {};

public:
	virtual void										                                                update() = 0;
  virtual const std::vector<std::pair<const Ogre::FileInfo, const std::string>>&  getFiles() = 0;
  
  
  virtual void                                addResourceLocation(const std::string& pLocation, char* pType = "FileSystem",
                                                                  char* pGroup = "", bool bRecursive = false) = 0;
  virtual void                                ReloadMaterial(const std::string& materialName, const std::string& groupName,
                                                             const std::string& filename, bool parseMaterialScript) = 0;
	virtual void									              updateOnEveryRenderable() = 0;

  // Return the image data, must be freed !
  virtual void*                               getBitmapMaterial(const std::string& matName, const std::string& groupName,
                                                                uint& width, uint& height, uchar& bpp) = 0;

public:
  // const FileInfo& : Loaded file informations
  // const std::string& : Group name
  Event2<const Ogre::FileInfo&, const std::string&>                               eventOnFileLoaded;

  Event0<>                                                                        eventUpdateStarted;
  Event0<>                                                                        eventUpdateFinished;
};

#endif // !IRESOURCE_MANAGER_H