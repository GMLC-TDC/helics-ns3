/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */


#include "ns3/application-container.h"
#include "ns3/node-container.h"
#include "ns3/ipv4.h"
#include <memory>

#include "ns3/helics.h"
#include "ns3/helics-application.h"
#include "ns3/helics-filter-application.h"
#include "ns3/helics-static-sink-application.h"
#include "ns3/helics-static-source-application.h"
#include "ns3/helics-helper.h"

#include "helics/core/core-types.hpp"

namespace ns3 {

HelicsHelper::HelicsHelper()
: broker("")
, name("ns3")
, core("zmq")
, stop(1.0)
, timedelta(1.0)
, coreinit("--loglevel=2")
{
    m_factory_filter.SetTypeId (HelicsFilterApplication::GetTypeId ());
    m_factory_sink.SetTypeId (HelicsStaticSinkApplication::GetTypeId ());
    m_factory_source.SetTypeId (HelicsStaticSourceApplication::GetTypeId ());
}

void
HelicsHelper::SetupFederate(void)
{
  helics::FederateInfo fi (name);
  fi.coreType = helics::coreTypeFromString (core);
  //fi.coreInitString = "--broker=" + "brokid" + " --broker_address=" + "addr" + " --federates 1";
  //fi.coreInitString = "--federates 1";
  fi.timeDelta = timedelta;
  if (!coreinit.empty()) {
    fi.coreInitString = coreinit;
  }
  helics_federate = std::make_shared<helics::MessageFederate> (fi);
  helics_endpoint = helics_federate->registerEndpoint ("fout");

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::HelicsSimulatorImpl"));
}

void
HelicsHelper::SetupCommandLine(CommandLine &cmd)
{
  cmd.AddValue ("broker", "address to connect the broker to", broker);
  cmd.AddValue ("name", "name of the ns3 federate", name);
  cmd.AddValue ("core", "name of the core to connect to", core);
  cmd.AddValue ("stop", "the time to stop the player", stop);
  cmd.AddValue ("timedelta", "the time delta of the federate", timedelta);
  cmd.AddValue ("coreinit", "the core initializion string", coreinit);
}

ApplicationContainer
HelicsHelper::InstallFilter (Ptr<Node> node, const std::string &name) const
{
    ApplicationContainer apps;
    Ptr<HelicsFilterApplication> app = m_factory_filter.Create<HelicsFilterApplication> ();
    if (!app) {
      NS_FATAL_ERROR ("Failed to create HelicsFilterApplication");
    }
    app->SetFilterName (name);
    Ptr<Ipv4> net = node->GetObject<Ipv4>();
    Ipv4InterfaceAddress interface_address = net->GetAddress(1,0);
    Ipv4Address address = interface_address.GetLocal();
    app->SetLocal(address, 1234);
    node->AddApplication (app);
    apps.Add (app);
    return apps;
}

ApplicationContainer
HelicsHelper::InstallStaticSink (Ptr<Node> node, const std::string &name, const std::string &destination) const
{
    ApplicationContainer apps;
    Ptr<HelicsStaticSinkApplication> app = m_factory_sink.Create<HelicsStaticSinkApplication> ();
    if (!app) {
      NS_FATAL_ERROR ("Failed to create HelicsStaticSinkApplication");
    }
    app->SetEndpointName (name);
    app->SetDestination (destination);
    Ptr<Ipv4> net = node->GetObject<Ipv4>();
    Ipv4InterfaceAddress interface_address = net->GetAddress(1,0);
    Ipv4Address address = interface_address.GetLocal();
    app->SetLocal(address, 1234);
    node->AddApplication (app);
    apps.Add (app);
    return apps;
}

ApplicationContainer
HelicsHelper::InstallStaticSource (Ptr<Node> node, const std::string &name, const std::string &destination) const
{
    ApplicationContainer apps;
    Ptr<HelicsStaticSourceApplication> app = m_factory_source.Create<HelicsStaticSourceApplication> ();
    if (!app) {
      NS_FATAL_ERROR ("Failed to create HelicsStaticSinkApplication");
    }
    app->SetEndpointName (name);
    app->SetDestination (destination);
    Ptr<Ipv4> net = node->GetObject<Ipv4>();
    Ipv4InterfaceAddress interface_address = net->GetAddress(1,0);
    Ipv4Address address = interface_address.GetLocal();
    app->SetLocal(address, 1234);
    node->AddApplication (app);
    apps.Add (app);
    return apps;
}

}

