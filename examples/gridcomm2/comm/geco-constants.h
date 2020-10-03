#ifndef GECO_CONSTANTS_H
#define GECO_CONSTANTS_H

namespace ns3 {

namespace geco
{
  // GECO protection application FSM states
   enum State {
    STATE_UNINITIALIZED = 4, // Not initialized, will not participate in protection.
                         // NOTE: This state is not part of the GECO FSM.

    STATE_IDLE = 1,   // Fault detected, action taken, nothing left to do.
    STATE_NORMAL = 0, // Monitoring for faults.
    STATE_WAIT = 2,   // Waiting for reports from peers.
    STATE_OOS =3     // Block decision made, inconsistent info. Nothing to do.
  };

  /* enum MessageType {
    MESSAGETYPE_UNASSIGNED, // Message type not assigned
    MESSAGETYPE_REQUEST,    // Request for status from peer
    MESSAGETYPE_RESPONSE    // Response to request for status from peer
  };*/
  enum MessageType {
    SETLOCAL = 0,
    SETREMOTE = 1,
    BACKUPRELAY = 2,
    REQUEST = 3,
    RESPONSE = 4
  };

} // namespace geco

} // namespace ns3

#endif /* GECO_CONSTANTS_H */
