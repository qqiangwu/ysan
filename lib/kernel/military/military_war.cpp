#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include "kernel/arch/architecture.h"
#include "kernel/common/common_util.h"
#include "kernel/common/randutil.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"

//! \todo   modify log

namespace yrui {
namespace military {
    namespace {
        //! troop attack arch
        bool try_strategy_attack(Military_mgr& mgr, Person* attacker, Intruded_arch* defensor)
        {
            BOOST_LOG_NAMED_SCOPE("StrategyAttArch");
            if (attacker) {
                //! choose a strategy
                NLOG(mgr.logger()) << format("%1% try strategy attack arch") % attacker->name();
                mgr.strategy_prepare(attacker);

                auto strs = mgr.arch_strategies();
                auto x = common::randint(0, strs.size() - 1);
                auto& str = strs.begin()[x];
                if (common::hit_percent(2 * str->probability()) && str->condition_suitable(defensor)) {
                    NLOG(mgr.logger()) << format("%1% prepare strategy %2%") % attacker->name() % str->name();
                    mgr.strategy_attack(attacker, str.get());
                    str->occur(mgr, attacker, defensor);
                }
            }

            return !defensor->is_conquered();
        }

        //! arch attack troop
        bool try_strategy_attack(Military_mgr& mgr, Person* attacker, Legion* defensor)
        {
            BOOST_LOG_NAMED_SCOPE("StrategyAttLegion");
            if (attacker && attacker->has_troop() && !attacker->is_troop_crashed()) {
                //! choose a strategy
                NLOG(mgr.logger()) << format("%1% in arch try strategy attack") % attacker->name();

                mgr.strategy_prepare(attacker);

                auto strs = mgr.defence_strategies();
                auto x = common::randint(0, strs.size() - 1);
                auto& str = strs.begin()[x];
                if (common::hit_percent(2 * str->probability()) && str->condition_suitable(defensor)) {
                    NLOG(mgr.logger()) << format("%1% prepare strategy %2%") % attacker->name() % str->name();
                    mgr.strategy_attack(attacker, str.get());
                    str->occur(mgr, attacker, defensor);
                }
            }

            return defensor->is_alive();
        }
    }

    bool Military_mgr::start_seige_(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_LOG_NAMED_SCOPE("ArchWar");
        MLOG() << format("%1% seige city %2%") % attacker->name() % defensor->name();

        siege_start(attacker, defensor);

        attacker->on_battle_start(*this);
        defensor->on_battle_start(*this);

        auto left_turn = map().max_turn(attacker->from(), attacker->fraction()->capital());
        auto atk_ctrl = attacker->controller();

        auto att_str = attacker->strategy_troops();
        auto def_str = defensor->strategy_troops();

        while (left_turn-- > 0 && !defensor->is_conquered() && attacker->is_alive()) {
            MLOG() << format("New turn of siege %1% -> %2%") % attacker->name() % defensor->name();

            attacker->on_day_begin();
            defensor->on_day_begin();

            atk_ctrl->command_troop(attacker, defensor);

            //! 攻击方可能选择退兵
            if (!attacker->is_alive())
                break;

            if (!(try_strategy_attack(*this, att_str.first, defensor)
                    && try_strategy_attack(*this, def_str.first, attacker)
                    && try_strategy_attack(*this, att_str.second, defensor)
                    && try_strategy_attack(*this, def_str.second, attacker))) {
                BOOST_ASSERT(!attacker->is_alive());
                break;
            }

            BOOST_ASSERT(!defensor->is_conquered());
            BOOST_ASSERT(attacker->is_alive());

            if (attacker->is_waging()) {
                attacker->attack(*this, defensor);
            } else {
                defensor->suffer_seige();

                siege_wait(attacker, defensor);

                MLOG() << format("%1% wait in %2%") % attacker->name() % defensor->name();
            }

            defensor->on_day_end();
            attacker->on_day_end();
        }

        if (defensor->is_conquered() && attacker->is_alive()) {
            BOOST_ASSERT(attacker->is_alive());

            siege_success(attacker, defensor);

            MLOG() << format("%1% 攻陷了 城池 %2%") % attacker->name() % defensor->name();
        } else {
            //! failed either because attacker is died or run out of food
            siege_failed(attacker, defensor);

            MLOG() << format("%1% 攻打城池 %2% 失败") % attacker->name() % defensor->name();
        }

        defensor->on_battle_end(*this);
        attacker->on_battle_end(*this);

        siege_end(attacker, defensor);

        return defensor->is_conquered() && attacker->is_alive();
    }

    namespace {
        //! troop attack troop
        //! \return     whether target Legion crashed
        bool try_strategy_attack_2(Military_mgr& mgr, Person* attacker, Legion* defensor)
        {
            if (attacker) {
                //! choose a strategy
                YLOG_SEV(trace) << format("----[Military]%1% try strategy attack") % attacker->name();

                mgr.strategy_prepare(attacker);

                //! pick a strategy randomly
                auto strs = mgr.troop_strategies();
                auto x = common::randint(0, strs.size() - 1);
                auto& str = strs.begin()[x];

                //! check whether it is occured
                if (common::hit_percent(str->probability() * 2) && str->condition_suitable(defensor)) {
                    YLOG_SEV(trace) << format("----[Military]%1% prepare strategy %2%") % attacker->name()
                            % str->name();
                    mgr.strategy_attack(attacker, str.get());
                    str->occur(mgr, attacker, defensor);
                }
            }

            return defensor->is_alive();
        }
    }

    bool Military_mgr::start_combat_(Legion* attacker, Legion* defensor)
    {
        BOOST_LOG_NAMED_SCOPE("FieldWar");
        MLOG() << format("Start battle %1% vs %2%") % attacker->name() % defensor->name();

        battle_start(attacker, defensor);

        auto atk_ctrl = attacker->controller();
        auto def_ctrl = defensor->controller();

        MLOG() << "Ask whether use strategies";
        attacker->on_battle_start(*this);
        defensor->on_battle_start(*this);

        auto att_str = attacker->strategy_troops();
        auto def_str = defensor->strategy_troops();

        int left_turn = map().max_turn(attacker->from(), attacker->fraction()->capital());

        while (left_turn-- > 0 && attacker->is_alive() && defensor->is_alive()) {
            MLOG() << format("New turn of battle %1% X %2%") % attacker->name() % defensor->name();

            attacker->on_day_begin();
            defensor->on_day_begin();

            //--------------------------------------------------------
            atk_ctrl->command_troop(attacker, defensor);
            def_ctrl->command_troop(defensor, attacker);

            //! 决策中可能退兵
            if (!attacker->is_alive() || !defensor->is_alive())
                break;

            //--------------------------------------------------------
            //! 计谋结束时可能出现军队失败
            //! strategy: 最好是按照某种顺序来发动计谋
            if (!(try_strategy_attack_2(*this, att_str.first, defensor)
                    && try_strategy_attack_2(*this, def_str.first, attacker)
                    && try_strategy_attack_2(*this, att_str.second, defensor)
                    && try_strategy_attack_2(*this, def_str.second, attacker))) {
                BOOST_ASSERT(!attacker->is_alive() || !defensor->is_alive());
                break;
            }

            if (attacker->is_waging()) {
                attacker->attack(*this, defensor);
            } else if (defensor->is_waging()) {
                defensor->attack(*this, attacker);
            }

            defensor->on_day_end();
            attacker->on_day_end();
        }

        //! only one survives
        BOOST_ASSERT(left_turn == -1 || attacker->is_alive() != defensor->is_alive());

        auto attacker_win = (left_turn != -1 && attacker->is_alive()) || (left_turn == -1 && !defensor->is_alive());

        auto winner = attacker_win ? attacker : defensor;
        auto loser = attacker_win ? defensor : attacker;

        BOOST_ASSERT(winner != loser);
        BOOST_ASSERT(winner && loser);

        defensor->on_battle_end(*this);
        attacker->on_battle_end(*this);

        battle_end(winner, loser);

        MLOG() << format("%1% win %2%") % winner->name() % loser->name();

        return attacker_win;
    }

    ///////////////////////////////////////////////////////////////////////
    namespace {
        struct Path {
            typedef const std::pair<Architecture*, Architecture*>& rep_type;

            Path(rep_type x)
                : p_(x)
            {
            }

            Architecture* from() const { return p_.first; }
            Architecture* to() const { return p_.second; }

            operator rep_type() const { return p_; }

        private:
            rep_type p_;
        };
    }

    void Military_mgr::schedule_battle_()
    {
        BOOST_LOG_NAMED_SCOPE("Schedule");

        std::vector<Legion*> siege_legions;
        std::map<std::pair<Architecture*, Architecture*>, std::pair<Legion*, Legion*>> fields;

        for (auto item : legions_) {
            auto path = Path(item.first);
            auto legion = item.second;
            auto other = legions_.find({ path.to(), path.from() });
            if (other != legions_.end()) {
                // 野战部队
                if (path.from() < path.to()) {
                    fields.insert({ path, { legion, other->second } });
                } else {
                    fields.insert({ other->first, { other->second, legion } });
                }
            } else {
                // 攻城部队
                siege_legions.push_back(legion);
            }
        }

        std::vector<Legion*> deferred_siege;

        //! 攻城战
        for (auto legion : siege_legions) {
            MLOG() << format("%1% is attacked by %2%") % legion->to()->name() % legion->from()->name();

            emit_arch_intruded(legion->to(), legion);

            if (auto defensor = common::get_controller(legion->to())) {
                MLOG() << format("%1% prepared to combat[W:%2%|U:%3%]") % legion->to()->name()
                        % legion->to()->num_worked() % legion->to()->num_unworked();

                if (auto troop = defensor->defend(legion)) {
                    MLOG() << format("Leader: %1% | %2% persons [Troop: %3%]") % troop->leader()->name()
                            % troop->num_available() % troop->troop();

                    fields.insert({ { legion->from(), legion->to() }, { legion, troop.get() } });

                    legion_cache_.push_back(std::move(troop));

                    continue;
                }
            }

            MLOG() << format("%1% does not defend") % legion->to()->name();
            deferred_siege.push_back(legion);
        }

        //! 野战
        std::vector<Legion*> winners;

        for (auto item : fields) {
            auto attacker = item.second.first;
            auto defensor = item.second.second;

            auto winner = start_combat_(attacker, defensor) ? attacker : defensor;

            if (winner == attacker) {
                defensor->reside_or_return();
            } else {
                attacker->reside_or_return();
            }

            if (winner->to()) {
                winners.push_back(winner);
            }
        }

        fields.clear();

        boost::range::push_back(deferred_siege, winners);

        //! \fixme
        //! 城战
        for (auto legion : deferred_siege) {
            if (legion->fraction()->is_alive()) {
                Intruded_arch defensor(legion->to(), legion);

                if (!defensor.is_conquered()) {
                    start_seige_(legion, &defensor);
                } else {
                    siege_success(legion, &defensor);
                }

                if (defensor.is_conquered()) {
                    legion->take_control_of(&defensor);
                }
            }
        }
    }
} // of namespace military
} // of namespace yrui
