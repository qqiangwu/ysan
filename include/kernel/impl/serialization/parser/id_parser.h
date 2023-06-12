#ifndef YRUI_SERIALIZATION_ID_PARSER_H_
#define YRUI_SERIALIZATION_ID_PARSER_H_

#include "config.h"
#include <boost/spirit/home/qi/numeric/uint.hpp>

namespace yrui {
namespace serialization {
    const boost::spirit::qi::uint_parser<id_t> id_;
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_ID_PARSER_H_
