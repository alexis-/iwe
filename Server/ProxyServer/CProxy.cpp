#include "stdafx.h"

#include "CProxy.h"



CProxy* CProxy::m_pInst = NULL;
const RakNet::TimeMS timeoutOnNoDataMS = 6000;


CProxy*
CProxy::getInstance()
{
  if (!m_pInst)
    m_pInst = new CProxy();

  return m_pInst;
}

CProxy::CProxy()
  : m_pListenInterface(NULL),
    m_pLoginSrvInterface(NULL),
    m_bStarted(false),
    m_bQuit(false),
    m_pUpdateThread(NULL)
{
}

CProxy::~CProxy()
{
}



//////////////////////////////////////////////////////////////////////////////
//
// Init stuffs
//
//////////////////////////////////////////////////////////////////////////////

bool
CProxy::start(const std::string&                  sListenIP,
              ushort                              uListenPort,
              const std::string&                  sLoginSrvIP,
              ushort                              uLoginSrvPort,
              const std::vector<s_serverInfos>&   vecAreas,
              ushort                              uMaxConnections)
{
  // Check if listen address is correctly formated
  if (!m_listenAddress.FromStringExplicitPort(sListenIP.c_str(), uListenPort))
  {
    log_ERROR("Invalid listen address. Aborting.");
    return false;
  }


  // Start listen interface
  RakNet::SocketDescriptor listenSocket(uListenPort, sListenIP.c_str());
  m_pListenInterface = RakNet::RakPeerInterface::GetInstance();

  if (m_pListenInterface->Startup(uMaxConnections, &listenSocket, 1) != RakNet::RAKNET_STARTED)
  {
    log_ERROR("Failed to startup Raknet interface. Aborting.");
    stop();
    return false;
  }

  m_pListenInterface->SetMaximumIncomingConnections(uMaxConnections);
  ff::write(pan::notice, "Started listening on ", sListenIP, ":", uListenPort, ".");
  ff::write(pan::notice, "Accepting max ", uMaxConnections, " connections.");

  ushort uTmpPort = 49777;

  // Start login server interface
  {
    // Check if login server address is correctly formated
    if (!m_loginSrvAddress.FromStringExplicitPort(sLoginSrvIP.c_str(), uLoginSrvPort))
    {
      log_ERROR("Invalid login server address. Aborting.");
      stop();
      return false;
    }

    RakNet::SocketDescriptor loginSocketDesc(++uTmpPort, NULL);

	  while (RakNet::SocketLayer::IsPortInUse(loginSocketDesc.port, loginSocketDesc.hostAddress, loginSocketDesc.socketFamily))
      loginSocketDesc.port = uTmpPort++;
      
    // Create and startup login server interface
    m_pLoginSrvInterface = RakNet::RakPeerInterface::GetInstance();

	  if (m_pLoginSrvInterface->Startup(1, &loginSocketDesc, 1) != RakNet::RAKNET_STARTED)
    {
      log_ERROR("Failed to startup login server interface.");
      stop();
      return false;
    }

    int latency = ping(m_pLoginSrvInterface, sLoginSrvIP, uLoginSrvPort);
    
    if (latency < 0)
      ff::write(pan::warning, "Failed to ping login server (", sLoginSrvIP, ":", uLoginSrvPort, ").");
    else
      ff::write(pan::notice, "Pinged login server (", sLoginSrvIP, ":", uLoginSrvPort, ") - ", latency, "ms.");

    // Attempt connection
    if (m_pLoginSrvInterface->Connect(sLoginSrvIP.c_str(), uLoginSrvPort, NULL, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
    {
      ff::write(pan::error, "Connection to login server (", sLoginSrvIP, ":", uLoginSrvPort, ") failed.");
      stop();
      return false;
    }

    m_pLoginSrvInterface->SetOccasionalPing(true);
  }

  // Connect to area servers
  for (std::vector<s_serverInfos>::const_iterator it = vecAreas.begin();
       it != vecAreas.end();
       it++)
  {
    RakNet::SocketDescriptor areaSocketDesc(uTmpPort++, NULL);

	  while (RakNet::SocketLayer::IsPortInUse(areaSocketDesc.port, areaSocketDesc.hostAddress, areaSocketDesc.socketFamily))
      areaSocketDesc.port = uTmpPort++;

    RakNet::RakPeerInterface* pAreaItf = RakNet::RakPeerInterface::GetInstance();

	  if (pAreaItf->Startup(1, &areaSocketDesc, 1) != RakNet::RAKNET_STARTED)
    {
      log_ERROR("Failed to startup an area server interface.");
      continue;
    }

    if (pAreaItf->Connect((*it).m_srvIP.c_str(), (*it).m_srvPort, NULL, 0) != RakNet::CONNECTION_ATTEMPT_STARTED)
    {
      ff::write(pan::error, "Connection attempt to area server (", (*it).m_srvIP, ":", (*it).m_srvPort, ") failed.");
      continue;
    }

    pAreaItf->SetOccasionalPing(true);

    // TODO: Do something here with this connection
  }


  // Proxy server started successfuly
  m_pListenInterface->AttachPlugin(this);
  m_bStarted = true;

  m_bQuit = false;
  m_pUpdateThread = new boost::thread(FastDelegate0<>(this, &CProxy::updateThread));

  return m_bStarted;
}

void
CProxy::stop()
{
  if (m_pUpdateThread)
  {
    // Stop update thread
    m_bQuit = true;

    m_pUpdateThread->join();
    delete m_pUpdateThread;
    m_pUpdateThread = NULL;
  }
  
  // Close listen interface
  if (m_pListenInterface)
  {
    m_pListenInterface->DetachPlugin(this);

    m_pListenInterface->Shutdown(5);
    RakNet::RakPeerInterface::DestroyInstance(m_pListenInterface);

    m_pListenInterface = NULL;
  }
  
  // Close Login server interface
  if (m_pLoginSrvInterface)
  {
    //m_pLoginSrvInterface->Shutdown(5);
    RakNet::RakPeerInterface::DestroyInstance(m_pLoginSrvInterface);

    m_pLoginSrvInterface = NULL;
  }

  // Close areas interfaces
  

  m_bStarted = false;
}



//////////////////////////////////////////////////////////////////////////////
//
// Update thread
//
//////////////////////////////////////////////////////////////////////////////

void
CProxy::updateThread()
{
  // Threaded update loop
  while (!m_bQuit)
  {
    try
    {
      while (m_pListenInterface->Receive())
        ;

      {
        RakNet::Packet* pPacket = NULL;

        for (pPacket = m_pLoginSrvInterface->Receive(); pPacket; m_pLoginSrvInterface->DeallocatePacket(pPacket), pPacket = m_pLoginSrvInterface->Receive())
          if (!OnLoginSrvReceive(pPacket))
            break;
      }
    }
    catch (std::exception ex)
    {
      ff::fmt(pan::critical, "Caught an exception in updateThread ({0}).", ex.what());
    }

    RakSleep(10);
  }
}



//////////////////////////////////////////////////////////////////////////////
//
// Common
//
//////////////////////////////////////////////////////////////////////////////

char handleDefaultPacket(uchar opCode, const char* pInfo)
{
  switch (opCode)
  {
  case ID_CONNECTION_REQUEST_ACCEPTED:
    ff::write(pan::notice, pInfo, "connection accepted.");
    return 1;

  case ID_ALREADY_CONNECTED:
    ff::write(pan::warning, pInfo, "already connected.");
    return 1;

  case ID_DISCONNECTION_NOTIFICATION:
    ff::write(pan::notice, pInfo, "disconnected.");
    return 2;

    // print out errors
  case ID_CONNECTION_ATTEMPT_FAILED:
    ff::write(pan::warning, pInfo, "connection attempt failed.");
    return 3;

  case ID_NO_FREE_INCOMING_CONNECTIONS:
    ff::write(pan::warning, pInfo, "no free incoming connections.");
    return 3;

  case ID_CONNECTION_LOST:
    ff::write(pan::warning, pInfo, "connection was lost.");
    return 3;

  case ID_CONNECTION_BANNED:
    ff::write(pan::warning, pInfo, "banned from server.");
    return 3;

  case ID_INVALID_PASSWORD:
    ff::write(pan::warning, pInfo, "invalid password.");
    return 3;

  case ID_INCOMPATIBLE_PROTOCOL_VERSION:
    ff::write(pan::warning, pInfo, "incompatible protocol version.");
    return 3;
  }

  return 0;
}



//////////////////////////////////////////////////////////////////////////////
//
// Proxy server handling methods
//
//////////////////////////////////////////////////////////////////////////////

// Proxy    <--     Client
//
// Triggered when a client sends a message.
RakNet::PluginReceiveResult
CProxy::OnReceive(RakNet::Packet* packet)
{
  if (!packet)
    return RakNet::RR_CONTINUE_PROCESSING;

  ff::fmt(pan::debug, "({0}) Packet<{1}>", packet->systemAddress.ToString(false),
    int_to_hex((int)packet->data[0]));

  std::map<RakNet::SystemAddress, s_clientInfos*>::iterator it = m_mapClientAddr.find(packet->systemAddress);

  if (it != m_mapClientAddr.end())
  {
    s_clientInfos* pInfos = (*it).second;
    return pInfos->m_sendFunc(packet, pInfos);
  }

  return RakNet::RR_CONTINUE_PROCESSING;
}

// Triggered when a new client connect to the proxy.
void
CProxy::OnNewConnection(const RakNet::SystemAddress&      systemAddress,
                        RakNet::RakNetGUID                rakNetGUID,
                        bool                              isIncoming)
{
  ff::fmt(pan::notice, "({0}) New connection", systemAddress.ToString(false));

  // Setup client infos
  s_clientInfos* pInfos = new s_clientInfos();
  pInfos->m_clientAddr = systemAddress;
  pInfos->m_pMainSrv = m_pLoginSrvInterface;
  pInfos->m_rakGUID = rakNetGUID;
  pInfos->m_uID = 0;
  pInfos->m_sendFunc =
    FastDelegate<RakNet::PluginReceiveResult(RakNet::Packet*, s_clientInfos*)>(this, &CProxy::OnLoginClientReceive);

  // For now we can only identify this client by his address.
  m_mapClientAddr[systemAddress] = pInfos;
}

void
CProxy::OnClosedConnection(const RakNet::SystemAddress&       systemAddress,
                           RakNet::RakNetGUID                 rakNetGUID,
                           RakNet::PI2_LostConnectionReason   lostConnectionReason)
{
  ff::fmt(pan::notice, "({0}) Closed connection", systemAddress.ToString(false));

  s_clientInfos* pInfos = m_mapClientAddr[systemAddress];

  m_mapClientAddr.erase(systemAddress);

  std::map<uint, s_clientInfos*>::iterator it = m_mapClientId.find(pInfos->m_uID);
  if (it != m_mapClientId.end())
    m_mapClientId.erase(it);
}



//////////////////////////////////////////////////////////////////////////////
//
// Login server handling methods
//
//////////////////////////////////////////////////////////////////////////////

// Login    <--     Client
//
// Handle message forwarding from clients to the login server
RakNet::PluginReceiveResult
CProxy::OnLoginClientReceive(RakNet::Packet* pPacket, s_clientInfos* pInfos)
{
  if (!pPacket || pPacket->length == 0)
    return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;

  RakNet::BitStream outStr;

  outStr.Write(pPacket->data[0]);
  outStr.Write(pInfos->m_clientAddr);

  if (pPacket->length > 1)
    outStr.Write((const char*)&pPacket->data[1], pPacket->length - 1);

  // Login messages have lower priority.. There's no rush involved in logging in
  m_pLoginSrvInterface->Send(&outStr, LOW_PRIORITY, RELIABLE_SEQUENCED, 0, m_loginSrvAddress, false);
  
  return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
}

// Returning false prevents the proxy from forwarding current packet to the client
bool
CProxy::HandleLoginSrvPacket(RakNet::Packet* pPacket, RakNet::SystemAddress& clientAddr, RakNet::BitStream& inStr, uchar opCode)
{
  uint off = inStr.GetReadOffset();

  switch (opCode)
  {
  case Opcodes::OP_LOGIN:
    bool bLoggedIn = false;

    inStr.Read(bLoggedIn);

    if (bLoggedIn)
    {
      ff::fmt(pan::debug, "({0}) Player successfully logged in.", clientAddr.ToString(false));

      // Do the switch to Area server here
    }

    break;
  }

  inStr.SetReadOffset(off);

  return true;
}

// Login    -->     Client
//
// Handle message forwarding from the login server to clients
bool
CProxy::OnLoginSrvReceive(RakNet::Packet* pPacket)
{
  if (!pPacket)
    return true;

  uchar* pDatas = pPacket->data;
  uint len = pPacket->length;
  int defaultHandleRet = 0;
  
  RakNet::BitStream strIn(pDatas, len, false);
  uchar opCode;

  if (!strIn.Read(opCode))
    return true;
  
  if ((defaultHandleRet = handleDefaultPacket(opCode, "Login server: ")) == 0 &&
      m_pLoginSrvInterface->NumberOfConnections() > 0)
  {
    RakNet::SystemAddress clientAddr;

    if (!strIn.Read(clientAddr))
      return true;

    // This may not be necessary
    std::map<RakNet::SystemAddress, s_clientInfos*>::iterator it = m_mapClientAddr.find(clientAddr);

    if (it == m_mapClientAddr.end())
      return true;

    // !---------

    if (!HandleLoginSrvPacket(pPacket, clientAddr, strIn, opCode))
      return true;
    
    RakNet::BitStream strOut;
    int off = strIn.GetReadOffset() / 8;

    strOut.Write(opCode);

    if (off < len)
      strOut.Write((const char*)&pDatas[off], len - off);

    m_pListenInterface->Send(&strOut, LOW_PRIORITY, RELIABLE_SEQUENCED, 0, clientAddr, false);
  }

  else if (defaultHandleRet > 1)
  {
    // TODO: Attempt delayed reconnection

    log_CRITICAL("Not connected to login server. Aborting.");

    m_pUpdateThread = NULL;
    m_bQuit = true;

    return false;
  }

  return true;
}
