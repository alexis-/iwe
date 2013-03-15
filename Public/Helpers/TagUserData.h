#ifndef TAG_USER_DATAS_H
# define TAG_USER_DATAS_H

# include <map>
# include <string>

class UserDataHolder
{
public:
  bool          setUserData(void* pData, const std::string& sDataTag, bool bReplace = true)
  {
    if (!bReplace && m_mapDatas.find(sDataTag) != m_mapDatas.end())
      return false;

    m_mapDatas[sDataTag] = pData;

    return true;
  }

  void*         getUserData(const std::string& sDataTag) const
  {
    std::map<std::string, void*>::const_iterator it = m_mapDatas.find(sDataTag);

    return (it == m_mapDatas.end()) ? NULL : (*it).second;
  }

private:
  std::map<std::string, void*> m_mapDatas;
};

#endif // !TAG_USER_DATAS_H