
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
#include "geco_app.h"

NS_LOG_COMPONENT_DEFINE ("CommExample");

using namespace ns3;

int main(int argc, char *argv[])
{
  using ns3::helics_federate;
  bool verbose = true;
  HelicsHelper helicsHelper;
  CommandLine cmd;
  helicsHelper.SetupCommandLine(cmd);
  if (verbose)
  {
    LogComponentEnable ("CommExample", LOG_LEVEL_INFO);
    //LogComponentEnable ("HelicsSimulatorImpl", LOG_LEVEL_LOGIC);
    LogComponentEnable ("HelicsApplication", LOG_LEVEL_LOGIC);
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
     args.push_back ("--name=commFed");
  }
  cmd.Parse(args);
  helicsHelper.SetupApplicationFederate();
  helics_federate->setProperty (helics_property_int_log_level, 5);
  helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("1s"));

  helics_federate->registerEndpoint("commEndpt", "");

  std::cout << "entering init State \n";
  helics_federate->enterInitializingMode ();
  std::cout << "entered init State\n";
  helics_federate->enterExecutingMode ();
  std::cout << "entered exec State \n";

  //gecoApp::SetTopology();   //TODO this function needs major fixing

  //check for update from grid side
  //NOTE: need to create a Federate class and turn the following
  //into a checkForMessage NS_LOG_FUNCTION
  //currently a simple one time operation
  // also need to create a buffer for multiple messages
  auto currenttime = helics_federate ->requestTime (Simulator::Now().GetSeconds());
  if (helics_federate->hasMessage())
  {
    auto msg = helics_federate->getMessage();
    std::cout << "Received message from " << msg->source << " at " << static_cast<double>(msg->time) << '\n';
    std::string sMsg = msg->data.to_string();
    std::string mType = sMsg.substr(sMsg.find("mType:")+6,1);
    std::string mCode = sMsg.substr(sMsg.find("mCode:")+6, 3);
  }
  else
  {
    std::cout<<"No Message Recevied";
  }

  //Responding to received message



  helics_federate->finalize();

  return 0;

}
