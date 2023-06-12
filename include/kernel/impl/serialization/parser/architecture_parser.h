#ifndef YRUI_SERIALIZATION_ARCHITECTURE_PARSER_
#define YRUI_SERIALIZATION_ARCHITECTURE_PARSER_

#include "kernel/impl/serialization/adapt_architecture.h"
#include "kernel/impl/serialization/parser/config.h"
#include "kernel/impl/serialization/parser/skipper.h"
#include <boost/spirit/include/qi.hpp>

namespace yrui {
namespace serialization {
    struct Architecture_parser : boost::spirit::qi::grammar<parser_iterator, unicode_encoding,
                                     std::vector<yrui::detail::Raw_architecture>(), Skipper> {

        Architecture_parser();

    private:
        start_type archs_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, yrui::detail::Raw_architecture(), skipper_type> arch_;
    };
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_ARCHITECTURE_PARSER_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
