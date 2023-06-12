#include "kernel/impl/person_impl.h"
#include "kernel/military/strategy.h"
#include "kernel/person/person.h"

namespace yrui {
namespace person {
    namespace {
        inline constexpr int adjust_drill(int x) { return 1 + (x - base_drill) / 20; }

        inline constexpr int adjust_morale(int x) { return 1 + (x - base_morale) / 20; }

        static_assert(adjust_drill(0) >= 1, "My poor math!");
        static_assert(adjust_morale(0) >= 1, "My poor math!");

        inline int pseduo_morale_loss(const Person& p, int attack) noexcept
        {
            return (1000 * attack) / (1 + p.morale() * p.controlled_troop());
        }
    }

    Attack_power Person::initiate_attack() const
    {
        BOOST_ASSERT(!is_troop_crashed());

        int val = adjust_drill(drill()) * adjust_morale(morale()) * attack().val() * (controlled_troop() / troop_unit);

        BOOST_ASSERT(val >= 0);

        return Attack_power { val };
    }

    void Person::suffer_attack(Attack_power power)
    {
        BOOST_ASSERT(!is_troop_crashed());

        impl().troop_loss(attack_loss_hint(power));
    }

    int Person::attack_loss_hint(Attack_power power) const noexcept
    {
        BOOST_ASSERT(!is_troop_crashed());

        const auto def
            = adjust_drill(drill()) * adjust_morale(morale()) * defence().val() * (controlled_troop() / troop_unit);

        //! 强制最低伤害
        return std::max((power - def).val(), min_loss);
    }

    bool Person::will_survive(Attack_power power) const noexcept
    {
        BOOST_ASSERT(!is_troop_crashed());

        auto hint_loss = attack_loss_hint(power);
        return hint_loss < troop() && morale() - pseduo_morale_loss(*this, hint_loss) > base_morale;
    }

    Strategy_power Person::initiate_strategy(const Strategy* str) const
    {
        int val = adjust_drill(drill()) * adjust_morale(morale()) / 2 * strategy_attack().val() * str->power();

        BOOST_ASSERT(val >= 0);

        return Strategy_power { val };
    }

    void Person::suffer_strategy(Strategy_power power)
    {
        BOOST_ASSERT(has_troop());
        const auto def = adjust_drill(drill()) * adjust_morale(morale()) / 2 * strategy_defence().val()
            * (controlled_troop() / troop_unit);

        impl().troop_loss(std::max((power - def).val(), min_loss));
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
