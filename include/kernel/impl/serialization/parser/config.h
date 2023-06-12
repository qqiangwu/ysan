#ifndef YRUI_SERIALIZATION_CONFIG_H_
#define YRUI_SERIALIZATION_CONFIG_H_

#include <boost/spirit/include/qi_char_class.hpp>

namespace yrui {
namespace serialization {
    using parser_iterator = std::string::const_iterator;
    using unicode_encoding = boost::spirit::char_encoding::unicode;
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_CONFIG_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
