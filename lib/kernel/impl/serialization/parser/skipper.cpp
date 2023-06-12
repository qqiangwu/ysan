#include "kernel/impl/serialization/parser/skipper.h"

namespace yrui {
namespace serialization {
    Skipper::Skipper()
        : base_type(blank_)
    {
        using namespace boost::spirit;
        using namespace qi;

        qi::unicode::space_type space;
        qi::unicode::char_type char_;

        qi::lexeme_type lexeme;
        qi::lit_type lit;
        qi::eoi_type eoi;
        qi::eol_type eol;

        blank_ = comment_ | space;
        comment_ = lit('%') > lexeme[*(char_ - comment_end_)] > comment_end_;
        comment_end_ = lit('%') | eoi | eol;
    }
} // of namespace serialization
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
