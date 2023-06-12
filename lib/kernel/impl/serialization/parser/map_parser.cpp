#include "kernel/impl/serialization/parser/map_parser.h"
#include "kernel/impl/serialization/parser/id_parser.h"
#include <boost/spirit/include/phoenix_function.hpp>
#include <boost/spirit/include/phoenix_object.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

namespace yrui {
namespace serialization {
    namespace detail {
        struct Make_map_impl {
            using result_type = yrui::resource::Map::graph_type;

            template <class VertexVec, class EdgeVec>
            result_type operator()(const VertexVec& vv, const EdgeVec& ev) const
            {
                return yrui::resource::detail::make_graph(vv, ev);
            }
        };

        const boost::phoenix::function<Make_map_impl> make_graph;
    } // of namespace detail

    Map_parser::Map_parser()
        : base_type(map_)
    {
        using namespace boost::spirit;
        using namespace qi;

        namespace ph = boost::phoenix;

        qi::int_type int_;
        qi::lit_type lit;

        qi::_val_type _val;
        qi::_1_type _1;
        qi::_2_type _2;
        qi::_3_type _3;

        map_ = (lit('{') > vertices_ > lit('}') > lit('{') > edges_
            > lit('}'))[_val = yrui::serialization::detail::make_graph(_1, _2)];

        vertices_ = *vertex_;

        edges_ = *edge_;

        vertex_ = (id_ > int_ > int_)[_val = ph::construct<resource::Map::point_type>(_2, _3)];

        edge_ = (id_ > id_)[_val = ph::construct<std::pair<int, int>>(_1 - 1, _2 - 1)];

        BOOST_SPIRIT_DEBUG_NODES((map_)(edge_)(vertex_)(edges_)(vertices_));
    }
} // of namespace serialization
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
