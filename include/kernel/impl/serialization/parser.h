#ifndef YRUI_SERIALIZATION_PARSER_H_
#define YRUI_SERIALIZATION_PARSER_H_

#include <boost/spirit/home/qi/operator/expect.hpp>

#include <sstream>

#include "kernel/config.h"

#include "kernel/impl/serialization/parser/architecture_parser.h"
#include "kernel/impl/serialization/parser/fraction_parser.h"
#include "kernel/impl/serialization/parser/map_parser.h"
#include "kernel/impl/serialization/parser/person_parser.h"

namespace yrui {
namespace serialization {
    template <class Parser> using attr_t = typename Parser::start_type::attr_type;

    using Parser_error = boost::spirit::qi::expectation_failure<yrui::serialization::parser_iterator>;

    template <class Parser> attr_t<Parser> read_db(const std::string& db, const Parser& p)
    {
        attr_t<Parser> attr;

        auto first = db.begin();
        auto last = db.end();

        Skipper skipper;

        try {
            boost::spirit::qi::phrase_parse(first, last, p, skipper, attr);
        } catch (Parser_error& e) {
            std::ostringstream out;
            out << "Parser error: "
                << " " << e.what() << std::string(e.first, e.last);
            YLOG_SEV(fatal) << out.str();
        } catch (std::exception& e) {
            YLOG_SEV(fatal) << "Unknown exception in parsing: " << e.what();
        }

        if (first != last) {
            auto line = 1 + std::count(db.begin(), first, '\n');
            auto parsed = std::string(db.begin(), first);
            auto msg = format("Failed at line %1%: %2%") % line % parsed;

            YLOG_SEV(fatal) << msg.str();

            throw std::runtime_error(msg.str());
        }

        return attr;
    }
} // of namespace serialization
} // of namespace yrui

#endif // !YRUI_SERIALIZATION_PARSER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
