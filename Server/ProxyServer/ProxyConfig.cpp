#include "stdafx.h"

#ifdef WIN32
#pragma warning(disable : 4290 )
#endif // !WIN32

#include <libconfig.hh>
#include "ProxyConfig.h"

uint                        CProxyConfig::uMaxUsers = 0;

std::string                 CProxyConfig::sListenIP = "";
uint                        CProxyConfig::uListenPort = 0;

std::string                 CProxyConfig::sLoginSrvIP = "";
uint                        CProxyConfig::uLoginSrvPort = 0;

ushort                      CProxyConfig::uLogLevel = 4;
std::string                 CProxyConfig::sLogFile = "";

std::vector<s_serverInfos>  CProxyConfig::vecAreasIP = std::vector<s_serverInfos>();

bool
CProxyConfig::parse(const std::string& sFilePath)
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

    // Login server
    sLoginSrvIP = (const std::string&)config.lookup("LoginSrvIP");
    uLoginSrvPort = config.lookup("LoginSrvPort");

    // Area servers
    libconfig::Setting& areaConfig = config.lookup("AreaServers");

    for (int i = 0; i < areaConfig.getLength(); i++)
    {
      try
      {
        s_serverInfos srv;
        bool b = true;
        
        b = b && areaConfig[i].lookupValue("IP", srv.m_srvIP);
        b = b && areaConfig[i].lookupValue("Port", srv.m_srvPort);

        if (b)
          vecAreasIP.push_back(srv);
        else
          log_WARNING("Invalid area server configuration");
      }
      catch (std::exception ex)
      {
        ff::writeln(pan::warning, "Caugh exception while parsing area configuration : ", ex.what());

        continue;
      }
    }
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
    << uMaxUsers << " " << std::endl
    
    << std::setw(30) << std::left << "Area Servers =" << " ";

  for (uint i = 0; i < vecAreasIP.size(); i++)
  {
    s_serverInfos srvInfo = vecAreasIP[i];

    if (i == 0)
      std::cout << srvInfo.m_srvIP << ":" << srvInfo.m_srvPort << " " << std::endl;
    else
      std::cout << std::setw(31) << " " << srvInfo.m_srvIP << ":" << srvInfo.m_srvPort << " " << std::endl << std::endl;
  }

  std::cout << "-------------------------------------------------" << std::endl << std::endl;


  return true;
}