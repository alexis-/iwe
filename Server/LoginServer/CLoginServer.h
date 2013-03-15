#ifndef CLOGIN_SERVER_H
# define CLOGIN_SERVER_H

#include <SocketLayer.h>
#include <PluginInterface2.h>
#include <RakSleep.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

class CLoginServer : public RakNet::PluginInterface2
{
public:
  static CLoginServer*                      getInstance();
  ~CLoginServer();

  bool                                      start(const std::string&                    sListenIP,
                                                  ushort                                uListenPort,
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
  
private:
  void                                      updateThread();

private:
  RakNet::SystemAddress                     m_listenAddress;
  RakNet::RakPeerInterface*                 m_pListenInterface;

  bool                                      m_bStarted;

  boost::thread*                            m_pUpdateThread;
  bool                                      m_bQuit;

private:
  static CLoginServer*                      m_pInst;
  CLoginServer();
};

#endif // !CLOGIN_SERVER_H