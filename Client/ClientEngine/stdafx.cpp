#include "stdafx.h"

#include <string>
#include <map>

#include <OgreStringConverter.h>


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

	sName += "." + Ogre::StringConverter::toString(id);
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