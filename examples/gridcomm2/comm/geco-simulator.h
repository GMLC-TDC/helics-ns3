/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef GECO_SIMULATOR_H
#define GECO_SIMULATOR_H

#include "ns3/gridcomm-graph.h"
#include "ns3/command-line.h"
#include "ns3/node-container.h"

namespace ns3
{

class GecoSimulator
{
public:
  /**
   * \brief Constructs a new GecoSimulator object.
   * Parameters from main() are typically supplied to this constructor.
   * \param argc number of command-line arguments
   * \param argv array of char* representing the command-line args
   */
  GecoSimulator (int argc, char* argv[]);

  /**
   * \brief Destructor.
   */
  virtual ~GecoSimulator ();

  /**
   * \brief Runs the simulation.
   * \return 0 if the simulation succeeded, non-zero otherwise
   */
  int Run (void);

  // TODO: Give the client the ability to set a simulation scenario, e.g.
  // set local and remote faults on specific apps.

private:
  GecoSimulator ();
  GecoSimulator (const GecoSimulator& other);
  GecoSimulator& operator= (const GecoSimulator& other);

  /**
   * \brief Initializes internal variables of the simulation.
   * \return true if initialization succeeded, false otherwise.
   */
  bool Initialize (void);

  /**
   * \brief Performs internal setup of the simulation scenario.
   * \return true if setup succeeded, false otherwise.
   */
  bool Setup (void);

  /**
   * \brief Sets up packet sink and onoff applications to simulate
   *        background traffic.
   */
  void SetupBackgroundTraffic (void);

  /** ns3 command-line processor */
  CommandLine m_cmdline;

  /** The file name of the user-supplied (from command-line) communication
      network (graphviz dot format). */
  std::string m_commNetworkFilename;

  /** The file name of the user-supplied (from command-line) electrical
      network (graphviz dot format). */
  std::string m_elecNetworkFilename;

  /** User-specified value (from command-line) for increased verbosity. */
  bool m_verbose;

  /** The storage for the input communication network specified by the user. */
  gridcomm::graph_t m_commNetwork;

  /** The storage for the input electrical network specified by the user. */
  gridcomm::graph_t m_elecNetwork;

  /** The properties used for parsing the communication network. */
  boost::dynamic_properties m_commProperties;

  /** The properties used for parsing the electrical network. */
  boost::dynamic_properties m_elecProperties;

  /** Vector of CSMA BGL nodes in the input graph. */
  gridcomm::NodeVector m_csmaNodes;

  /** Vector of router BGL nodes in the input graph. */
  gridcomm::NodeVector m_routerNodes;

  /** Vector of agent BGL nodes in the graph. */
  gridcomm::NodeVector m_agentNodes;

  /** NodeContainer for ns3 agent nodes. Used by application helper to
      install applications on agent nodes. */
  NodeContainer m_agentNodeContainer;
};

} // end namespace

#endif /* GECO_SIMULATOR_H */
