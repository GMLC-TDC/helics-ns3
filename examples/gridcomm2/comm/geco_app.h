#ifndef GECO_APP_H
#define GECO_APP_H

#include "ns3/type-id.h"
#include "ns3/application.h"
#include "ns3/ptr.h"
#include "ns3/socket.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include "ns3/event-id.h"
#include "geco-constants.h"
#include "geco-udp-header.h"


#include <string>
#include <vector>
#include <map>


namespace ns3
{
class GecoApp : public Application
{
public:
  static const uint16_t SERVER_PORT = 1025;
  static TypeId GetTypeId();
  GecoApp();
  virtual ~GecoApp();
  geco::State GetState();
  void SetTopology ();
  void SetLocalFault();
  void SetRemoteFault();
  void ClearRemoteFault();
protected:
  virtual void DoDispose();
private:
  virtual void StartApplication();
  virtual void StopApplication();
  void SetState(geco::State state);
  void HandleRead(Ptr<Socket> socket);
  void Send (GecoUdpHeader& header, Address& dest);
  void ProcessMessage (GecoUdpHeader& header, Address& source);
  void ProcessNormal (GecoUdpHeader& header, Address& source);
  void ProcessIdle (GecoUdpHeader& header, Address& source);
  void ProcessWait (GecoUdpHeader& header, Address& source);
  void ProcessOos (GecoUdpHeader& header, Address& source);

  void WaitTimeout();
  struct Relay
  {
    std::pair<int,int> line;
    int bus;
  };
  std::vector<Relay> relayVector;
  std::vector<std::pair<int,int>> possibleFaultLines(Relay &initRelay);
  std::vector<Relay> GetResponsibleRelays (Relay &relay,std::vector<std::pair<int,int>> possibleFaultLines);
  Relay NodeNameToRelay(Ptr<Node> node);
  Ptr<Node> RelayToNode (Relay &relay);
  std::string GetLogMessagePrefix();
  static std::string GetStateName(geco::State s);
  static std::string GetMessageTypeName(geco::MessageType t);
  Relay m_relay;
  geco::State m_state;
  //Ptr<Socket> m_serverSockets;
  Ptr<Socket> m_Socket;
  EventId m_waitTimeoutEvent;
  typedef std::map<uint32_t, geco::State> StateMap;
  StateMap m_peetStateMap;
  uint32_t m_numPeersUnreported;
  bool m_verbose;
};
}

#endif
