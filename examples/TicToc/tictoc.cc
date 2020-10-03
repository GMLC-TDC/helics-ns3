/*
* simpleTicToc example [as seen in helics omnet++ examples] version ns3
* also based on  ns3's first.cc
* publication federate pubFed
* created August 2020
* author: aenkoji
*
* simple example
* federates: publication federate (pubFed) && ns3 federate (test)
* pubFed will publish to broker -> test the value "Hello from pub!"
* this (tictoc) federate will subscribe to pubFed via broker

* tictoc will make a publication and subscription to pubFed
* tictoc will receive the published message "Hello from pub!" from pubFed
* this will initiate comm (tictoc) simulation
* when finished with comm simulation, it will publish
* a "Simulation is done" message to pubFed
* USES a udpechoserver and udpechoclient to simulate the
* back and forth sending of messages
* TODO: match simulation parameters with omnet example
*/

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/helics-helper.h"
#include "ns3/simulator.h"


// Network Topology
//
//       10.1.1.0
// tic <------------> inputFed
//    point-to-point
//


using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TicTocExample");


int main(int argc, char *argv[])
{
    using ns3::helics_federate;
    bool verbose = true;

    HelicsHelper helicsHelper;

    std::string tic = "tic";
    std::string inputFed = "inputFed";
    std::string pubKey = "sendback";
    std::string subKey = "time";

    CommandLine cmd;
    helicsHelper.SetupCommandLine(cmd);
    cmd.AddValue ("src", "First helics endpoint", tic);
    cmd.AddValue ("fed", "Last helics endpoint", inputFed);
    cmd.Parse (argc,argv);

    if(verbose)
    {
      LogComponentEnable ("TicTocExample", LOG_LEVEL_INFO);
      //LogComponentEnable ("HelicsSimulatorImpl", LOG_LEVEL_LOGIC);
      LogComponentEnable ("HelicsApplication", LOG_LEVEL_LOGIC);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_LOGIC);
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_LOGIC);
    }


    GlobalValue::Bind("SimulatorImplementationType", StringValue ("ns3::HelicsSimulatorImpl"));

    NS_LOG_INFO ("Calling helicsHelper.SetupApplicationFederate");

    bool add_name = true;
    std::vector<std::string> args;

    const char* const name_setting = "--name=";
    const auto name_len = std::strlen(name_setting);
    for (int i = 0; i < argc; ++i)
    {
       if (std::strncmp (argv[i], name_setting, name_len) == 0)
       {
           add_name = false;
       }
       args.push_back (argv[i]);
    }
    if (add_name)
    {
       args.push_back ("--name=tictoc");
    }


    cmd.Parse (args);
    helicsHelper.SetupApplicationFederate();
    helics_federate->setProperty (helics_property_int_log_level, 5);
    helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("2s"));


    NodeContainer nodes;
    nodes.Create(2);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue ("10Mbps"));         //TODO match data rate, packet sizew, delay, etc with omnet example
    pointToPoint.SetChannelAttribute("Delay", StringValue ("1s"));

    NetDeviceContainer devices;
    devices = pointToPoint.Install (nodes);

    InternetStackHelper stack;
    stack.Install (nodes);

    Ipv4AddressHelper address;
    address.SetBase ("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces;
    interfaces = address.Assign (devices);
    UdpEchoServerHelper echoServer(9);  //default udp echo ports
    UdpEchoClientHelper echoClient(interfaces.GetAddress(1),9);
    echoClient.SetAttribute("Interval", TimeValue(Seconds(2.5)));


    const auto name = helics_federate->getName();


    std::cout <<" registering subscription '" << subKey << "' for " << name <<'\n';
    auto &received = helics_federate->registerSubscription (subKey);

    std::cout << " registering publication '" << pubKey << "' for " << name <<'\n';
    auto &reply = helics_federate->registerGlobalPublication<std::string> (pubKey);

    std::cout << "entering init State \n";
    helics_federate->enterInitializingMode ();
    std::cout << "entered init State\n";
    helics_federate->enterExecutingMode ();
    std::cout << "entered exec State \n";

    auto newTime = helics_federate ->requestTime (Simulator::Now().GetSeconds());


    if (received.checkUpdate())
    {
        const auto nmessage = received.getValue<std::string> ();
        std::cout << "received publication on key" << received.getKey() << " at " << static_cast<double> (received.getLastUpdate()) << " ::" <<nmessage << '\n';

        ApplicationContainer tic = echoServer.Install(nodes.Get(1));
        tic.Start (Seconds (0.0));
        tic.Stop (Seconds (10.0));

        ApplicationContainer inputFed = echoClient.Install(nodes.Get(0));
        inputFed.Start (Seconds (0.0));
        inputFed.Stop (Seconds (10.0));

        helics_federate->publish(reply, "Simulation is done");
        newTime = helics_federate->requestTime(Simulator::Now().GetSeconds());      //might be an issue, since above publication ends session for pubFed
        NS_LOG_INFO ("Running Simulation.");
        Simulator::Stop (Seconds (10.0));
        Simulator::Run ();
        Simulator::Destroy ();
    }


    helics_federate->finalize();
    return 0;

}
