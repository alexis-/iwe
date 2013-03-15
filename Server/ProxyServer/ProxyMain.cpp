#include "stdafx.h"

#include "CLogger.h"
#include "CProxy.h"


#define PROXY_VERSION "0.1"

int main(int argc, char** argv)
{
  CLogger::init();

  ff::fmt(pan::notice, "Server v{0} started", PROXY_VERSION);

  char* pCfgFile = "proxy.cfg";

  if (argc > 1)
    pCfgFile = argv[1];

  if (!CProxyConfig::parse(pCfgFile))
  {
    ff::write(pan::critical, "Failed to load config '", pCfgFile, "'. Aborting.");
    exit(1);
  }

  CLogger::setLogLevel(CProxyConfig::uLogLevel);
  CLogger::setLogFile(CProxyConfig::sLogFile);

  CProxy* pProxy = CProxy::getInstance();
  if (!pProxy->start(CProxyConfig::sListenIP, CProxyConfig::uListenPort,
    CProxyConfig::sLoginSrvIP, CProxyConfig::uLoginSrvPort,
    CProxyConfig::vecAreasIP, CProxyConfig::uMaxUsers))
  {
    log_CRITICAL("Failed to start proxy server. Aborting.");
    exit(1);
  }

  while (true)
  {
    std::string sInput;
    std::cin >> sInput;

    if (sInput == "q" || sInput == "quit" || sInput == "e" || sInput == "exit")
      break;
  }

  pProxy->stop();

  return 0;
}