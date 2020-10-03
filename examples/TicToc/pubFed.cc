/*
* simpleTicToc example [as seen in helics omnet++ examples] version ns3
* publication federate pubFed
* created August 2020
* author: aenkoji
*
* simple example
* federates: publication federate (pubFed) && ns3 federate (test)
* pubFed will publish to broker -> tictoc the value "Hello from pub!" &&
* subscribe to updates from tictoc
* tictoc will publish abd subscribe to pubFed via broker
*
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
  helics::helicsCLI11App app ("Pub Test Example", "PubTestNS3Example");
  std::string pubKey = "time";
  std::string subKey = "sendback";

  ns3::CommandLine cmd;
  ns3::HelicsHelper helicsHelper;
  cmd.AddValue ("key", "name of the key to publish on", pubKey);
  cmd.AddValue ("key", "name of the key to subscribe on", subKey);
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
     args.push_back ("--name=pubFed");
  }


  cmd.Parse (args);
  helicsHelper.SetupFederate (args);
  helics_federate->setProperty (helics_property_int_log_level, 5);
  helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("2s"));

  const auto name = helics_federate->getName();
  std::cout << " registering publication '" << pubKey << "' for " << name <<'\n';
  auto &pub = helics_federate->registerGlobalPublication<std::string> (pubKey);
  std::cout <<" registering subscription '" << subKey << "' for " << name <<'\n';
  auto &sub = helics_federate->registerSubscription (subKey);

  std::cout << "entering init State \n";
  helics_federate->enterInitializingMode ();
  std::cout << "entered init State\n";
  helics_federate->enterExecutingMode ();
  std::cout << "entered exec State \n";

  std::string message = "Hello from pub!";
  helics_federate->publish(pub, message);
  std::cout << message <<std::endl;
  auto newTime = helics_federate ->requestTime (2.0);


  std::cout << "processed time " << static_cast<double> (newTime) << "\n";
  newTime = helics_federate ->requestTime (4.0);
  std::cout << "processed time " << static_cast<double> (newTime) << "\n";
  if (sub.checkUpdate())
  {
    const auto nmessage = sub.getValue<std::string> ();
    std::cout << "received publication on key " << sub.getKey() << " at " << static_cast<double> (sub.getLastUpdate ()) << " ::" << nmessage << '\n';
  }
  std::cout<<sub.checkUpdate();
  helics_federate->finalize ();
  return 0;

}
