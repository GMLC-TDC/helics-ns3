/*
* HELICS federate for Gridcomm example power system
* created August 2020
* author: aenkoji
*
*/
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>   //ctime?

#include "helics/application_api/MessageFederate.hpp"
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/helics-helper.h"
#include "ns3/simulator.h"
#include "helics/core/helicsCLI11.hpp"


using namespace std;

void readAgentNodes(vector<string> &listAgent, string name)
{
  ifstream infile;
  if(!infile)
  {
      cout<< "ERROR: unable to retrieve file" <<endl;
  }

  infile.open(name);
  if (infile.is_open())
  {
    string agent = "";
    while(!infile.eof())
    {
      infile >> agent;
      cout<<agent<<endl;
      listAgent.push_back(agent);
    }
    infile.close();
  }
}

string chooseRandomAgent(vector<string> listAgent)
{

  srand(time(0));
  int index = rand()%68;
  return listAgent[index];
}


int main (int argc, char *argv[])
{
  using ns3::helics_federate;
  ns3::HelicsHelper helicsHelper;
  ns3::CommandLine cmd;
  vector<string> agentsList;
  helics::helicsCLI11App app ("Gridcomm ns3 Example", "GridcommNS3Example");
  std::string myName = "subFed";



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

  helics_federate->setProperty (helics_property_int_log_level, 5);
  helics_federate->setProperty (helics_property_time_delta, helics::loadTimeFromString("1s"));

  const auto name = helics_federate->getName();

  std::cout << " registering endpoint '" << "Re:Griddyn" << "' for " <<name<<'\n';
  auto &subEndpt = helics_federate->registerEndpoint("Re:Griddyn", "");



  std::cout << "entering init State\n";
  helics_federate->enterInitializingMode ();
  std::cout << "entered init State\n";
  helics_federate->enterExecutingMode ();
  std::cout << "entered exec State\n";


/*    ACTUAL CODE TODO, RETURN TO THIS !!!!!!!!!!!!!!!!!!!!!!!
  int j = 2;
	while (j!= 0)
	{
    std::string msg = chooseRandomAgent(agentList);
    cout << msg << endl;
    helics_federate->sendMessage(subEndpt, "commEndpt", msg.data(), msg.size());
		j--;
	}

*/

  /*TODO THIS IS FOR TESTING PURPOSES ONLY!!!!!!!!!*/
	std::string msg = "_4_14";
  cout << msg << endl;
  helics_federate->sendMessage(subEndpt, "commEndpt", msg.data(), msg.size());


  auto currenttime = helics_federate->requestTime (1.0);
  if (helics_federate->hasMessage())
  {
    auto newMsg = helics_federate->getMessage();
    cout << "received message from " << newMsg->source << " at " << static_cast<double>(newMsg->time) << ": " << newMsg->data.to_string() << '\n';

  }
  else
  {
    cout << "No Message" << endl;
  }


  helics_federate->finalize ();
  return 0;


}
