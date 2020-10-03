#include "gridcomm-graph.h"
#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;
using namespace boost::graph;
using namespace ns3::gridcomm;

namespace ns3 {

namespace gridcomm {

void SetupProperties (graph_t& g, dynamic_properties& dp)
{
  // Graph properties
  ref_property_map<graph_t*, std::string>
    gdir (get_property (g, graph_directed));
  dp.property ("directed", gdir);
  ref_property_map<graph_t*, std::string>
    gbb (get_property (g, graph_boundingbox));
  dp.property ("bb", gbb);
  ref_property_map<graph_t*, std::string>
    ov (get_property (g, graph_overlap));
  dp.property ("overlap", ov);

  // Vertex properties
  dp.property ("id",       get (vertex_name, g));
  dp.property ("nodetype", get (vertex_nodetype, g));
  dp.property ("shape",    get (vertex_shape, g));
  dp.property ("width",    get (vertex_width, g));
  dp.property ("height",   get (vertex_height, g));
  dp.property ("label",    get (vertex_label, g));
  dp.property ("pos",      get (vertex_position, g));
  dp.property ("delay",    get (vertex_delay, g));
  dp.property ("datarate", get (vertex_datarate, g));
  dp.property ("ipaddr",   get (vertex_ipaddr, g));

  // Edge properties
  dp.property ("pos",      get (edge_position, g));
  dp.property ("penwidth", get (edge_penwidth, g));
  dp.property ("name",     get (edge_name, g));
  dp.property ("delay",    get (edge_delay, g));
  dp.property ("datarate", get (edge_datarate, g));
}

bool LoadGraph (std::string &infile, graph_t &graph, dynamic_properties &dp)
{
  std::ifstream ifs;
  bool readOk = false;
  try
    {
      ifs.open (infile.c_str (), std::ifstream::in);
      if (ifs.good ())
        {
          readOk = read_graphviz (ifs, graph, dp, "id");
        }
      ifs.close ();
    }
  catch (std::ios_base::failure &e)
    {
      std::cerr << "Error reading file: " << e.what () << std::endl;
    }
  catch (graph_exception &ge)
    {
      std::cerr << "Error parsing graph: " << ge.what () << std::endl;
    }
  return readOk;
}


} // namespace gridcomm

} // namespace ns3
