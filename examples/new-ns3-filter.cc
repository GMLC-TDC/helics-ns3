/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/helics-helper.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1   n2   n3   n4
//    point-to-point  |    |    |    |
//                    ================
//                      LAN 10.1.2.0


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("HelicsExample");

/*
 * The main() loop below represents the ns-3 model. The helics ns-3
 * integration will filter messages sent by MessageFederate instances by
 * creating HelicsApplication instances at Nodes. The name given to the
 * HelicsApplication should match a registered endpoint.
 */

int 
main (int argc, char *argv[])
{
  std::string config = argv[0];

  std::cout << config << std::endl;
  HelicsHelper helicsHelper;

  helicsHelper.SetupApplicationFederateWithConfig(config);


  // TODO: query federate for info
  std::cout << helicsHelper.helics_federate.getName() << std::endl;

//  InternetStackHelper stack;
//  stack.Install (p2pNodes.Get (0));
//  stack.Install (csmaNodes);
//
//  Ipv4AddressHelper address;
//  address.SetBase ("10.1.1.0", "255.255.255.0");
//  Ipv4InterfaceContainer p2pInterfaces;
//  p2pInterfaces = address.Assign (p2pDevices);
//
//  address.SetBase ("10.1.2.0", "255.255.255.0");
//  Ipv4InterfaceContainer csmaInterfaces;
//  csmaInterfaces = address.Assign (csmaDevices);
//
//  ApplicationContainer apps1 = helicsHelper.InstallFilter (
//          csmaNodes.Get (nCsma), endpoint1);
//  apps1.Start (Seconds (0.0));
//  apps1.Stop (Seconds (10.0));
//
//  ApplicationContainer apps2 = helicsHelper.InstallFilter (
//          p2pNodes.Get (0), endpoint2);
//  apps2.Start (Seconds (0.0));
//  apps2.Stop (Seconds (10.0));
//
//  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();
//
//  pointToPoint.EnablePcapAll ("second");
//  csma.EnablePcap ("second", csmaDevices.Get (1), true);
//
//  Simulator::Stop (Seconds (10.0));
//
//  Simulator::Run ();
//
//  Simulator::Destroy ();
  return 0;
}

