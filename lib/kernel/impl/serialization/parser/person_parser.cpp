#include "kernel/impl/serialization/parser/person_parser.h"
#include "kernel/impl/serialization/parser/id_parser.h"

namespace yrui {
namespace serialization {
    Person_parser::Person_parser()
        : base_type(persons_)
    {
        using namespace boost::spirit;
        using namespace qi;

        boost::spirit::qi::unicode::char_type char_;
        lexeme_type lexeme;
        lit_type lit;
        qi::int_type int_;

        persons_ %= *person_;
        person_ %= id_ //! id_t
            > lexeme[+(char_ - ' ')] //! name
            > int_ //! born year
            > int_ //! show year
            > int_ //! lifespan

            > id_ //! show city
            > id_ //! relative
            > id_ //! fraction

            > int_ //! fight
            > int_ //! leadership
            > int_ //! wisdom
            > int_ //! politics

            > int_ //! potential

            > int_ //! fight exp
            > int_ //! leadership exp
            > int_ //! wisdom exp
            > int_ //! politics exp

            > int_ //! loyalty
            > int_ //! max loyalty

            > int_ //! troop
            > int_ //! max troop
            > int_ //! morale
            > int_ //! drill

            > id_ //! stayed city
            > int_ //! feat
            > int_ //! position

            > fixed_skills_ //! fix abilities
            ;

        fixed_skills_ %= lit('{') > *id_ > lit('}');

        BOOST_SPIRIT_DEBUG_NODES((persons_)(person_)(fixed_skills_));
    }
} // of namespace serialization
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
