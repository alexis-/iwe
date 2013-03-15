#include "stdafx.h"

#include <pantheios/implicit_link/core.h>
#include <pantheios/implicit_link/fe.simple.WithCallback.h>
#include <pantheios/implicit_link/be.N.h>
#include <pantheios/implicit_link/be.file.WithCallback.h>
#include <pantheios/implicit_link/be.fprintf.WithCallback.h>

#include <pantheios/frontends/stock.h>
#include <pantheios/frontends/fe.simple.h>
#include <pantheios/backends/be.N.h>
#include <pantheios/backends/bec.file.h> 
#include <pantheios/backends/bec.fprintf.h>

#include "CLogger.h"


pan_be_N_t  PAN_BE_N_BACKEND_LIST[] =
{
    PANTHEIOS_BE_N_STDFORM_ENTRY(1, pantheios_be_file, 0),
    PANTHEIOS_BE_N_STDFORM_ENTRY(2, pantheios_be_fprintf, 0),

    PANTHEIOS_BE_N_TERMINATOR_ENTRY
};

PANTHEIOS_CALL(void) pantheios_be_fprintf_getAppInit(
      int                     backEndId
  ,   pan_be_fprintf_init_t*  init)
{
  init->flags = PANTHEIOS_BE_INIT_F_CUSTOM_MASK
    | PANTHEIOS_BE_INIT_F_NO_PROCESS_ID
    | PANTHEIOS_BE_INIT_F_NO_SEVERITY
    | PANTHEIOS_BE_INIT_F_NO_THREAD_ID;
}

PANTHEIOS_CALL(void) pantheios_be_file_getAppInit(
      int                     backEndId
  ,   pan_be_file_init_t*     init)
{
  init->flags = PANTHEIOS_BE_INIT_F_CUSTOM_MASK
    | PANTHEIOS_BE_INIT_F_NO_PROCESS_ID
    | PANTHEIOS_BE_INIT_F_NO_THREAD_ID;

  init->fileName = "startup.log";
}

PANTHEIOS_CALL(PAN_CHAR_T const*) pantheios_fe_getAppProcessIdentity(void)
{
  return "LoginServer";
}

void
  CLogger::init()
{
  if (pantheios::init())
  {
    log_CRITICAL("Failed to init logging\n");
    exit(1);
  }

  pantheios_be_file_setFilePath("startup.log",
    PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_ALL);
  pantheios_fe_simple_setSeverityCeiling(7);
}

void
  CLogger::cleanup()
{
  pantheios::pantheios_uninit();
}

void
  CLogger::setLogFile(const std::string& sLogFile)
{
  if (sLogFile == "")
    return;

  pantheios_be_file_setFilePath(sLogFile.c_str(),
    PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BE_FILE_F_TRUNCATE, PANTHEIOS_BEID_ALL);
}

void
  CLogger::setLogLevel(ushort uLvl)
{
  pantheios_fe_simple_setSeverityCeiling(uLvl);
}