#include "kernel/impl/serialization/parser/fraction_parser.h"
#include "kernel/impl/serialization/parser/id_parser.h"

namespace yrui {
namespace serialization {
    Fraction_parser::Fraction_parser()
        : base_type(pack_)
    {
        using namespace boost::spirit::qi;

        lit_type lit;
        int_type int_;
        _val_type _val;
        _1_type _1;
        _2_type _2;
        _3_type _3;

        pack_ %= fractions_ > relations_;

        namespace Ph = boost::phoenix;
        fractions_ %= lit('{') > *fraction_ > lit('}');

        relations_ = lit('{') > *(id_ > id_ > relation_)[detail::push_rel(_val, _1, _2, _3)] > lit('}');

        fraction_ %= id_ //! id
            > id_ //! leader
            > id_ //! capital
            > int_ //! has experor
            > int_ //! has seal
            > int_ //! money
            > int_ //! food
            ;

        relation_ %= int_ //! friendliness
            > int_ //! relation state
            ;

        BOOST_SPIRIT_DEBUG_NODES((pack_)(relations_)(fractions_)(relation_)(fraction_));
    }
} // of namespace serialization
} // of namespace yrui
