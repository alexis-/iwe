#ifndef CLOGGER_H
# define CLOGGER_H


static_class CLogger
{
public:
  static void     init();
  static void     cleanup();

  static void     setLogFile(const std::string& sLogFile);
  static void     setLogLevel(ushort uLvl);
};


#endif // !CLOGGER_H