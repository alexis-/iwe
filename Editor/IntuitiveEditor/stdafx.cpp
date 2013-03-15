#include "stdafx.h"


const char*
getUniqueName(const std::string& name)
{
	static std::map<std::string, unsigned long>		nameMap;
	static char																		pNameRet[2048];
	std::string																		sName(name);
	unsigned long																	id = 0;

	if (nameMap.find(sName) != nameMap.end())
	{
		id = ++nameMap[sName];
		nameMap[sName] = id;
	}
	else
		nameMap[sName] = id;

  char tmp[3];
  _itoa(id, tmp, 3);
	sName += ".";
  sName += tmp; //Ogre::StringConverter::toString(id);
	strncpy(pNameRet, sName.c_str(), 2048);

	return pNameRet;
}

bool
listContains(const std::list<std::string>& list, std::string name)
{
	for (std::list<std::string>::const_iterator it = list.begin();
			 it != list.end();
			 it++)
		if (*it == name)
			return true;

	return false;
}

std::string upper_string(const std::string& str)
{
  std::string upper;
  std::transform(str.begin(), str.end(), std::back_inserter(upper), std::toupper);
  return upper;
}

bool hasCIEnding(std::string const &fullString, std::string const &ending)
{
	std::string newFullString = upper_string(fullString);
	std::string newEnding = upper_string(ending);
  unsigned int lastMatchPos = newFullString.rfind(newEnding); // Find the last occurrence of ending
  bool isEnding = lastMatchPos != std::string::npos; // Make sure it's found at least once

  // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
  for(int i = lastMatchPos + newEnding.length(); (i < newFullString.length()) && isEnding; i++)
  {
    if((newFullString[i] != '\n') &&
        (newFullString[i] != '\r') )
    {
      isEnding = false;
    }
  }

  return isEnding;
}

bool hasEnding(std::string const &fullString, std::string const &ending)
{
  unsigned int lastMatchPos = fullString.rfind(ending); // Find the last occurrence of ending
  bool isEnding = lastMatchPos != std::string::npos; // Make sure it's found at least once

  // If the string was found, make sure that any characters that follow it are the ones we're trying to ignore
  for(int i = lastMatchPos + ending.length(); (i < fullString.length()) && isEnding; i++)
  {
    if((fullString[i] != '\n') &&
        (fullString[i] != '\r') )
    {
      isEnding = false;
    }
  }

  return isEnding;
}



///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////

CIWEStream::CIWEStream(const std::string& sFilename, uint uFlags)
	: std::fstream(sFilename, uFlags)
{
}

CIWEStream&
CIWEStream::operator<<(const std::string& str)
{
	*this << str.length();
	write(str.c_str(), str.length());

	return *this;
}

CIWEStream&
CIWEStream::operator>>(std::string& str)
{
	int len = 0;

	*this >> len;
	char* buff = NEW char[len + 1];

	buff[len] = 0;
	read(buff, len);

	str = buff;
	delete buff;

	return *this;
}

CIWEStream&
CIWEStream::operator<<(const Ogre::Vector3& vec)
{
	*this << vec.x;
	*this << vec.y;
	*this << vec.z;

	return *this;
}

CIWEStream&
CIWEStream::operator>>(Ogre::Vector3& vec)
{
	*this >> vec.x;
	*this >> vec.y;
	*this >> vec.z;

	return *this;
}


CIWEStream&
CIWEStream::operator<<(const Ogre::Quaternion& quat)
{
	*this << quat.w;
	*this << quat.x;
	*this << quat.y;
	*this << quat.z;

	return *this;
}

CIWEStream&
CIWEStream::operator>>(Ogre::Quaternion& quat)
{
	*this >> quat.w;
	*this >> quat.x;
	*this >> quat.y;
	*this >> quat.z;

	return *this;
}