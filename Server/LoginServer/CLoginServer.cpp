#include "stdafx.h"

#include "CLoginServer.h"


CLoginServer* CLoginServer::m_pInst = NULL;



CLoginServer*
CLoginServer::getInstance()
{
  if (!m_pInst)
    m_pInst = new CLoginServer();

  return m_pInst;
}

CLoginServer::CLoginServer()
  : m_pListenInterface(NULL)
{
}

CLoginServer::~CLoginServer()
{
}



//////////////////////////////////////////////////////////////////////////////
//
// Init stuffs
//
//////////////////////////////////////////////////////////////////////////////

bool
CLoginServer::start(const std::string&                    sListenIP,
                    ushort                                uListenPort,
                    ushort                                uMaxConnections)
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


  // Login server started successfuly
  m_pListenInterface->AttachPlugin(this);
  m_bStarted = true;

  m_bQuit = false;
  m_pUpdateThread = new boost::thread(FastDelegate0<>(this, &CLoginServer::updateThread));

  return m_bStarted;
}

void
CLoginServer::stop()
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
}



//////////////////////////////////////////////////////////////////////////////
//
// Update thread
//
//////////////////////////////////////////////////////////////////////////////

void
CLoginServer::updateThread()
{
  // Threaded update loop
  while (!m_bQuit)
  {
    try
    {
      while (m_pListenInterface->Receive())
        ;
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

bool
handleDefaultPacket(uchar opCode, RakNet::SystemAddress& addr)
{
  switch (opCode)
  {
  case ID_CONNECTION_REQUEST_ACCEPTED:
    ff::write(pan::notice, "Connection accepted.");
    break;

  case ID_ALREADY_CONNECTED:
    ff::write(pan::warning, "Already connected.");
    break;

  case ID_DISCONNECTION_NOTIFICATION:
    ff::write(pan::notice, "Disconnected.");
    break;

    // print out errors
  case ID_CONNECTION_ATTEMPT_FAILED:
    ff::write(pan::warning, "Connection attempt failed.");
    break;

  case ID_NO_FREE_INCOMING_CONNECTIONS:
    ff::write(pan::warning, "No free incoming connections.");
    break;

  case ID_CONNECTION_LOST:
    ff::write(pan::warning, "Connection was lost.");
    break;

  case ID_CONNECTION_BANNED:
    ff::write(pan::warning, "Banned from server.");
    break;

  case ID_INVALID_PASSWORD:
    ff::write(pan::warning, "Invalid password.");
    break;

  case ID_INCOMPATIBLE_PROTOCOL_VERSION:
    ff::write(pan::warning, "Incompatible protocol version.");
    break;

  default:
    return false;
  }
  
  ff::fmt(pan::debug, "({0} | Proxy) Packet<{1}>", addr.ToString(false),
    int_to_hex((int)opCode));

  return true;
}



//////////////////////////////////////////////////////////////////////////////
//
// Login server handling methods
//
//////////////////////////////////////////////////////////////////////////////

RakNet::PluginReceiveResult
CLoginServer::OnReceive(RakNet::Packet* packet)
{
  if (!packet)
    return RakNet::RR_CONTINUE_PROCESSING;

  uchar* pDatas = packet->data;
  int len = packet->length;

  if (len == 0)
    return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
  
  RakNet::BitStream inStr(pDatas, len, false);
  uchar opCode;

  if (!inStr.Read(opCode))
    return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;

  if (!handleDefaultPacket(opCode, packet->systemAddress) && opCode >= ID_USER_PACKET_ENUM)
  {
    RakNet::SystemAddress clientAddr;
    
    if (!inStr.Read(clientAddr))
      return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
    
    ff::fmt(pan::debug, "({0}) Packet<{1}>", clientAddr.ToString(false),
      int_to_hex((int)opCode));

    switch (opCode)
    {
    case Opcodes::OP_LOGIN:
    {
      RakNet::RakString sLogin;
      RakNet::RakString sPwd;
      
      RakNet::StringCompressor::Instance()->DecodeString(&sLogin, 15, &inStr);
      RakNet::StringCompressor::Instance()->DecodeString(&sPwd, 16, &inStr);

      bool bLoggedIn = false;

      if (sLogin == "test" || sPwd == "test")
      {
        bLoggedIn = true;
        ff::fmt(pan::notice, "({0}) Player successfully logged in.", clientAddr.ToString(false));
      }

      RakNet::BitStream outStr;

      outStr.Write(opCode);
      outStr.Write(clientAddr);
      outStr.Write(bLoggedIn);

      m_pListenInterface->Send(&outStr, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, packet->systemAddress, false);

      break;
    }

    default:
      return RakNet::RR_CONTINUE_PROCESSING;
    }
  }
  
  return RakNet::RR_STOP_PROCESSING_AND_DEALLOCATE;
}

void
CLoginServer::OnNewConnection(const RakNet::SystemAddress&          systemAddress,
                              RakNet::RakNetGUID                    rakNetGUID,
                              bool                                  isIncoming)
{
  ff::fmt(pan::notice, "({0}) New connection.", systemAddress.ToString(false));

}

void
CLoginServer::OnClosedConnection(const RakNet::SystemAddress&       systemAddress,
                                 RakNet::RakNetGUID                 rakNetGUID,
                                 RakNet::PI2_LostConnectionReason   lostConnectionReason)
{
  ff::fmt(pan::notice, "({0}) Closed connection.", systemAddress.ToString(false));
}