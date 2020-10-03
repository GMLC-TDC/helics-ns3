/*
* ns3 version of the hypercube network model
* presented in the OMNet++ examples
*
* helics (publication) federate
* this federate will register itself to the helics broker
* and will publish a new dimension
*
* on the ns3Fed federate side, ns3Fed will subscribe to
* the broker and requests a new dimension from this (pubHCN)
* via the broker. Once the ns3Fed receives the new dimensions,
* it passes it to the ns3 network to build the new hypercube
*/



#include <stdio.h>
#include <string>
#include "ns3/helics-helper.h"
#include <iostream>
#include <memory>
#include <vector>
#include <cstring>
#include "helics/core/helicsCLI11.hpp"

int main(int argc, char *argv[])
{
  using ns3::helics_federate;
  helics::helicsCLI11App app ("PubHCN Test Example", "PubHCNTestNS3Example");
  std::string pubKey = "newDim";

  ns3::CommandLine cmd;
  ns3::HelicsHelper helicsHelper;
  cmd.AddValue ("key", "name of the key to publish on", pubKey);
  helicsHelper.SetupCommandLine(cmd);


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
     args.push_back ("--name=pubHCN");
  }

  cmd.Parse (args);
  helicsHelper.SetupFederate (args);
  helics_federate->setProperty (helics_property_int_log_level, 5);
  helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("1s"));

  const auto name = helics_federate->getName();
  std::cout << " registering publication '" << "newDim" << "' for " << name <<'\n';
  auto &nodes = helics_federate->registerGlobalPublication<std::string> ("newDim");

  std::cout << "entering init State \n";
  helics_federate->enterInitializingMode ();
  std::cout << "entered init State\n";
  helics_federate->enterExecutingMode ();
  std::cout << "entered exec State \n";
  
  helics_federate->publish(nodes, 4.0);   //sending new dim 4
  auto currenttime = helics_federate ->requestTime (2.0);
  helics_federate->finalize();
  return 0;


}
