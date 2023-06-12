#include "kernel/diplomacy/fraction_relation.h"

namespace std {
size_t hash<yrui::diplomacy::fraction_pair>::operator()(yrui::diplomacy::fraction_pair v) const
{
    auto x = v.first;
    auto y = v.second;
    auto len = sizeof(x) * 8 / 2;

    std::hash<yrui::id_t> hasher;
    return hasher((x << len) & y);
}
} // of namespace std
