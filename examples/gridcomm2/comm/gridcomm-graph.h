/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#ifndef GRIDCOMM_GRAPH_H
#define GRIDCOMM_GRAPH_H

#include "ns3/ptr.h"
#include "ns3/node.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace boost::graph;

namespace boost {
  enum graph_directed_t    { graph_directed };
  enum graph_boundingbox_t { graph_boundingbox };
  enum graph_overlap_t     { graph_overlap };

  enum vertex_nodetype_t   { vertex_nodetype };
  enum vertex_width_t      { vertex_width };
  enum vertex_height_t     { vertex_height };
  enum vertex_shape_t      { vertex_shape };
  enum vertex_label_t      { vertex_label };
  enum vertex_position_t   { vertex_position };
  enum vertex_delay_t      { vertex_delay };
  enum vertex_datarate_t   { vertex_datarate };
  enum vertex_ipaddr_t     { vertex_ipaddr };

  enum edge_penwidth_t     { edge_penwidth };
  enum edge_position_t     { edge_position };
  enum edge_delay_t        { edge_delay };
  enum edge_datarate_t     { edge_datarate };

  BOOST_INSTALL_PROPERTY(graph,  directed);
  BOOST_INSTALL_PROPERTY(graph,  boundingbox);
  BOOST_INSTALL_PROPERTY(graph,  overlap);
  BOOST_INSTALL_PROPERTY(vertex, nodetype);
  BOOST_INSTALL_PROPERTY(vertex, width);
  BOOST_INSTALL_PROPERTY(vertex, height);
  BOOST_INSTALL_PROPERTY(vertex, shape);
  BOOST_INSTALL_PROPERTY(vertex, label);
  BOOST_INSTALL_PROPERTY(vertex, position);
  BOOST_INSTALL_PROPERTY(vertex, delay);
  BOOST_INSTALL_PROPERTY(vertex, datarate);
  BOOST_INSTALL_PROPERTY(vertex, ipaddr);
  BOOST_INSTALL_PROPERTY(edge,   penwidth);
  BOOST_INSTALL_PROPERTY(edge,   position);
  BOOST_INSTALL_PROPERTY(edge,   delay);
  BOOST_INSTALL_PROPERTY(edge,   datarate);
}

namespace ns3 {

namespace gridcomm {

typedef property<vertex_name_t,     std::string,
        property<vertex_nodetype_t, std::string,
        property<vertex_width_t,    double,
        property<vertex_height_t,   double,
        property<vertex_shape_t,    std::string,
        property<vertex_label_t,    std::string,
        property<vertex_position_t, std::string,
        property<vertex_delay_t,    std::string,
        property<vertex_datarate_t, std::string,
        property<vertex_ipaddr_t,   std::string
        > > > > > > > > > > vertex_p;

typedef property<edge_penwidth_t, int,
        property<edge_name_t,     std::string,
        property<edge_position_t, std::string,
        property<edge_delay_t,    std::string,
        property<edge_datarate_t, std::string
        > > > > > edge_p;

typedef property<graph_directed_t,    std::string,
        property<graph_boundingbox_t, std::string,
        property<graph_overlap_t,     std::string
        > > > graph_p;

/** BGL graph type that is used by gridcomm applications. */
typedef boost::adjacency_list<
  vecS, vecS, undirectedS, vertex_p, edge_p, graph_p> graph_t;

/** Vector of BGL vertex descriptors. */
typedef std::vector<graph_traits<graph_t>::vertex_descriptor> NodeVector;

/** Edge */
typedef std::pair<graph_traits<graph_t>::vertex_descriptor,
                  graph_traits<graph_t>::vertex_descriptor> Edge;

/** Vector of edges. */
typedef std::vector<Edge> EdgeVector;

/** Maps vertex descriptors to ns-3 nodes. */
typedef std::map<graph_traits<graph_t>::vertex_descriptor,
                 ns3::Ptr<ns3::Node> > NodeMap;

void SetupProperties (graph_t& g, dynamic_properties& dp);
bool LoadGraph (std::string& infile, graph_t& graph, dynamic_properties& dp);

const std::string AGENT_NODE_TYPE = "agent";
const std::string CSMA_NODE_TYPE = "csma";
const std::string ROUTER_NODE_TYPE = "router";

} // namespace gridcomm

} // namespace ns3

#endif /* GRIDCOMM_GRAPH_H */
