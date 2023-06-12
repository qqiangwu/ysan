#ifndef YRUI_SERIALIZATION_SKIPPER_H_
#define YRUI_SERIALIZATION_SKIPPER_H_

#include "kernel/impl/serialization/parser/config.h"
#include <boost/spirit/include/qi.hpp>

namespace yrui {
namespace serialization {
    struct Skipper : boost::spirit::qi::grammar<parser_iterator, unicode_encoding> {

        Skipper();

    private:
        using rule_t = start_type;

        rule_t blank_;
        rule_t comment_;
        rule_t comment_end_;
    };
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_SKIPPER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
