#ifndef IPROJECT_MANAGER
# define IPROJECT_MANAGER

interface_class IProjectManager
{
public:
	virtual void														newProject(const std::string& projectName, const std::string& baseDir) = 0;
	virtual bool														loadProject(const std::string& projectFilePath, const std::string& ext) = 0;
	virtual void														saveProject() = 0;
	virtual void														saveProject(const std::string& projectFile, const std::string& projectFilePath, const std::string& ext) = 0;
	virtual void														closeProject() = 0;

public:
	virtual const std::string&							getProjectName() = 0;
	virtual const std::string&							getProjectDir() = 0;
  virtual const std::string&							getProjectFile() = 0;
  virtual bool                            isProjectLoaded() = 0;

public:
  Event1<const std::string&>              eventProjectLoaded;
  Event0<>                                eventProjectClosed;
  Event0<>                                eventProjectSaved;
};

#endif // !IPROJECT_MANAGER