#ifndef YRUI_SERIALIZATION_PERSON_PARSER_H_
#define YRUI_SERIALIZATION_PERSON_PARSER_H_

/*!
 *
 * \file     Serialization/Parser/person_parser.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    parse the database file to raw person
 * \ingroup  Serialization
 *
 */

#include <boost/spirit/include/qi.hpp>

#include "kernel/impl/serialization/adapt_person.h"
#include "kernel/impl/serialization/parser/config.h"
#include "kernel/impl/serialization/parser/skipper.h"

namespace yrui {
namespace serialization {
    struct Person_parser : boost::spirit::qi::grammar<parser_iterator, unicode_encoding,
                               std::vector<yrui::detail::Raw_person>(), Skipper> {

        Person_parser();

    private:
        start_type persons_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, yrui::detail::Raw_person(), skipper_type> person_;

        boost::spirit::qi::rule<iterator_type, unicode_encoding, std::vector<yrui::id_t>(), skipper_type> fixed_skills_;
    };
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_PERSON_PARSER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
