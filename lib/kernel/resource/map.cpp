#include <boost/graph/properties.hpp>
#include <boost/graph/exterior_property.hpp>
#include <boost/graph/floyd_warshall_shortest.hpp>
#include <boost/graph/st_connected.hpp>

#include <limits>

#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/resource/map.h"

namespace yrui {
namespace resource {
    namespace detail {
        Map::graph_type make_graph(const std::vector<Map::point_type>& vs, const std::vector<std::pair<int, int>>& es)
        {
            Map::graph_type g(es.begin(), es.end(), vs.size());

            using vd_type = Map::graph_type::vertex_descriptor;

            //! add map vertex property
            for (vd_type v = 0; v < vs.size(); ++v) {
                g[v] = vs[v];
            }

            return g;
        }
    } // of namespace detail

    Map::Map(const std::vector<point_type>& vs, const std::vector<std::pair<int, int>>& es)
        : map_(detail::make_graph(vs, es))
    {
        //! \todo   add distance table
    }

    std::vector<Architecture*> Map::adjacent_city(const Architecture* city) const
    {
        auto& arch_mgr = use_subsys<arch::Architecture_mgr>();
        std::vector<Architecture*> ret;

        auto vd = descriptor(city->id());
        auto rg = boost::adjacent_vertices(vd, graph());
        for (auto beg = rg.first; beg != rg.second; ++beg) {
            ret.push_back(arch_mgr.get(id(*beg)));
        }

        return ret;
    }

    bool Map::is_connected(const Architecture* x, const Architecture* y) const
    {
        auto xd = descriptor(x->id());
        auto yd = descriptor(y->id());

        return boost::graph::st_connected(map_, xd, yd);
    }

    bool Map::is_frontier(const Architecture* x) const
    {
        auto adj = adjacent_city(x);
        return std::find_if(adj.begin(), adj.end(), [x](Architecture* a) { return a->fraction() != x->fraction(); })
            != adj.end();
    }

    int Map::max_turn(const Architecture* x, const Architecture* y) const { return is_connected(x, y) ? 4 : 1; }
} // of namespace resource
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
