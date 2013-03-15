#ifndef SERVER_UTILS_H
# define SERVER_UTILS_H

# include <sstream>

# include <RakPeerInterface.h>
# include <RakSleep.h>
# include <MessageIdentifiers.h>
# include <BitStream.h>
# include <StringCompressor.h>
# include <GetTime.h>

static int
ping(RakNet::RakPeerInterface* pItf, const std::string& sIP, ushort uPort)
{
  int ret = -1;

  if (!pItf || !pItf->Ping(sIP.c_str(), uPort, false))
    return ret;

  RakSleep(1);
  RakNet::Packet* p = pItf->Receive();

  if (p && p->data[0] == ID_UNCONNECTED_PONG)
  {
    RakNet::TimeMS time;
    RakNet::BitStream bsIn(p->data, p->length, false);

    bsIn.IgnoreBytes(1);
    bsIn.Read(time);

    ret = (RakNet::GetTimeMS() - time);
  }

  return ret;
}

static std::string
int_to_hex(int n)
{
  std::ostringstream oss;

  oss << "0x" << std::hex << n;

  return(oss.str());
}

#endif // !SERVER_UTILS_H