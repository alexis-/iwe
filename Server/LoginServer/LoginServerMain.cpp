// LoginServer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "CLoginServer.h"
#include "CLogger.h"
#include "LoginConfig.h"

#define LOGINSRV_VERSION "0.1"

int main(int argc, char** argv)
{
  CLogger::init();

  ff::fmt(pan::notice, "Server v{0} started", LOGINSRV_VERSION);

  char* pCfgFile = "login.cfg";

  if (argc > 1)
    pCfgFile = argv[1];

  if (!CLoginConfig::parse(pCfgFile))
  {
    ff::write(pan::critical, "Failed to load config '", pCfgFile, "'. Aborting.");

    exit(1);
  }

  CLogger::setLogLevel(CLoginConfig::uLogLevel);
  CLogger::setLogFile(CLoginConfig::sLogFile);

  
  CLoginServer* pLoginSrv = CLoginServer::getInstance();
  if (!pLoginSrv->start(CLoginConfig::sListenIP,
                        CLoginConfig::uListenPort,
                        CLoginConfig::uMaxUsers))
  {
    log_CRITICAL("Failed to start login server. Aborting.");
    exit(1);
  }

  while (true)
  {
    std::string sInput;
    std::cin >> sInput;

    if (sInput == "q" || sInput == "quit" || sInput == "e" || sInput == "exit")
      break;
  }

  pLoginSrv->stop();

	return 0;
}
