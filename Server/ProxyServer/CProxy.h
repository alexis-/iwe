#ifndef CPROXY_H
# define CPROXY_H

#include <SocketLayer.h>
#include <PluginInterface2.h>
#include <RakSleep.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <UDPForwarder.h>

struct s_clientInfos
{
  RakNet::RakPeerInterface*                 m_pMainSrv;
  std::vector<RakNet::RakPeerInterface*>    m_vecSupplSrv;
  FastDelegate<RakNet::PluginReceiveResult(RakNet::Packet*,
    s_clientInfos*)>                        m_sendFunc;

  RakNet::SystemAddress                     m_clientAddr;
  RakNet::RakNetGUID                        m_rakGUID;
  uint                                      m_uID;
};

class CProxy : public RakNet::PluginInterface2
{
public:
  static CProxy*                            getInstance();
  ~CProxy();

  bool                                      start(const std::string&                    sListenIP,
                                                  ushort                                uListenPort,
                                                  const std::string&                    sLoginSrvIP,
                                                  ushort                                uLoginSrvPort,
                                                  const std::vector<s_serverInfos>&     vecAreas,
                                                  ushort                                uMaxConnections);
  void                                      stop();

// Client -> Proxy
private:
	virtual RakNet::PluginReceiveResult       OnReceive(RakNet::Packet* packet);
	virtual void                              OnNewConnection(const RakNet::SystemAddress&          systemAddress,
                                                            RakNet::RakNetGUID                    rakNetGUID,
                                                            bool                                  isIncoming);
	virtual void                              OnClosedConnection(const RakNet::SystemAddress&       systemAddress,
                                                               RakNet::RakNetGUID                 rakNetGUID,
                                                               RakNet::PI2_LostConnectionReason   lostConnectionReason);

// Login <-> Proxy <-> Client
private:
  RakNet::PluginReceiveResult               OnLoginClientReceive(RakNet::Packet* pPacket, s_clientInfos*);

  bool                                      HandleLoginSrvPacket(RakNet::Packet*          pPacket,
                                                                 RakNet::SystemAddress&   clientAddr,
                                                                 RakNet::BitStream&       inStr,
                                                                 uchar                    opCode);
  bool                                      OnLoginSrvReceive(RakNet::Packet* pPacket);

// Area <-> Proxy <-> Client
private:


private:
  void                                      updateThread();

private:
  RakNet::SystemAddress                     m_listenAddress;
  RakNet::RakPeerInterface*                 m_pListenInterface;
  
  RakNet::SystemAddress                     m_loginSrvAddress;
  RakNet::RakPeerInterface*                 m_pLoginSrvInterface;

  std::vector<RakNet::RakPeerInterface*>    m_vecAreasInterfaces;

  //std::vector<s_clientInfos*>               m_vecClientAddr;
  std::map<RakNet::SystemAddress,
           s_clientInfos*>                  m_mapClientAddr;
  std::map<uint, s_clientInfos*>            m_mapClientId;

  bool                                      m_bStarted;

  boost::thread*                            m_pUpdateThread;
  bool                                      m_bQuit;

private:
  CProxy();
  static CProxy*                            m_pInst;
};

#endif