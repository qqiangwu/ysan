#ifndef YRUI_DIPLOMACY_FRACTION_RELATION_H_
#define YRUI_DIPLOMACY_FRACTION_RELATION_H_

#include "kernel/config.h"
#include <functional>
#include <unordered_map>
#include <utility>

namespace yrui {
namespace diplomacy {
    enum class Relation_state { normal, in_war, alliance };

    using fraction_pair = std::pair<id_t, id_t>;

    struct Relation {
        int friendliness;
        Relation_state relation;
    };

    using Relation_table = std::unordered_map<yrui::diplomacy::fraction_pair, yrui::diplomacy::Relation>;
} // of namespace diplomacy
} // of namespace yrui

namespace std {
template <> struct hash<yrui::diplomacy::fraction_pair> {
    size_t operator()(yrui::diplomacy::fraction_pair v) const;
};
} // of namespace std

#endif // !YRUI_DIPLOMACY_FRACTION_RELATION_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
