#include "stdafx.h"

#ifdef WIN32
#pragma warning(disable : 4290 )
#endif // !WIN32

#include <libconfig.hh>
#include "LoginConfig.h"

uint                        CLoginConfig::uMaxUsers = 0;

std::string                 CLoginConfig::sListenIP = "";
uint                        CLoginConfig::uListenPort = 0;

ushort                      CLoginConfig::uLogLevel = 4;
std::string                 CLoginConfig::sLogFile = "";


bool
CLoginConfig::parse(const std::string& sFilePath)
{
  libconfig::Config config;

  log_NOTICE((const char*)"Parsing proxy configuration file\n");

  try
  {
    // Load config file
    config.readFile(sFilePath.c_str());

    // Logging settings
    try
    {
      uLogLevel = (ushort)((int)config.lookup("LogLevel"));
    }
    catch (std::exception ex)
    {
    }
    
    try
    {
      sLogFile = (const std::string&)config.lookup("LogFile");
    }
    catch (std::exception ex)
    {
    }

    // Proxy server
    sListenIP = (const std::string&)config.lookup("ListenIP");
    uListenPort = config.lookup("ListenPort");
    uMaxUsers = config.lookup("MaxUsers");
  }
  catch (std::exception ex)
  {
    log_CRITICAL(ex.what());
    return false;
  }

  // Display loaded config
  std::cout << "Proxy server configuration '" << sFilePath.c_str() << "' loaded." << std::endl << std::endl

    << "-------------------------------------------------" << std::endl

    << std::setw(30) << std::left << "Listen address =" << " "
    << sListenIP << ":" << uListenPort << " " << std::endl

    << std::setw(30) << std::left << "Max users =" << " "
    << uMaxUsers << " " << std::endl;

  std::cout << "-------------------------------------------------" << std::endl << std::endl;


  return true;
}