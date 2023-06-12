#ifndef YRUI_SERIALIZATION_MAP_PARSER_H_
#define YRUI_SERIALIZATION_MAP_PARSER_H_

/*!
 *
 * \file     Serialization/Parser/map_parser.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    parse the database file to big map
 * \ingroup  Serialization
 *
 */

#include "kernel/impl/serialization/parser/config.h"
#include "kernel/impl/serialization/parser/skipper.h"
#include <boost/spirit/include/qi.hpp>
#include <kernel/resource/map.h>

namespace yrui {
namespace serialization {
    struct Map_parser
        : boost::spirit::qi::grammar<parser_iterator, unicode_encoding, resource::Map::graph_type(), Skipper> {

        Map_parser();

    private:
        using edge_type = std::pair<int, int>;

        start_type map_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, std::vector<edge_type>(), skipper_type> edges_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, std::vector<resource::Map::point_type>(), skipper_type>
            vertices_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, edge_type(), skipper_type> edge_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, resource::Map::point_type(), skipper_type> vertex_;
    };
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_MAP_PARSER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
