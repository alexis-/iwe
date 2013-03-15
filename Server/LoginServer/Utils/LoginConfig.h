#ifndef LOGIN_CONFIG_H
# define LOGIN_CONFIG_H

static_class CLoginConfig
{
public:
  static bool                           parse(const std::string& sFilePath);

public:
  static std::string                    sListenIP;
  static uint                           uListenPort;
  static uint                           uMaxUsers;

  static std::string                    sLogFile;
  static ushort                         uLogLevel;
};

#endif // !LOGIN_CONFIG_H