#ifndef YRUI_MILITARY_TROOP_H_
#define YRUI_MILITARY_TROOP_H_

#include <cstddef>
#include <vector>

#include "kernel/component_fwd.h"

namespace yrui {
namespace military {
    inline int required_food(int ntroops) noexcept { return ntroops / 10; }

    inline int required_money(int ntroops) noexcept { return ntroops / 20; }

    inline int max_supply(int money, int food) noexcept { return std::min(money * 20, food * 10); }

    int required_food(const std::vector<Person*>& troops);

    int required_money(const std::vector<Person*>& troops);
} // of namespace military
} // of namespace yrui

#endif // YRUI_MILITARY_TROOP_H_
