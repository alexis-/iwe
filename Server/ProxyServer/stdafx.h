#ifndef STDAFX_H
# define STDAFX_H

# include "../Common/serverbase.h"

# include <iostream>



struct s_serverInfos
{
  std::string         m_srvIP;
  uint                m_srvPort;
};


# include "ProxyConfig.h"


#endif // !STDAFX_H