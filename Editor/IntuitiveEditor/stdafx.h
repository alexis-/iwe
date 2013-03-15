#pragma once


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

#include <stdexcept>
#include <string>
#include <list>

//#include <Windows.h> // Only for keys;... replace

#include "iwebase.h"

#include "Interface.h"

namespace Ogre
{
	class SceneNode;
	class Vector3;
	class Ray;
	class Plane;
	class TerrainGroup;
	class Terrain;
	class ManualObject;
};


#define NEW                 new
#define TRACE               printf // TODO: Replace this


const char									cRenderQueueViewGrid = 0x60;

const char*									getUniqueName(const std::string&						name);
bool												listContains(const std::list<std::string>&	list,
																				std::string											name);
bool												hasEnding(std::string const&								fullString,
																			std::string const&								ending);
bool												hasCIEnding(std::string const&							fullString,
																				std::string const&							ending);


#include "EditorDatas.h"

bool												checkPathExists(const char* szPath);
bool												isDirectory(const char* szPath);

class CIWEStream : public std::fstream
{
public:
	CIWEStream(const std::string& sFilename, uint uFlags);

public:

	template<typename T>
	CIWEStream& operator<<(const T& obj)
	{
		write((char*)&obj, sizeof(obj));

		return *this;
	}

	template<typename T>
	CIWEStream& operator>>(T& obj)
	{
		read((char*)&obj, sizeof(obj));

		return *this;
	}

	CIWEStream& operator<<(const std::string& str);
	CIWEStream& operator>>(std::string& str);

	CIWEStream& operator<<(const Ogre::Vector3& vec);
	CIWEStream& operator>>(Ogre::Vector3& vec);

	CIWEStream& operator<<(const Ogre::Quaternion& quat);
	CIWEStream& operator>>(Ogre::Quaternion& quat);
};

template<typename T>
bool												vecItemExists(const std::vector<T>& vector, const T& item)
{
	for (std::vector<T>::const_iterator it = vector.begin();
			 it != vector.end();
			 it++)
		if (*it == item)
			return true;

	return false;
}

template<typename T>
typename std::vector<T>::iterator		vecGetItemIterator(std::vector<T>& vector, const T& item)
{
	for (std::vector<T>::iterator it = vector.begin();
			 it != vector.end();
			 it++)
		if (*it == item)
			return it;

	return vector.end();
}