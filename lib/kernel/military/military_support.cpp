#include "kernel/military/military_support.h"
#include "kernel/person/person.h"

namespace ML = yrui::military;

//! \todo   modify log

void ML::Military_support::combat(Person* attacker, Person* defensor, bool is_defence)
{
    BOOST_ASSERT(!attacker->is_troop_crashed());
    BOOST_ASSERT(!defensor->is_troop_crashed());

    YLOG() << format("%1% [%2%: M %3% | D %4%] <-> "
                     "%5% [%6%: M %7% | D %8%]")
            % attacker->name() % attacker->troop() % attacker->morale() % attacker->drill() % defensor->name()
            % defensor->troop() % defensor->morale() % defensor->drill();

    auto atk = attacker->initiate_attack();
    auto def = defensor->initiate_attack();

    //! 一方处于防御时，受到攻击减弱
    if (is_defence) {
        //! adjust attack power
        atk = Attack_power(atk.val() / 2);
        def = Attack_power(def.val() / 2);
    }

    YLOG() << format("Expect loss: %1% <-> %2%") % attacker->attack_loss_hint(def) % defensor->attack_loss_hint(atk);

    //! 减弱威力
    while (atk && def && !attacker->will_survive(def) && !defensor->will_survive(atk)) {
        atk = Attack_power(atk.val() * 2 / 3);
        def = Attack_power(def.val() * 2 / 3);
    }

    YLOG() << format("Actual loss: %1% <-> %2%") % attacker->attack_loss_hint(def) % defensor->attack_loss_hint(atk);

    attacker->suffer_attack(def);
    defensor->suffer_attack(atk);

    /*
    BOOST_ASSERT_MSG(
                !(attacker->is_troop_crashed() && defensor->is_troop_crashed()),
                "Two troops cannot crash in the same time"
                );
    */
    if (attacker->is_troop_crashed() && defensor->is_troop_crashed()) {
        BOOST_ASSERT(attacker->has_troop() || defensor->has_troop());
        if (attacker->troop() > defensor->troop()) {
            attacker->morale_up(10);
        } else {
            defensor->morale_up(10);
        }
    }

    if (attacker->is_troop_crashed()) {
        BOOST_ASSERT_MSG(!defensor->is_troop_crashed(), "Two troops cannot crash in the same time");
        defensor->morale_up(10);
    } else if (defensor->is_troop_crashed()) {
        BOOST_ASSERT_MSG(!attacker->is_troop_crashed(), "Two troops cannot crash in the same time");
        attacker->morale_up(10);
    }

    YLOG() << format("%1% [%2%: M %3% | D %4%] <-> %5% [%6%: M %7% | D %8%]") % attacker->name() % attacker->troop()
            % attacker->morale() % attacker->drill() % defensor->name() % defensor->troop() % defensor->morale()
            % defensor->drill();
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
