#include "kernel/arch/architecture.h"
#include "kernel/military/legion.h"
#include "kernel/military/troop.h"
#include "kernel/person/person.h"

//! \todo       Add log

namespace yrui {
namespace arch {
    int Architecture::suffer_siege()
    {
        const auto consume = military::required_food(troop());

        consume_food(consume);

        //! 降低武将忠诚度
        for (auto p : in_city()) {
            if (!p->is_leader()) {
                // p->loyalty(std::max<int>(p->loyalty() - 1, 0));
                //! \todo   handling the signal
            }
        }

        //! 降低城池繁荣度

        return consume;
    }

    int Architecture::suffer_wreck(Strategy_power power)
    {
        auto old = steady();
        auto adjust = mayor() ? power - mayor()->strategy_defence().val() * 100 : power;

        const auto adj = adjust.val() / 10;
        set_steady(adj > int(steady()) ? 0 : steady() - adj);

        return old - steady();
    }

    Attack_power Architecture::suffer_attack(Attack_power attack_strength)
    {
        Attack_power ret((troop() / troop_unit) * steady());

        int old = steady();

        //! fix attack city
        old -= std::max(attack_strength.val() / 300, 1);

        //! 消减兵力
        for (auto p : in_city()) {
            if (!p->is_troop_crashed()) {
                BOOST_ASSERT(p->has_troop());
                p->suffer_attack(Attack_power { attack_strength.val() / 10 });
            }
        }

        set_steady(old > 0 ? old : 0);

        return ret;
    }

    bool Architecture::is_conquered() const noexcept
    {
        return steady() == 0 || food() == 0 || troop() == 0 || in_city().empty();
    }
}
} // of namespace yrui::arch
