#include <type_traits>

#include "kernel/arch/architecture.h"
#include "kernel/common/randutil.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/legion.h"
#include "kernel/military/military_mgr.h"
#include "kernel/military/strategy.h"
#include "kernel/person/person.h"

//! \todo   modify log

namespace yrui {
namespace military {
    namespace detail {
        bool defensible_impl(Person* attacker, Person* defensor)
        {
            auto rnd = common::randint(0, 100);
            if (rnd <= 10) {
                return true;
            } else if (rnd <= 70) {
                return attacker->strategy_success_rate() < defensor->strategy_detect_rate();
            } else {
                return false;
            }
        }
    }

    namespace strategy {
        namespace {
            void strategy_attack(Military_mgr& mgr, Person* attacker, Person* defensor, Strategy_power power)
            {
                BOOST_ASSERT(attacker->has_troop() && !attacker->is_troop_crashed());
                BOOST_ASSERT(defensor->has_troop() && !defensor->is_troop_crashed());

                if (detail::defensible_impl(attacker, defensor)) {
                    mgr.strategy_detected(attacker, defensor);
                    YLOG_SEV(trace) << format("----[Military] %1% 的计谋被 %2% 识破了") % attacker->name()
                            % defensor->name();
                } else {
                    defensor->suffer_strategy(power);
                    YLOG_SEV(trace) << format("----[Military] %1% 中计了") % defensor->name();

                    if (defensor->is_troop_crashed()) {
                        YLOG_SEV(trace) << format("----[Military]%1% 使用计谋击破了 %2%") % attacker->name()
                                % defensor->name();

                        BOOST_ASSERT(!attacker->is_troop_crashed());
                        mgr.strategy_conquer(attacker, defensor);
                    }
                }
            }
        }

        bool Disturb::check_condition_impl_(target t) const { return true; }

        void Disturb::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            BOOST_ASSERT(attacker->has_troop() && !attacker->is_troop_crashed());
            for (auto defensor : t->all()) {
                BOOST_ASSERT(defensor->has_troop() && !defensor->is_troop_crashed());
                if (defensible(attacker, defensor)) {
                    mgr.strategy_detected(attacker, defensor);
                    YLOG_SEV(trace) << format("----[Military] %1% 的计谋被 %2% 识破了") % attacker->name()
                            % defensor->name();
                } else {
                    YLOG_SEV(trace) << format("----[Military] Nothing happened");
                }
            }
        }

        bool Raid::check_condition_impl_(target t) const { return t->num_available() >= 3; }

        void Raid::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            strategy_attack(mgr, attacker, t->frontier(), attacker->initiate_strategy(this));
        }

        bool Ambush::check_condition_impl_(target t) const { return t->num_available() >= 4; }

        void Ambush::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            auto power = attacker->initiate_strategy(this);

            auto defensors = t->frontiers(2);

            strategy_attack(mgr, attacker, defensors.front(), power);
            strategy_attack(mgr, attacker, defensors.back(), power);
        }

        bool Fire::check_condition_impl_(target t) const { return true; }

        void Fire::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            for (auto defensor : t->all()) {
                strategy_attack(mgr, attacker, defensor, attacker->initiate_strategy(this));
            }
        }

        bool Strike::check_condition_impl_(target t) const { return t->num_available() <= 3; }

        void Strike::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            BOOST_ASSERT(t->leader());

            strategy_attack(mgr, attacker, t->leader(), attacker->initiate_strategy(this));
        }

        ////////////////////////////////////////////////////////////////////
        bool Burn::check_condition_impl_(target t) const { return true; }

        void Burn::occur_impl_(Military_mgr& mgr, initiator attacker, target defensor) const
        {
            defensor->arch()->consume_food(attacker->initiate_strategy(this).val());
            YLOG_SEV(trace) << format("----[Military] %1% 使用火烧掉了 %2% 的粮草") % attacker->name()
                    % defensor->name();
        }

        bool Wreck::check_condition_impl_(target t) const { return true; }

        void Wreck::occur_impl_(Military_mgr& mgr, initiator attacker, target defensor) const
        {
            defensor->suffer_wreck(attacker->initiate_strategy(this));
            YLOG_SEV(trace) << format("----[Military] %1% 对 %2% 使用了坑道") % attacker->name() % defensor->name();
        }

        bool Traitor::check_condition_impl_(target defensor) const
        {
            const auto& ps = defensor->arch()->in_city();
            return std::any_of(ps.begin(), ps.end(), [](Person* p) { return p->loyalty() <= 3; });
        }

        void Traitor::occur_impl_(Military_mgr& mgr, initiator attacker, target defensor) const
        {
            defensor->out_of_defence();
            YLOG_SEV(trace) << format("----[Military] %1% 通过内奸攻陷了 %2%") % attacker->name() % defensor->name();
        }

        ////////////////////////////////////////////////////////////////////
        bool Bluff::check_condition_impl_(target t) const { return true; }

        void Bluff::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            auto defensor = t->leader();

            if (defensible(attacker, defensor)) {
                mgr.strategy_detected(attacker, defensor);
                YLOG_SEV(trace) << format("----[Military] %1% 的计谋被 %2% 识破了") % attacker->name()
                        % defensor->name();
            } else {
                t->retreat();
                YLOG_SEV(trace) << format("----[Military] %1% 中计退兵了") % defensor->name();
            }
        }

        bool Sneak::check_condition_impl_(target t) const { return t->num_available() >= 2; }

        void Sneak::occur_impl_(Military_mgr& mgr, initiator attacker, target t) const
        {
            strategy_attack(mgr, attacker, t->frontier(), attacker->initiate_strategy(this));
        }
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
