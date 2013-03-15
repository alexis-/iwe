// ConsoleTestClient.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "CLogger.h"

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>

#include <RakString.h>

#include <RakPeerInterface.h>
#include <SocketLayer.h>
#include <PluginInterface2.h>
#include <RakSleep.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>
#include <PacketPriority.h>


class CClient
{
public:
  CClient()
    : m_bConnected(false),
      m_pUpdateThread(NULL),
      m_bQuitThread(false)
  {
    m_pSrvItf = RakNet::RakPeerInterface::GetInstance();
  }

  ~CClient()
  {
    
    delete m_pUpdateThread;
    m_pUpdateThread = NULL;
  }

  // Initialize raknet interface
  bool init(void)
  {
    RakNet::SocketDescriptor socketDescriptor(0, NULL);

    bool bRet = (m_pSrvItf->Startup(1, &socketDescriptor, 1) == RakNet::RAKNET_STARTED);

    if (bRet)
      m_pUpdateThread = new boost::thread(FastDelegate0<>(this, &CClient::updateThread));

    return bRet;
  }

  // Connect to a remote server
  bool connect(const std::string& sSrvIP, ushort uSrvPort)
  {
    if (m_bConnected)
      return true;

    m_srvAddress.FromStringExplicitPort(sSrvIP.c_str(), uSrvPort);
    bool bRet = (m_pSrvItf->Connect(sSrvIP.c_str(), uSrvPort, 0, 0) == RakNet::CONNECTION_ATTEMPT_STARTED);
    m_pSrvItf->SetOccasionalPing(true);

    return bRet;
  }

  // Disconnect from remove server
  void disconnect(void)
  {
    if (!m_bConnected)
      return;

    m_bQuitThread = true;
    m_pUpdateThread->join();

    m_pSrvItf->CloseConnection(m_pSrvItf->GetSystemAddressFromIndex(0), true, 0);

    m_bConnected = false;
  }

  bool handleDefaultPacket(uchar opCode, uchar* pDatas)
  {
    switch (opCode)
    {
    case ID_CONNECTION_REQUEST_ACCEPTED:
      log_NOTICE("Notice: ID_CONNECTION_REQUEST_ACCEPTED");
      m_bConnected = true;
      break;

    case ID_DISCONNECTION_NOTIFICATION:
      log_NOTICE("Notice: ID_DISCONNECTION_NOTIFICATION");
      m_bConnected = false;
      break;

      // print out errors
    case ID_CONNECTION_ATTEMPT_FAILED:
      log_WARNING("Error: ID_CONNECTION_ATTEMPT_FAILED");
      m_bConnected = false;
      break;

    case ID_NO_FREE_INCOMING_CONNECTIONS:
      log_WARNING("Error: ID_NO_FREE_INCOMING_CONNECTIONS");
      m_bConnected = false;
      break;

    case ID_CONNECTION_LOST:
      log_WARNING("Error: ID_CONNECTION_LOST");
      m_bConnected = false;
      break;

    case ID_ALREADY_CONNECTED:
      log_WARNING("Error: ID_ALREADY_CONNECTED");
      break;

    case ID_CONNECTION_BANNED:
      log_WARNING("Error: ID_CONNECTION_BANNED");
      break;

    case ID_INVALID_PASSWORD:
      log_WARNING("Error: ID_INVALID_PASSWORD");
      break;

    case ID_INCOMPATIBLE_PROTOCOL_VERSION:
      log_WARNING("Error: ID_INCOMPATIBLE_PROTOCOL_VERSION");
      break;

    default:
      return false;
    }

    return true;
  }

  // Fetch incoming packets
  void updateThread()
  {
    while (!m_bQuitThread)
    {
      RakNet::Packet* p = m_pSrvItf->Receive();

      while (p)
      {
        uchar* pDatas = p->data;
        uchar opCode = pDatas[0];

        // Handle packet
        if (!handleDefaultPacket(opCode, pDatas))
        {
          //handleCustomPacket(opCode, pDatas, p);
          ff::write(pan::notice, "Packet <", int_to_hex((int)opCode), ">");

          switch (opCode)
          {
            case Opcodes::OP_LOGIN:
            {
              RakNet::BitStream inStr(pDatas, p->length, false);
              
              bool bConnected = false;

              inStr.IgnoreBytes(1);
              inStr.Read(bConnected);

              if (bConnected)
                log_NOTICE("Successfully logged in.");
              else
                log_NOTICE("Failed to log in.");
            }
          }
        }

        // Free packet & get next one if available
        m_pSrvItf->DeallocatePacket(p);
        p = m_pSrvItf->Receive();
      }

      RakSleep(50);
    }

    m_bQuitThread = false;
  }

  bool sendLoginPacket(const std::string& sLogin, const std::string& sPwd)
  {
    if (!m_bConnected)
      return false;
    
    uchar opCode = Opcodes::OP_LOGIN;

    RakNet::BitStream strOut;

    strOut.Write(opCode);
    RakNet::StringCompressor::Instance()->EncodeString(sLogin.c_str(), 15, &strOut);
    RakNet::StringCompressor::Instance()->EncodeString(sPwd.c_str(), 16, &strOut);
    
    m_pSrvItf->Send(&strOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, m_srvAddress, false);

    return true;
  }

  bool sendTestPacket()
  {
    if (!m_bConnected)
      return false;

    uchar opCode = ID_USER_PACKET_ENUM;

    RakNet::BitStream strOut;

    strOut.Write((RakNet::MessageID)opCode);
    strOut.Write(m_pSrvItf->GetMyBoundAddress());

    m_pSrvItf->Send(&strOut, HIGH_PRIORITY, RELIABLE_SEQUENCED, 0, m_srvAddress, false);

    return true;
  }

  bool                          m_bConnected;
  RakNet::RakPeerInterface*     m_pSrvItf;
  RakNet::SystemAddress         m_srvAddress;

  boost::thread*                m_pUpdateThread;
  bool                          m_bQuitThread;
};

static CClient*         pClient = NULL;

bool handleInput(const std::string &sInput)
{
  if (sInput == "exit"  || sInput == "e" ||
      sInput == "q"     || sInput == "quit")
      return false;
  
  std::vector<std::string> args;
  boost::split(args, sInput, boost::is_any_of(" "));

  try
  {
    if ((args[0] == "connect" || args[0] == "c") && args.size() == 2)
    {
      std::vector<std::string> destArgs;
      boost::split(destArgs, args[1], boost::is_any_of(":"));

      if (destArgs.size() == 2)
      {
        std::string sIP = destArgs[0];
        ushort uPort = boost::lexical_cast<ushort>(destArgs[1]);

        if (!pClient->connect(sIP, uPort))
          log_WARNING("Failed to connect.");
      }
      else
        log_WARNING("Wrong address syntax. Syntax is : XXX.XXX.XXX.XXX:YYYYY.");
    }

    else if (args[0] == "disconnect" || args[0] == "d")
      pClient->disconnect();

    else if (args[0] == "stest")
      pClient->sendTestPacket();

    else if (args[0] == "login")
      if (args.size() == 3)
        pClient->sendLoginPacket(args[1], args[2]);
  }
  catch (std::exception ex)
  {
    ff::write(pan::warning, "Wrong command format (", ex.what(), ")");
  }

  return true;
}

int main(int argc, char** argv)
{
  CLogger::init();

  CLogger::setLogLevel(7);
  CLogger::setLogFile("console-client.log");
  
  pClient = new CClient();
  pClient->init();

  std::string sInput;

  do
  {
    sInput.clear();
    std::getline(std::cin, sInput);
  } while (handleInput(sInput));

  return 0;
}

