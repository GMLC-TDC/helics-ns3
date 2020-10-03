#include "ns3/log.h"
#include "ns3/header.h"
#include "ns3/type-id.h"
#include "ns3/buffer.h"

#include "geco-udp-header.h"
#include "geco-constants.h"

#include <iostream>

NS_LOG_COMPONENT_DEFINE ("GecoUdpHeader")
  ;

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (GecoUdpHeader)
  ;

GecoUdpHeader::GecoUdpHeader () :
  m_state (geco::STATE_UNINITIALIZED), m_type (geco::MESSAGETYPE_UNASSIGNED)
{
  NS_LOG_FUNCTION (this);
}

GecoUdpHeader::~GecoUdpHeader ()
{
}

void
GecoUdpHeader::SetState (geco::State s)
{
  m_state = s;
}

void
GecoUdpHeader::SetMessageType (geco::MessageType t)
{
  m_type = t;
}

geco::State
GecoUdpHeader::GetState (void) const
{
  return m_state;
}

bool
GecoUdpHeader::IsNormal (void) const
{
  return m_state == geco::STATE_NORMAL;
}

bool
GecoUdpHeader::IsIdle (void) const
{
  return m_state == geco::STATE_IDLE;
}

bool
GecoUdpHeader::IsWaiting (void) const
{
  return m_state == geco::STATE_WAIT;
}

bool
GecoUdpHeader::IsOos (void) const
{
  return m_state == geco::STATE_OOS;
}

geco::MessageType
GecoUdpHeader::GetMessageType (void) const
{
  return m_type;
}

bool
GecoUdpHeader::IsUnassigned (void) const
{
  return m_type == geco::MESSAGETYPE_UNASSIGNED;
}

bool
GecoUdpHeader::IsRequest (void) const
{
  return m_type == geco::MESSAGETYPE_REQUEST;
}

bool
GecoUdpHeader::IsResponse (void) const
{
  return m_type == geco::MESSAGETYPE_RESPONSE;
}

TypeId
GecoUdpHeader::GetTypeId (void)
{
  static TypeId id = TypeId ("ns3::GecoUdpHeader")
    .SetParent<Header> ()
    .AddConstructor<GecoUdpHeader> ()
  ;
  return id;
}

TypeId
GecoUdpHeader::GetInstanceTypeId (void) const
{
  return GetTypeId ();
}

void
GecoUdpHeader::Print (std::ostream& os) const
{
  NS_LOG_FUNCTION (this << &os);
  os << "(state = ";
  if (IsNormal ())       os << "Normal";
  else if (IsIdle ())    os << "Idle";
  else if (IsWaiting ()) os << "Waiting";
  else if (IsOos ())     os << "OOS";
  else                   os << "<Unknown>";

  os << ", type = ";
  if (IsUnassigned ())    os << "Unassigned";
  else if (IsRequest ())  os << "Request";
  else if (IsResponse ()) os << "Response";
  else                    os << "<Unknown>";
  os << ")";
}

uint32_t
GecoUdpHeader::GetSerializedSize (void) const
{
  NS_LOG_FUNCTION (this);
  return 2;
}

void
GecoUdpHeader::Serialize (Buffer::Iterator it) const
{
  NS_LOG_FUNCTION (this << &it);
  it.WriteU8 (static_cast<uint8_t> (m_state));
  it.WriteU8 (static_cast<uint8_t> (m_type));
}

uint32_t
GecoUdpHeader::Deserialize (Buffer::Iterator it)
{
  NS_LOG_FUNCTION (this << &it);
  m_state = static_cast<geco::State> (it.ReadU8());
  m_type = static_cast<geco::MessageType> (it.ReadU8());
  return GetSerializedSize ();
}

} // namespace ns3
