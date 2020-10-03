# SimpleTicToc example
(As based on omnet++ SimpleTicToc example and ns3 first.cc) 

This example consists of pubFed.cc (the main publication federate) and the ns3 federate tictoc in tictoc.cc
The federate pubFed will publish the value "Hello from pub!" to tictoc, which will then initiate a ns3 network simulation that will echo data between two nodes.

## Set Up ##
After building with Waf (./waf build):
1. Execute helics_broker -f2 on commandline
2. Execute ./waf --run pubFed (from within the ns-3-dev directory)
3. Execute ./waf --run tictoc

### Note ###
Ensure that wscript file is updated to include the above two files in order to be built
