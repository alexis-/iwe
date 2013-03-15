#ifndef PROJECT_MANAGER_H
# define PROJECT_MANAGER_H


class CProjectManager : public IProjectManager
{
public:
	static CProjectManager*									getInstance();
	~CProjectManager();

public:
	virtual void														newProject(const std::string& projectName, const std::string& baseDir);
	virtual bool														loadProject(const std::string& projectFilePath, const std::string& ext);
	virtual void														saveProject();
	virtual void														saveProject(const std::string& projectFile, const std::string& projectFilePath, const std::string& ext);
	virtual void														closeProject();

public:
	virtual const std::string&							getProjectName()
	{
		return m_sProjectName;
	}

	virtual const std::string&							getProjectDir()
	{
		return m_sBaseDir;
	}

	virtual const std::string&							getProjectFile()
	{
		return m_sProjectFile;
	}

  virtual bool                            isProjectLoaded()
  {
    return m_bProjectLoaded;
  }

	void																		setProjectDir(const std::string& sBaseDir)
	{
		m_sBaseDir = sBaseDir;
	}

	void																		setProjectName(const std::string& sProjectName)
	{
		m_sProjectName = sProjectName;
	}

private:
	void																		initProject();

private:
	CProjectManager();
	static CProjectManager*									m_pInst;

	std::string															m_sProjectName;
	std::string															m_sBaseDir;
	std::string															m_sProjectFile;
	std::string															m_sExt;
	std::string															m_sProjectFilePath;

  bool                                    m_bProjectLoaded;
};

#endif // !PROJECT_MANAGER_H