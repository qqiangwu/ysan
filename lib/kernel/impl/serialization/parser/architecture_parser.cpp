#include "kernel/impl/serialization/parser/architecture_parser.h"
#include "kernel/impl/serialization/parser/id_parser.h"

namespace yrui {
namespace serialization {
    Architecture_parser::Architecture_parser()
        : base_type(archs_)
    {
        using namespace boost::spirit::qi;

        boost::spirit::qi::unicode::char_type char_;

        int_type int_;
        lexeme_type lexeme;

        archs_ = *arch_;
        arch_ %= id_ //! id
            > lexeme[+(char_ - ' ')] //! name
            > id_ //! fraction
            > id_ //! mayor
            > int_ //! architecture type
            > int_ //! farming
            > int_ //! max_farming
            > int_ //! farming_growth
            > int_ //! trade
            > int_ //! max_trade
            > int_ //! trade_growth
            > int_ //! steady
            > int_ //! max_steady
            > int_ //! security
            > int_ //! security growth
            > int_ //! population
            > int_ //! max_population
            > int_ //! population growth
            > int_ //! money
            > int_ //! food
            > int_ //! corrupt
            > int_ //! capacity
            > int_ //! aristocrat
            ;

        BOOST_SPIRIT_DEBUG_NODES((arch_)(archs_));
    }
} // of namespace serialization
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
