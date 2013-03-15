// Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

extern "C" IWEAPI IEngineInterface* getEngineInterface();


int main(int argc, char* argv[])
{
  IEngineInterface* pEngine = getEngineInterface();

  pEngine->initialize(NULL);

	return 0;
}