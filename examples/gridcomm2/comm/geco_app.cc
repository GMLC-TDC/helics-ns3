#include "ns3/log.h"
#include "ns3/ptr.h"
#include "ns3/ipv4.h"
#include "ns3/names.h"
#include "ns3/packet.h"
#include "ns3/uinteger.h"
#include "ns3/boolean.h"
#include "ns3/socket.h"
#include "ns3/ipv4-address.h"
#include "ns3/socket-factory.h"
#include "ns3/simulator.h"
#include "geco_app.h"

#include <string>
#include <cstdlib>

NS_LOG_COMPONENT_DEFINE ("GecoApp");

namespace ns3
{
  NS_OBJECT_ENSURE_REGISTERED (GecoApp);

  TypeId GecoApp::GetTypeId()
  {
    static TypeId tid = TypeId ("ns3::GecoApp")
    .SetParent<Application> ()
    .AddConstructor<GecoApp> ()
    .AddAttribute ("Verbose",
                 "Enable verbose output",
                 BooleanValue (false),
                 MakeBooleanAccessor (&GecoApp::m_verbose),
                 MakeBooleanChecker ())
    ;
    return tid;
  }

  GecoApp::GecoApp () :
  m_relay (), m_state (geco::STATE_UNINITIALIZED), m_Socket (0), m_waitTimeoutEvent (),
  m_peerStateMap (), m_numPeersUnreported (0), m_verbose (false)

{
  NS_LOG_FUNCTION (this);
}

GecoApp::~GecoApp ()
{
  NS_LOG_FUNCTION (this);
}

void
GecoApp::DoDispose ()
{
  NS_LOG_FUNCTION (this);
  Application::DoDispose ();
}

void GecoApp::StartApplication()
{
  NS_LOG_FUNCTION (this);
  if (m_socket == 0)
  {
    TypeId id = TypeId::LookupByName ("ns3::UdpSocketFactory");
    m_socket = Socket::CreateSocket (GetNode (), id);
    Ptr<Ipv4> ipv4 = GetNode ()->GetObject<Ipv4> ();
    InetSocketAddress local =
      InetSocketAddress (Ipv4Address::GetAny (), SERVER_PORT);
    m_Socket->Bind (local);
  }
  m_serverSocket->SetRecvCallback (
  MakeCallback (&GecoApp::HandleRead, this));

    m_relay = NodeNameToRelay (GetNode ());
    if (m_verbose)
      {
        NS_LOG_INFO (
          GetLogMessagePrefix ()
          << "Initializing my relay to: link "
          << m_relay.line.first << " -- " << m_relay.line.second
          << ", at bus " << m_relay.bus);
      }

  SetState (geco::STATE_NORMAL);

}
void GecoApp::StopApplication()
{
    NS_LOG_FUNCTION (this);
    if (m_serverSocket != 0)
    {
      m_serverSocket->SetRecvCallback (
        MakeNullCallback<void, Ptr<Socket> > ());
    }
}
void GecoApp::SetState (geco::State s)
{
  if (m_verbose)
  {
    NS_LOG_INFO (
      GetLogMessagePrefix ()
      << "STATE CHANGE: "
      << GetStateName (m_state) << " -> " << GetStateName (s));
  }
  m_state = s;
}

void
GecoApp::HandleRead (Ptr<Socket> socket)
{
  NS_LOG_FUNCTION (this << socket);
  Ptr<Packet> p;
  Address from;
  while ((p = socket->RecvFrom (from)))
    {
      if (p->GetSize () > 0)
        {
          GecoUdpHeader h;
          p->RemoveHeader (h);
          InetSocketAddress inetAddr =
            InetSocketAddress::ConvertFrom (from);
          if (m_verbose)
            {
              NS_LOG_INFO (
                GetLogMessagePrefix ()
                << "RX state message, from: " << inetAddr.GetIpv4 ()
                << ", at: " << Simulator::Now ()
                << ", state: " << GetStateName (h.GetState ())
                << ", type: " << GetMessageTypeName (h.GetMessageType ()));
            }
          Address addr = inetAddr.GetIpv4 ();
          ProcessMessage (h, addr);
        }
      else
        {
          if (m_verbose)
            {
              NS_LOG_INFO ("RX empty packet");
            }
        }
    }
}


void GecoApp:ProcessMessage (GecoUdpHeader& header, Address& source){
  switch (m_state)
    {
      case geco::STATE_UNINITIALIZED:
        NS_ASSERT (false);
        break;

      case geco::STATE_NORMAL:
        ProcessNormal (header, source);
        break;

      case geco::STATE_IDLE:
        ProcessIdle (header, source);
        break;

      case geco::STATE_WAIT:
        ProcessWait (header, source);
        break;

      case geco::STATE_OOS:
        ProcessOos (header, source);
        break;
    }
}


void GecoApp:ProcessNormal (GecoUdpHeader& header, Address& source)

{
  if (header.IsRequest ())
      {
        GecoUdpHeader h;
        h.SetMessageType (geco::RESPONSE);
        h.SetState (m_state); // Report no fault (NORMAL)
        Send (h, source);
      }
}

void GecoApp:ProcessIdle (GecoUdpHeader& header, Address& source)
{
  if (header.IsRequest ())
    {
      GecoUdpHeader h;
      h.SetMessageType (geco::RESPONSE);
      h.SetState (m_state); // Report trip (IDLE)
      Send (h, source);
    }
}

void GecoApp::ProcessWait (GecoUdpHeader& header, Address& source)
{
  if (header.IsRequest ())
    {
      // TODO: Model processing time before send
      GecoUdpHeader h;
      h.SetMessageType (geco::RESPONSE);
      h.SetState (m_state); // Report remote fault (WAIT)
      Send (h, source);
    }
  else if (header.IsResponse ())
    {
      // Attempt to make decision:

      // If at least one other relay sees a fault, then trip
      if (header.IsIdle () || header.IsWaiting ()) // local || remote fault
        {
          if (m_verbose)
            {
              NS_LOG_INFO (GetLogMessagePrefix ()
                << "Tripping because at least one other relay sees a fault.");
            }
          SetState (geco::STATE_IDLE);
          Simulator::Cancel (m_waitTimeoutEvent);
        }
      else
        {
          // Store response
          uint32_t sourceAddr = Ipv4Address::ConvertFrom (source).Get ();
            //InetSocketAddress::ConvertFrom (source).GetIpv4 ().Get ();
          geco::State currentPeerState = m_peerStateMap[sourceAddr];
          if (currentPeerState == geco::STATE_UNINITIALIZED)
            {
              m_peerStateMap[sourceAddr] = header.GetState ();
              --m_numPeersUnreported;
              if (m_verbose)
                {
                  NS_LOG_INFO (GetLogMessagePrefix ()
                    << "Peer relay " << Ipv4Address (sourceAddr)
                    << " reported in. Waiting for "
                    << m_numPeersUnreported << " relays to report in.");
                }
            }

          // If all relays reported in, then no fault was seen
          // by peers, and this relay needs servicing.
          if (m_numPeersUnreported <= 0)
            {
              if (m_verbose)
                {
                  NS_LOG_INFO (GetLogMessagePrefix ()
                    << "Going OOS because no other relays see a fault.");
                }
              SetState (geco::STATE_OOS);
              Simulator::Cancel (m_waitTimeoutEvent);
            }
        }
    }
}

void GecoApp::ProcessOos (GecoUdpHeader& header, Address& source)
{
  if (header.IsRequest ())
      {
        // TODO: Model processing time before send,
        // i.e. use Simulator::Schedule() with non-zero future time
        GecoUdpHeader h;
        h.SetMessageType (geco::RESPONSE);
        h.SetState (m_state); // Report OOS
        Send (h, source);
      }
}

void GecoApp::SetLocalFault ()
{
  if (m_verbose)
    {
      NS_LOG_INFO (GetLogMessagePrefix () << "Local fault sensed");
    }
  // state == NORMAL && local fault sensed: state -> IDLE (trip)
  if (m_state == geco::STATE_NORMAL)
    {
      SetState (geco::STATE_IDLE);
    }
}

void GecoApp::SetRemoteFault()
{
  if (m_verbose)
    {
      NS_LOG_INFO (GetLogMessagePrefix () << "Remote fault sensed");
    }
  // state == NORMAL: query peers; start timer; state -> WAIT
  if (m_state == geco::STATE_NORMAL)
  {
    SetState (geco::STATE_WAIT);
  }
  //TODO finsih writing this
}

void GecoApp::ClearRemoteFault()
{
  if (m_state == geco::STATE_WAIT)
  {
    m_peerStateMap.clear ();
    SetState (geco::STATE_NORMAL);
  }
}


GecoApp::RelayVector GecoApp::GetResponsibleRelays (Relay &relay,std::vector<std::pair<int,int>> possibleFaultLines);
{
  //TODO complete
}

void GecoApp::WaitTimeout ()
{
  // state == WAIT && timeout: state -> IDLE
  if (m_state == geco::STATE_WAIT)
    {
      if (m_verbose)
        {
          NS_LOG_INFO (GetLogMessagePrefix ()
            << "Tripping because timed-out waiting for relay responses.");
        }
      m_peerStateMap.clear ();
      SetState (geco::STATE_IDLE);
    }
}

void GecoApp::Send()
{
  Ptr<Packet> p = Create<Packet> (0);
  p->AddHeader (header);
  ///*
  TypeId tid = TypeId::LookupByName ("ns3::UdpSocketFactory");
  m_sendSocket = Socket::CreateSocket (GetNode (), tid);
  InetSocketAddress a (Ipv4Address::ConvertFrom (dest), SERVER_PORT);
  m_sendSocket->Bind ();
  if (m_sendSocket->Connect (a) != 0)
    {
      NS_LOG_WARN (GetLogMessagePrefix() << "Connect failed");
    }

  int numBytes = m_sendSocket->Send (p);
  if (numBytes > 0)
    {
      if (m_verbose)
        {
          NS_LOG_INFO (
            GetLogMessagePrefix ()
            << "TX state message (" << numBytes << " bytes), to: "
            << a.GetIpv4 ()
            << ", at: " << Simulator::Now ()
            << ", state: " << GetStateName (header.GetState ())
            << ", type: " << GetMessageTypeName (header.GetMessageType ()));
        }
    }
  else
    {
      NS_LOG_WARN ("TX failed");
    }
}


void GecoApp::SetState (geco::State s)
{
  if (m_verbose)
  {
    NS_LOG_INFO (
      GetLogMessagePrefix ()
      << "STATE CHANGE: "
      << GetStateName (m_state) << " -> " << GetStateName (s));
  }
  m_state = s;
}

std::string GecoApp::GetLogMessagePrefix (void)
{
  std::stringstream ss;
  ss << Simulator::Now ()
     << " [" << Names::FindName (GetNode ()) << "] - ";
  return ss.str ();
}

geco::State GecoApp::GetState (void) const
{
  return m_state;
}

std::string GecoApp::GetStateName (geco::State s)
{
  std::string name ("Unknown");
  switch (s)
    {
      case geco::STATE_UNINITIALIZED:
        name = "Uninitialized";
        break;
      case geco::STATE_NORMAL:
        name = "Normal";
        break;
      case geco::STATE_IDLE:
        name = "Idle";
        break;
      case geco::STATE_WAIT:
        name = "Waiting";
        break;
      case geco::STATE_OOS:
        name = "OOS";
        break;
    }
  return name;
}

std::string
GecoAdhocApplication::GetMessageTypeName (geco::MessageType t)
{
  std::string name ("Unknown");
  switch (t)
    {
      case geco::SETLOCAL:
        name = "SetLocal";
        break;
      case geco::REQUEST:
        name = "Request";
        break;
      case geco::RESPONSE:
        name = "Response";
        break;
      case geco::SETREMOTE:
        name = "SetRemote";
        break;
      case geco::BACKUPRELAY:
        name = "Back Up Relay";
        break;

    }
  return name;
}

/* TODO electric topology update (processElectricTopo needs to be redone)
* Currently pulls gridcomm::graph object from gridcomm-graph.h
* which uses the boost library and example graphviz file
* possible issues with boos library
*/
void SetTopology()
{

}


} //end namespace ns3
