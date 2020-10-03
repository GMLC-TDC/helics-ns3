/*
*
* ns3 version of the hypercube network model
* presented in the OMNet++ examples
*
* Hypercube network with simplified deflection routing
* Node Application: HCNNode
*   HCNNodes: router + source + sink
* param: dimension (d)
* 1.) router receives packets
* 2.) decide which gate each generated packet is sent out
* 3.) @next time slot, send out packets
*
* ns3Fed on setup subscribes to broker
* when it receives a message (new dimension)
* via the broker it passes it to the ns3 network
* to build a new hypercube
* TOOO:
* fix the message passing application (custom)
*/

#include <string>
#include <ns3/core-module.h>
#include "ns3/network-module.h"
#include <ns3/point-to-point-module.h>
#include "ns3/internet-module.h"
#include <ns3/applications-module.h>
#include "ns3/ipv4-global-routing-helper.h"
#include <stdio.h>
#include <sstream>
#include "ns3/helics-helper.h"
#include "ns3/simulator.h"
#include <vector>
#include <string>
#include <iostream>
#include "helics/core/helicsCLI11.hpp"


using namespace ns3;
using namespace std;

NS_LOG_COMPONENT_DEFINE ("HCN_Example");



void createHCN(double newDim)
{
  int num_nodes = pow(2,newDim);
  LogComponentEnable ("PacketSink", LOG_LEVEL_LOGIC);
  LogComponentEnable ("OnOffApplication", LOG_LEVEL_LOGIC);




  NS_LOG_INFO("Generating Nodes");
  NodeContainer nodes;
  nodes.Create(num_nodes);

  PointToPointHelper p2p;
  p2p.SetDeviceAttribute("DataRate", StringValue ("5Mbps"));    //fixme fix values later
  p2p.SetChannelAttribute("Delay", StringValue ("2ms"));

  InternetStackHelper istack;
  istack.Install (nodes);

  Ipv4AddressHelper ipv4;
  Ipv4InterfaceContainer interfaces;
  ipv4.SetBase ("10.0.0.0", "255.255.255.0");

  NS_LOG_INFO("Creating Edges");


  int count =0;

  for(int i=0; i<num_nodes; i++)
  {
    for(int j=0; j<newDim; j++)
    {
        if(i < (i^(1<<j)))
        {
          NodeContainer p2plink = NodeContainer (nodes.Get (i), nodes.Get(i < (i^(1<<j))));
          NetDeviceContainer devices = p2p.Install(p2plink);
          ipv4.Assign(devices);
          ipv4.NewNetwork();
          count++;
        }
    }
  }

  NS_LOG_INFO("Assigning names, IPs, and sink apps");



  for (int i=0; i<num_nodes; i++)
  {
    stringstream ss;
    ss << "10.1." << i << ".0";
    ipv4.SetBase (ss.str().c_str(), "255.255.255.0");
    string nodename = "n" + to_string(i);
    Names::Add(nodename, nodes.Get(i));
    PacketSinkHelper sink ("ns3::UdpSocketFactory", InetSocketAddress (Ipv4Address::GetAny (), 9));
    ApplicationContainer apps = sink.Install (nodes.Get(i));
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(2.0));
  }

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    for(int i=0; i<num_nodes; i++)
    {
      //for(int j=0; j<num_nodes; j++)
      //{
        //if(i!=j)
        //{
          OnOffHelper onoff("ns3::UdpSocketFactory",  InetSocketAddress (Ipv4Address::GetAny (), 9));
          //onoff.SetConstantRate (DataRate ("5Mbps"));
          onoff.SetAttribute ("OnTime", StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          onoff.SetAttribute ("OffTime",  StringValue ("ns3::ConstantRandomVariable[Constant=1]"));
          ApplicationContainer app_send = onoff.Install(nodes.Get(i));
          app_send.Start(Seconds(2.0));
          app_send.Stop(Seconds(3.0));
        //}
      //}
    }


    NS_LOG_INFO("Simulation Start");

    Simulator::Stop (Seconds (10.0));
    Simulator::Run ();
    Simulator::Destroy ();

}

int main(int argc, char *argv[])
{

  using ns3::helics_federate;
  bool verbose = true;
  helics::helicsCLI11App app ("HCN NS3 Example", "HCNNS3Example");
  ns3::HelicsHelper helicsHelper;
  ns3::CommandLine cmd;
  helicsHelper.SetupCommandLine (cmd);
  if(verbose)
  {
    LogComponentEnable ("HCN_Example", LOG_LEVEL_INFO);
    //LogComponentEnable ("HelicsSimulatorImpl", LOG_LEVEL_LOGIC);
    LogComponentEnable ("HelicsApplication", LOG_LEVEL_LOGIC);
  }

  GlobalValue::Bind("SimulatorImplementationType", StringValue ("ns3::HelicsSimulatorImpl"));

  NS_LOG_INFO ("Calling helicsHelper.SetupApplicationFederate");

  bool add_name = true;
  vector<string> args;
  const char* const name_setting = "--name=";
  const auto name_len = strlen(name_setting);
  for (int i = 0; i < argc; ++i)
  {
     if (strncmp (argv[i], name_setting, name_len) == 0)
     {
         add_name = false;
     }
     args.push_back (argv[i]);
  }
  if (add_name)
  {
     args.push_back ("--name=HCN");
  }

  cmd.Parse (args);
  helicsHelper.SetupApplicationFederate();
  helics_federate->setProperty (helics_property_int_log_level, 5);
  helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("2s"));

  string name = helics_federate->getName();

  cout << " registering publication '" << "newDim" << "' for " << name <<'\n';
  auto &received = helics_federate->registerSubscription("newDim");
  //std::cout << " registering publication '" << pubKey << "' for " << name <<'\n';
  //auto &reply = helics_federate->registerGlobalPublication<std::string> ("sendback");

  cout << "entering init State \n";
  helics_federate->enterInitializingMode ();
  cout << "entered init State\n";
  helics_federate->enterExecutingMode ();
  cout << "entered exec State \n";
  auto currenttime = helics_federate ->requestTime (Simulator::Now().GetSeconds());

  if (received.checkUpdate())
  {
    auto new_val = received.getValue<string> ();
    cout<<"MESSAGE RECEIVED!!!!!!!!!!!!!!!!!!!!" << "\n";
    createHCN(stod(new_val));
  }
  else
  {
    cout<<"No New Value";
  }
  cout<<"SIM COMPLETE!!!!!!!!!!!!!!!!!!!!!!!!" << "\n";
  helics_federate->finalize();


  return 0;
}
