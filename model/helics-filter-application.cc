#include <iostream>
#include <fstream>
#include <vector>

#include "ns3/log.h"
#include "ns3/string.h"

#include "ns3/helics.h"
#include "ns3/helics-application.h"
#include "ns3/helics-filter-application.h"
#include "helics/helics.hpp"

#include <algorithm>
#include <sstream>
#include <string>

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("HelicsFilterApplication");

NS_OBJECT_ENSURE_REGISTERED (HelicsFilterApplication);

TypeId
HelicsFilterApplication::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::HelicsFilterApplication")
    .SetParent<HelicsApplication> ()
    .AddConstructor<HelicsFilterApplication> ()
  ;
  return tid;
}

HelicsFilterApplication::HelicsFilterApplication ()
{
  NS_LOG_FUNCTION (this);
}

HelicsFilterApplication::~HelicsFilterApplication()
{
  NS_LOG_FUNCTION (this);
}

void
HelicsFilterApplication::DoDispose (void)
{
  NS_LOG_FUNCTION (this);

  HelicsApplication::DoDispose ();
}

void 
HelicsFilterApplication::StartApplication (void)
{
  NS_LOG_FUNCTION (this);

  HelicsApplication::StartApplication();
}

void 
HelicsFilterApplication::StopApplication ()
{
  NS_LOG_FUNCTION (this);

  HelicsApplication::StopApplication();
}

void
HelicsFilterApplication::DoFilter (std::unique_ptr<helics::Message> message)
{
  NS_LOG_FUNCTION (this);
  Send (message->dest, std::move (message));
}

void
HelicsFilterApplication::DoEndpoint (helics::endpoint_id_t id, helics::Time time, std::unique_ptr<helics::Message> message)
{
  NS_LOG_FUNCTION (this);
  NS_FATAL_ERROR ("HelicsFilterApplication should not receive endpoint events");
}

void
HelicsFilterApplication::DoRead (std::unique_ptr<helics::Message> message)
{
  NS_LOG_FUNCTION (this);
  helics_federate->sendMessage (helics_endpoint, std::move (message));
}

} // Namespace ns3
