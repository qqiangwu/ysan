#ifndef YRUI_CORE_MAP_H_
#define YRUI_CORE_MAP_H_

#include <boost/graph/adjacency_list.hpp>
#include <vector>

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include "kernel/resource/location.h"

namespace yrui {
namespace resource {

    //! map object that retain all map and architecture location information
    class Map {
    public:
        typedef Location point_type;
        typedef Map self_type;
        typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, point_type> graph_type;
        typedef graph_type::vertex_descriptor vertex_descriptor;

    public:
        //! \pre    \es contains edge descriptor, not id_t
        Map(const std::vector<point_type>& vs, const std::vector<std::pair<int, int>>& es);

        template <class GraphT>
        explicit Map(GraphT&& g)
            : map_(std::forward<GraphT>(g))
        {
        }

        const graph_type& graph() const;

        std::vector<Architecture*> adjacent_city(const Architecture* city) const;

        bool is_frontier(const Architecture* x) const;
        bool is_connected(const Architecture* x, const Architecture* y) const;

        int max_turn(const Architecture* x, const Architecture* y) const;

    private:
        graph_type map_;
        std::vector<std::vector<int>> distance_;
    };

    namespace detail {
        Map::graph_type make_graph(const std::vector<Map::point_type>& vs, const std::vector<std::pair<int, int>>& es);
    } // of namespace detail
} // of namespace resource
} // of namespace yrui

namespace yrui {
namespace resource {
    inline const Map::graph_type& Map::graph() const { return map_; }

    inline constexpr id_t id(Map::vertex_descriptor v) noexcept { return v + 1; }

    inline constexpr Map::vertex_descriptor descriptor(id_t id) noexcept { return id - 1; }
} // of namespace resource
} // of namespace yrui

#endif // !YRUI_CORE_MAP_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
