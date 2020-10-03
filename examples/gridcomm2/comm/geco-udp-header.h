#ifndef GECO_UDP_HEADER_H
#define GECO_UDP_HEADER_H

#include "ns3/header.h"
#include "ns3/type-id.h"
#include "ns3/buffer.h"
#include "geco-constants.h"

#include <iostream>

namespace ns3 {

class GecoUdpHeader : public Header
{
public:
  static TypeId GetTypeId (void);

  GecoUdpHeader ();
  virtual ~GecoUdpHeader ();
  void SetState (geco::State s);
  void SetMessageType (geco::MessageType t);

  geco::State GetState (void) const;
  bool IsNormal (void) const;
  bool IsIdle (void) const;
  bool IsWaiting (void) const;
  bool IsOos (void) const;

  geco::MessageType GetMessageType (void) const;
  bool IsUnassigned (void) const;
  bool IsRequest (void) const;
  bool IsResponse (void) const;

  virtual TypeId GetInstanceTypeId (void) const;
  virtual void Print (std::ostream& os) const;
  virtual uint32_t GetSerializedSize (void) const;
  virtual void Serialize (Buffer::Iterator it) const;
  virtual uint32_t Deserialize (Buffer::Iterator it);

private:
  geco::State m_state;
  geco::MessageType m_type;
};

} // namespace ns3

#endif /* GECO_UDP_HEADER_H */
