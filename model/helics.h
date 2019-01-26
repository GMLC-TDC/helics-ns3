/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#ifndef HELICS_H
#define HELICS_H

#include <ostream>
#include <memory>

#include "helics/helics.hpp"

namespace ns3 {

extern std::shared_ptr<helics::MessageFederate> helics_federate;
extern helics::Endpoint helics_endpoint;

std::ostream& operator << (std::ostream& stream, const helics::Message &message);

std::ostream& operator << (std::ostream& stream, std::unique_ptr<helics::Message> message);

}

#endif /* HELICS_H */

