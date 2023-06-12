#include <boost/range/algorithm/count.hpp>
#include <boost/range/algorithm/find.hpp>

#include "kernel/common/randutil.h"
#include "kernel/core/timeline.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/legion.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

#include "kernel/impl/person_impl.h"

namespace Impl = yrui::person;

using boost::range::count;
using boost::range::find;

void Impl::Person_mgr::on_month_begin(Timeline& t)
{
    BOOST_LOG_NAMED_SCOPE("PersonMgr");
    MLOG() << "on month begin";
    for (auto p : appeared()) {
        p->on_month_begin(t);
    }
}

void Impl::Person_mgr::on_month_end(Timeline& t)
{
    BOOST_LOG_NAMED_SCOPE("PersonMgr");
    MLOG() << "on month end";

    for (auto p : appeared()) {
        p->on_month_end(t);
    }
}

//! 检查人物出现
void Impl::Person_mgr::on_year_begin(Timeline& t)
{
    if (!not_appeared_.empty()) {
        auto backup = not_appeared();

        BOOST_LOG_NAMED_SCOPE("CheckAppearrance");
        MLOG() << format("inspected %1% persons not appeared") % backup.size();

        for (auto p : backup) {
            LOG_ATTACH_ATTR(Pers, [p] { return p->name(); });

            if (p->showed_year() <= t.year() && common::hit_percent(60)) {
                BOOST_ASSERT(!p->is_active());

                //! 人物出现
                p->appear();

                //! check if the person need to join some fraction
                //! or do we need to move this to person::on_month_begin?
                if (auto rel = p->relative()) {
                    if (rel->is_active() && rel->is_in_office()) {
                        p->join(rel->stayed_city());
                    }
                }

                BOOST_ASSERT(p->is_active());
            }
        }
    }
}

//! 检查人物死亡
void Impl::Person_mgr::on_year_end(Timeline& t)
{
    BOOST_ASSERT(!persons_.empty());

    auto backup = appeared();

    BOOST_LOG_NAMED_SCOPE("CheckDeath");
    MLOG() << format("%1% appeared persons to be checked") % backup.size();

    for (auto p : backup) {
        LOG_ATTACH_ATTR(Pers, [p] { return p->name(); });

        BOOST_ASSERT(p->is_active());
        if (t.year() >= p->born_year() + p->lifespan() && common::rand_percent() <= 40) {

            p->die();

            BOOST_ASSERT(!p->fraction());
            BOOST_ASSERT(!p->has_troop());
            BOOST_ASSERT(!p->stayed_city());
            BOOST_ASSERT(!p->is_alive());

            MLOG() << "Person died";
        }
    }
}

yrui::Person* Impl::Person_mgr::get(id_t id) const
{
    BOOST_ASSERT(persons_.count(id));

    return persons_.find(id)->second;
}

void Impl::Person_mgr::load(std::vector<uptr<Person>> ps)
{
    BOOST_ASSERT(!ps.empty());

    MLOG() << format("Load %1% persons") % ps.size();

    person_vec_ = std::move(ps);

    persons_.clear();
    persons_.reserve(person_vec_.size());
    for (auto& pp : person_vec_) {
        auto p = pp.get();

        all_.push_back(p);
        persons_.insert({ p->id(), p });

        auto& raw = p->impl().raw_;
        if (raw.lifespan) {
            if (raw.stayed_city) {
                appeared_.push_back(p);
                if (raw.fraction) {
                    official_.push_back(p);
                } else {
                    out_of_office_.push_back(p);
                }
            } else {
                not_appeared_.push_back(p);
            }
        }
    }

    MLOG() << format("Persons appeared: %1%") % appeared_.size();
    MLOG() << format("Persons in office: %1%") % official_.size();
    MLOG() << format("Persons off office: %1%") % out_of_office_.size();
    MLOG() << format("Persons not appeared: %1%") % not_appeared_.size();
}

void Impl::Person_mgr::update() { }

bool Impl::Person_mgr::init_impl()
{
    BOOST_LOG_NAMED_SCOPE("InitPersonMgr");

    install_signals_();

    for (auto p : all_) {
        p->init_self();
    }

    return true;
}

void Impl::Person_mgr::finalize_impl() { }

bool Impl::Person_mgr::self_check() const { return {}; }

void Impl::Person_mgr::install_signals_()
{
    MLOG() << "Install signals";

    person_appear.connect([this](Person* p) {
        //! 加入系统管理
        appeared_.push_back(p);
        out_of_office_.push_back(p);
        not_appeared_.erase(find(not_appeared_, p));
    });

    person_die.connect([this](Person* p) {
        appeared_.erase(find(appeared_, p));

        auto iter = find(official_, p);
        if (iter != official_.end()) {
            official_.erase(iter);
        } else {
            BOOST_ASSERT(count(out_of_office_, p));
            out_of_office_.erase(find(out_of_office_, p));
        }
    });

    person_join_fraction.connect([this](Person* p, Fraction* f) {
        BOOST_ASSERT(p->is_in_office());
        BOOST_ASSERT(count(out_of_office_, p));
        BOOST_ASSERT(!count(official_, p));

        official_.push_back(p);
        out_of_office_.erase(find(out_of_office_, p));
    });

    person_leave_fraction.connect([this](Person* p, Fraction* f) {
        BOOST_ASSERT(!p->is_in_office());
        BOOST_ASSERT(count(official_, p));
        BOOST_ASSERT(!count(out_of_office_, p));

        official_.erase(find(official_, p));
        out_of_office_.push_back(p);
    });

    {
        auto& mgr = use_subsys<military::Military_mgr>();

        //! add military signals
        //! @{
        mgr.combat.connect(sig_level::mutual, [](Person* attacker, Person* defensor) {
            attacker->gain_fight_exp(1);
            defensor->gain_fight_exp(1);
            attacker->gain_leadership_exp(1);
            defensor->gain_leadership_exp(1);
        });

        mgr.defeat.connect(sig_level::mutual, [](Person* attacker, Person* defensor) {
            attacker->gain_fight_exp(3);
            attacker->gain_leadership_exp(3);
        });

        mgr.siege_attack.connect(sig_level::mutual, [](Person* attacker, Intruded_arch*) {
            attacker->gain_fight_exp(3);
            attacker->gain_leadership_exp(1);
        });

        mgr.siege_attack_success.connect(sig_level::mutual, [](Person* attacker, Intruded_arch*) {
            attacker->gain_fight_exp(5);
            attacker->gain_leadership_exp(1);
        });

        mgr.strategy_attack.connect(
            sig_level::mutual, [](Person* attacker, Strategy*) { attacker->gain_wisdom_exp(2); });

        mgr.strategy_conquer.connect(
            sig_level::mutual, [](Person* attacker, Person*) { attacker->gain_wisdom_exp(4); });

        mgr.strategy_detected.connect(
            sig_level::mutual, [](Person*, Person* defensor) { defensor->gain_wisdom_exp(4); });

        //! 士气及训练
        mgr.defeat.connect(sig_level::mutual, [](Person* winner, Person*) {
            if (winner->has_troop()) {
                winner->morale_up(10);
                winner->drill_up(5);
            }
        });

        mgr.siege_attack_success.connect(sig_level::mutual, [](Person* winner, Intruded_arch*) {
            if (winner->has_troop()) {
                winner->morale_up(10);
                winner->drill_up(5);
            }
        });
        //! @}

        //! add feat
        //! @{
        mgr.combat.connect(sig_level::mutual, [](Person* attacker, Person* defensor) {
            attacker->add_feat(1);
            defensor->add_feat(1);
        });

        mgr.defeat.connect(sig_level::mutual, [](Person* attacker, Person* defensor) { attacker->add_feat(5); });

        mgr.siege_attack.connect(sig_level::mutual, [](Person* attacker, Intruded_arch*) { attacker->add_feat(3); });

        mgr.siege_attack_success.connect(
            sig_level::mutual, [](Person* attacker, Intruded_arch*) { attacker->add_feat(10); });

        mgr.strategy_attack.connect(sig_level::mutual, [](Person* attacker, Strategy*) { attacker->add_feat(3); });

        mgr.strategy_conquer.connect(sig_level::mutual, [](Person* attacker, Person*) { attacker->add_feat(5); });

        mgr.battle_end.connect(sig_level::mutual, [](Legion* winner, Legion* loser) {
            for (auto p : winner->all_persons()) {
                p->add_feat(p->is_troop_combatable() ? 10 : 5);
            }
        });

        mgr.siege_success.connect(sig_level::mutual, [](Legion* attacker, Intruded_arch*) {
            for (auto p : attacker->all_persons()) {
                p->add_feat(p->is_troop_combatable() ? 20 : 10);
            }
        });

        mgr.siege_failed.connect(sig_level::mutual, [](Legion*, Intruded_arch* a) {
            for (auto p : a->arch()->in_city()) {
                p->add_feat(p->is_troop_combatable() ? 10 : 5);
            }
        });
        //! @}
    }

    {
        auto& mgr = use_subsys<fraction::Fraction_mgr>();

        mgr.fraction_died.connect(sig_level::observer, [](Fraction* f) {
            for (auto p : f->persons()) {
                p->on_fraction_die(f);
            }
        });

        mgr.succession.connect(sig_level::mutual, [](Fraction* f, Person* p) {
            BOOST_ASSERT(p->is_active());
            BOOST_ASSERT(p->fraction() == f);

            p->on_become_leader(f);
        });
    }
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
