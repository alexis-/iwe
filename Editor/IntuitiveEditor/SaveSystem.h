#ifndef SAVE_SYSTEM_H
# define SAVE_SYSTEM_H

abstract_class CSaveSystem
{
public:
	CSaveSystem(std::string name, std::string ext);

public:
	virtual bool									OnSave(const std::string& filename) = 0;
	virtual bool									OnLoad(const std::string& filename) = 0;

public:
	inline const std::string&			getName() const
	{
		return m_sName;
	}

	inline const std::string&			getExt() const
	{
		return m_sExt;
	}

private:
	std::string										m_sName;
	std::string										m_sExt;
};

#endif // !SAVE_SYSTEM_H