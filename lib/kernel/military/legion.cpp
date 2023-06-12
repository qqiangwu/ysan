#include <numeric>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm/count_if.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include "kernel/common/randutil.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/legion.h"
#include "kernel/military/military_mgr.h"
#include "kernel/military/military_support.h"
#include "kernel/person/person.h"

//! \todo   modify log

namespace yrui {
namespace military {
    using person::Person;

    Legion::Legion(const Troop_container& troops, Architecture* from, Architecture* target)
        : troops_(troops)
        , from_(from)
        , to_(target)
        , fraction_(from->fraction())
    {
        BOOST_ASSERT(troops_.size() <= troop_num);

        mgr_ = &use_subsys<Military_mgr>();
        logger_ = &mgr_->logger();

        //! 选出leader
        auto iter = std::find_if(troops_.begin(), troops_.end(), [](Person* p) { return p->is_leader(); });

        if (iter != troops_.end()) {
            std::iter_swap(iter, troops_.begin());
        } else {
            // 按位置来吧，如果都相等，那就按照功绩来
            std::sort(troops_.begin(), troops_.end(), [](Person* x, Person* y) {
                return x->position() > y->position() || (x->position() == y->position() && x->feat() > y->feat());
            });
        }

        leader_ = troops_.front();
    }

    Legion::~Legion()
    {
        if (!troops_.empty())
            reside_or_return();

        mgr_ = nullptr;
        logger_ = nullptr;
    }

    Military_mgr& Legion::mgr() const noexcept { return *mgr_; }

    ylogger::logger_type& Legion::logger() const noexcept { return *logger_; }

    void Legion::reside_or_return()
    {
        BOOST_LOG_NAMED_SCOPE("ResideOrReturn");

        if (fraction()->is_alive()) {
            if (to_ && to_->fraction() == fraction()) {
                for (auto p : troops_) {
                    p->reside(to_);
                }
            } else {
                for (auto p : troops_) {
                    p->go_back();
                }
            }
        }

        troops_.clear();
    }

    const std::string& Legion::name() const
    {
        BOOST_ASSERT(fraction());
        return fraction()->name();
    }

    Person* Legion::leader() const { return leader_; }

    Fraction* Legion::fraction() const { return fraction_; }

    bool Legion::is_alive() const
    {
        BOOST_ASSERT(leader());
        return !troops_.empty() && !leader()->is_troop_crashed();
    }

    ///////////////////////////////////////////////////////////////////////
    int Legion::troop() const
    {
        return std::accumulate(troops_.begin(), troops_.end(), 0,
            [](int acc, Person* p) { return acc + (p->is_troop_crashed() ? 0 : p->troop()); });
    }

    //! 返回一支在最前的军队
    Person* Legion::frontier() const
    {
        BOOST_ASSERT(is_alive());

        auto iter = troops_.rbegin();
        while ((*iter)->is_troop_crashed()) {
            ++iter;
        }

        BOOST_ASSERT(iter != troops_.rend());

        return *iter;
    }

    //! 返回多支在最前的军队
    Legion::Troop_container Legion::frontiers(int size) const
    {
        BOOST_ASSERT(size < int(troops_.size()));

        auto tmp = all();
        BOOST_ASSERT(size < int(tmp.size()));

        return std::vector<Person*>(tmp.end() - size, tmp.end());
    }

    std::pair<Person*, Person*> Legion::strategy_troops() const { return { strategy_1_, strategy_2_ }; }

    Legion::Troop_container Legion::all() const
    {
        using boost::adaptors::filtered;
        using boost::range::push_back;

        Troop_container ret;

        auto alived = [](Person* p) { return !p->is_troop_crashed(); };

        push_back(ret, troops_ | filtered(alived));

        return ret;
    }

    const Legion::Troop_container& Legion::all_persons() const noexcept { return troops_; }

    Legion::Troop_container::size_type Legion::num_available() const
    {
        using boost::range::count_if;

        return count_if(troops_, [](Person* p) { return !p->is_troop_crashed(); });
    }

    ///////////////////////////////////////////////////////////////////////
    Fraction_controller* Legion::controller() const
    {
        BOOST_ASSERT(fraction());
        return fraction()->controller();
    }

    void Legion::prepare_attack() { state_ = State::attack; }

    void Legion::prepare_defense() { state_ = State::defence; }

    void Legion::retreat()
    {
        if (strategy_1_)
            troops_.push_back(strategy_1_);
        if (strategy_2_)
            troops_.push_back(strategy_2_);

        for (auto p : troops_) {
            p->go_back();
        }

        strategy_1_ = nullptr;
        strategy_2_ = nullptr;

        troops_.clear();
    }

    ///////////////////////////////////////////////////////////////////////
    void Legion::on_day_begin() { BOOST_ASSERT(state_ == State::attack); }

    void Legion::on_day_end() { state_ = State::attack; }

    //! 出战军队
    bool Legion::is_waging() const { return state_ == State::attack; }

    namespace {
        inline void remove_troop(Legion::Troop_container& c, Person* p) { c.erase(std::find(c.begin(), c.end(), p)); }

        inline bool prepare_troops(std::vector<Person*>& troops, const std::vector<Person*>& source)
        {
            if (!source.empty()) {
                troops = source;
                return true;
            }
            return false;
        }
    }

    void Legion::attack(Military_mgr& mgr, Intruded_arch* defensor)
    {
        BOOST_ASSERT(is_alive());
        BOOST_ASSERT(is_waging());
        BOOST_ASSERT(!defensor->is_conquered());

        auto atk_stack = all();
        while (!atk_stack.empty() && is_alive()) {
            auto atk_troop = atk_stack.back();

            auto atk = atk_troop->initiate_attack();
            auto def = defensor->suffer_attack(atk);
            atk_troop->suffer_attack(def);

            mgr.siege_attack(atk_troop, defensor);

            MLOG() << format("%1% attack city %2%") % atk_troop->name() % defensor->name();

            if (atk_troop->is_troop_crashed()) {
                mgr.siege_attack_failed(atk_troop, defensor);

                MLOG() << format("%1% attack %2% failed") % atk_troop->name() % defensor->name();
            } else if (defensor->is_conquered()) {
                mgr.siege_attack_success(atk_troop, defensor);

                MLOG() << format("%1% conquered %2%") % atk_troop->name() % defensor->name();

                break;
            }

            atk_stack.pop_back();
        }
    }

    void Legion::attack(Military_mgr& mgr, Legion* enemy)
    {
        BOOST_ASSERT(state_ == State::attack);
        BOOST_ASSERT(is_alive());
        BOOST_ASSERT(enemy->is_alive());

        auto atk_stack = all();
        auto def_stack = enemy->all();

        int turn = std::max(atk_stack.size(), def_stack.size());
        while (--turn >= 0 && is_alive() && enemy->is_alive()) {
            if (atk_stack.empty()) {
                if (!prepare_troops(atk_stack, all())) {
                    break;
                }
            } else if (def_stack.empty()) {
                if (!prepare_troops(def_stack, enemy->all())) {
                    break;
                }
            }

            BOOST_ASSERT(!atk_stack.empty() && !def_stack.empty());

            auto atk_troop = atk_stack.back();
            auto def_troop = def_stack.back();

            MLOG() << format("%1% of %2% 军 --> %3% of %4% 军") % atk_troop->name() % name() % def_troop->name()
                    % enemy->name();

            BOOST_ASSERT(!atk_troop->is_troop_crashed());
            BOOST_ASSERT(!def_troop->is_troop_crashed());

            Military_support::combat(atk_troop, def_troop, enemy->state_ == State::defence);

            mgr.combat(atk_troop, def_troop);

            BOOST_ASSERT_MSG(!(atk_troop->is_troop_crashed() && def_troop->is_troop_crashed()),
                "Two troops will never crashed simutaneously");

            if (def_troop->is_troop_crashed()) {
                BOOST_ASSERT(!atk_troop->is_troop_crashed());

                mgr.defeat(atk_troop, def_troop);

                MLOG() << format("%1% of %2% defeat %3% of %4%") % atk_troop->name() % name() % def_troop->name()
                        % enemy->name();
            } else if (atk_troop->is_troop_crashed()) {
                BOOST_ASSERT(!def_troop->is_troop_crashed());

                mgr.defeat(def_troop, atk_troop);

                MLOG() << format("%1% of %2% defeat %3% of %4%") % def_troop->name() % enemy->name() % atk_troop->name()
                        % name();
            }

            atk_stack.pop_back();
            def_stack.pop_back();
        }
    }

    namespace {
        std::pair<Person*, Person*> prepare_strategy_troops(Legion::Troop_container& troops)
        {
            BOOST_ASSERT(!troops.empty());

            //! leader will not considered
            std::sort(troops.begin() + 1, troops.end(),
                [](Person* x, Person* y) { return x->wisdom().val() < y->wisdom().val(); });

            switch (troops.size()) {
            case 2:
            case 3:
            case 4:
                return { troops.back(), nullptr };

            case 5:
            case 6:
            case 7:
                return { troops[troops.size() - 1], troops[troops.size() - 2] };

            default:
                return {};
            }
        }
    }

    void Legion::on_battle_start(Military_mgr& mgr)
    {
        MLOG() << format("Legion %1% on battle start") % name();

        BOOST_ASSERT(is_alive());

        //! decide whether to use strategy
        auto ctrl = controller();
        auto candidate = prepare_strategy_troops(troops_);

        //! remove from battle troops
        if (candidate.first && ctrl->permit_strategy(candidate.first)) {
            BOOST_ASSERT(!candidate.first->is_troop_crashed());

            strategy_1_ = candidate.first;
            remove_troop(troops_, strategy_1_);

            mgr.become_strategy_troop(strategy_1_);

            MLOG() << format("%1% prepare to use strategy") % strategy_1_->name();
        }

        if (candidate.second && ctrl->permit_strategy(candidate.second)) {
            BOOST_ASSERT(!candidate.second->is_troop_crashed());

            strategy_2_ = candidate.second;
            remove_troop(troops_, strategy_2_);

            mgr.become_strategy_troop(strategy_2_);

            MLOG() << format("%1% prepare to use strategy") % strategy_2_->name();
        }

        BOOST_ASSERT(strategy_1_ != leader());
        BOOST_ASSERT(strategy_2_ != leader());
    }

    void Legion::on_battle_end(Military_mgr& mgr)
    {
        if (strategy_1_)
            troops_.push_back(strategy_1_);
        if (strategy_2_)
            troops_.push_back(strategy_2_);

        strategy_1_ = strategy_2_ = nullptr;

        if (!is_alive())
            retreat();
    }

    ////////////////////////////////////////////////////////////////////////
    //! \todo   Install related signals
    void Legion::take_control_of(Intruded_arch* iarch)
    {
        BOOST_LOG_NAMED_SCOPE("TakeControlOf");

        auto arch = iarch->arch();

        MLOG() << format("Legion %1% take control of %2%") % name() % iarch->name();

        if (iarch->is_out_of_defence()) {
            MLOG() << format("%1% is out of defence") % iarch->name();
        } else if (arch->steady() == 0) {
            MLOG() << format("%1% steady reduced to 0") % arch->name();
        } else if (arch->food() == 0) {
            MLOG() << format("%1% ran out of food") % arch->name();
        } else if (arch->num_in_city() == 0) {
            MLOG() << format("%1% has on persons") % arch->name();
        } else if (arch->troop() == 0) {
            MLOG() << format("%1% has no troop") % arch->name();
        }

        /*!
         * 这里，我们只能处理in_city的人员。对于那些出外办事（比如在出生部队中）的人，应该
         * 怎么办？不管了，我认为Person自身会处理好这一切的。
         */
        auto this_frac = fraction();

        std::vector<Person*> captives { arch->in_city().begin(), arch->in_city().end() };

        for (auto captive : captives) {
            captive->captured_by(this_frac);
        }

        arch->transfer_to(this_frac);

        //! delayed the process of leader
        using person::Person;
        auto iter_leader = std::find_if(begin(captives), end(captives), std::mem_fn(&Person::is_leader));
        if (iter_leader != end(captives)) {
            iter_swap(iter_leader, end(captives) - 1);
        }

        for (auto captive : captives) {
            deal_captive(captive);
        }

        reside_or_return();
    }

    void Legion::deal_captive(Person* p)
    {
        MLOG() << "Capture " << p->name();

        //! 是直接在persuade里面改呢，还是另外改呢？
        if (p->is_persuaded_by(fraction())) {
            MLOG() << "Persuaded " << p->name();
            //! \todo   添加劝降成功信号
            p->capitulate(fraction());
        } else {
            //! 否则，让ai决定如何处理
            //! 可选的方法有：释放，处斩
            //! 技术问题，暂时不允许处斩君主？
            //! \todo
            if (common::hit_percent(20)) {
                //! what if p is the leader?
                p->killed_by(fraction());
            } else {
                //! release the person
                p->release(fraction());
            }
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
