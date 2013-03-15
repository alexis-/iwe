
#pragma once

#include "iwebase.h"
#include "Interface.h"

#define NEW                 new
#define TRACE               printf

const int										iMaxFPS = 30;
const int										iMaxFPSDelay = 1000 / iMaxFPS;

//////////////////////////////////////////////////////////////////////////////////////////////

const char*									getUniqueName(const std::string&						name);

bool												listContains(const std::list<std::string>&	list,
																				std::string											name);

bool												hasEnding(std::string const&								fullString,
																			std::string const&								ending);
bool												hasCIEnding(std::string const&							fullString,
																				std::string const&							ending);