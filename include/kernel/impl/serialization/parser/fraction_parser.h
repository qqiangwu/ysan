#ifndef YRUI_SERIALIZATION_FRACTION_PARSER_H_
#define YRUI_SERIALIZATION_FRACTION_PARSER_H_

#include "kernel/impl/serialization/adapt_fraction.h"
#include "kernel/impl/serialization/parser/config.h"
#include "kernel/impl/serialization/parser/skipper.h"
#include <boost/spirit/include/qi.hpp>

namespace yrui {
namespace serialization {
    namespace detail {
        struct Push_diplomacy_impl {
            using result_type = void;

            template <class Map> void operator()(Map& map, id_t a, id_t b, diplomacy::Relation rel) const
            {
                BOOST_ASSERT_MSG(a < b, "fraction a must less than b");
                map.insert({ { a, b }, rel });
            }
        };

        const boost::phoenix::function<Push_diplomacy_impl> push_rel;
    }

    struct Fraction_parser : boost::spirit::qi::grammar<parser_iterator, unicode_encoding, Fraction_pack(), Skipper> {

        Fraction_parser();

    private:
        start_type pack_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, std::vector<yrui::detail::Raw_fraction>(),
            skipper_type>
            fractions_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, yrui::detail::Raw_fraction(), skipper_type> fraction_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding,
            std::unordered_map<diplomacy::fraction_pair, diplomacy::Relation>(), skipper_type>
            relations_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, diplomacy::Relation(), skipper_type> relation_;
    };
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_FRACTION_PARSER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
