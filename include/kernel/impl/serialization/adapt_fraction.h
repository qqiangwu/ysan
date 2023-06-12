#ifndef YRUI_SERIALIZATION_RAW_FRACTION_
#define YRUI_SERIALIZATION_RAW_FRACTION_

#include <boost/fusion/include/adapt_struct.hpp>
#include <unordered_map>
#include <vector>

#include <kernel/detail/raw_fraction.h>
#include <kernel/diplomacy/fraction_relation.h>

namespace yrui {
namespace serialization {
    struct Fraction_pack {
        std::vector<yrui::detail::Raw_fraction> raw_fraction;
        yrui::diplomacy::Relation_table rel;
    };
} // of namespace serialization
} // of namespace yrui

BOOST_FUSION_ADAPT_STRUCT(yrui::detail::Raw_fraction,
    (yrui::id_t, id)(yrui::id_t, leader)(yrui::id_t, capital)(bool, has_seal)(bool, has_emperor)(int, money)(int, food))

BOOST_FUSION_ADAPT_STRUCT(yrui::serialization::Fraction_pack,
    (std::vector<yrui::detail::Raw_fraction>, raw_fraction)(yrui::diplomacy::Relation_table, rel))

BOOST_FUSION_ADAPT_STRUCT(yrui::diplomacy::Relation, (int, friendliness)(yrui::diplomacy::Relation_state, relation))
#endif // !YRUI_SERIALIZATION_RAW_FRACTION_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
