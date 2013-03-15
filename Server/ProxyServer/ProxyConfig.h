#ifndef PROXY_CONFIG_H
# define PROXY_CONFIG_H

static_class CProxyConfig
{
public:
  static bool                           parse(const std::string& sFilePath);

public:
  static uint                           uMaxUsers;

  static std::string                    sListenIP;
  static uint                           uListenPort;

  static std::string                    sLoginSrvIP;
  static uint                           uLoginSrvPort;

  static std::vector<s_serverInfos>     vecAreasIP;

  static std::string                    sLogFile;
  static ushort                         uLogLevel;
};

#endif // !PROXY_CONFIG_H