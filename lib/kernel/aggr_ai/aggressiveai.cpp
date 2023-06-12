#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

#include "kernel/aggr_ai/aggressiveai.h"

namespace Fai {
Aggressive_ai::~Aggressive_ai() { unregister_slots(); }

void Aggressive_ai::unregister_slots()
{
    if (unregister_) {
        unregister_();
        unregister_ = nullptr;
    }
}

void Aggressive_ai::on_get_city(Architecture* arch)
{
    YPRE_CONDITION(self_ == arch->fraction());
    YLOG() << format("%1% OnGetCity %2%") % self_->name() % arch->name();

    cities_obtained_.insert(arch);
}

void Aggressive_ai::on_lose_city(Architecture* arch)
{
    YLOG() << format("%1% OnLoseCity %2%") % self_->name() % arch->name();

    cities_losed_.insert(arch);
}

void Aggressive_ai::on_get_person(Person* p)
{
    YPRE_CONDITION(self_ == p->fraction());
    YLOG() << format("%1% on-get-person %2%") % self_->name() % p->name();

    persons_obtained_.insert(p);
}

void Aggressive_ai::on_lose_person(Person* p)
{
    YLOG() << format("%1% on-lose-person %2%") % self_->name() % p->name();

    persons_losed_.insert(p);
}

void Aggressive_ai::pre_work(const Timeline& tl)
{
    // confirm obtains and lost archs and persons
    if (!cities_obtained_.empty()) {
        auto backup = cities_obtained_;

        for (auto arch : backup) {
            if (arch->fraction() != self_) {
                cities_obtained_.erase(arch);
            }
        }
    }

    if (!cities_losed_.empty()) {
        auto backup = cities_losed_;

        for (auto arch : backup) {
            if (arch->fraction() == self_) {
                cities_losed_.erase(arch);
            }
        }
    }

    if (!persons_obtained_.empty()) {
        auto backup = persons_obtained_;

        for (auto p : backup) {
            if (p->fraction() != self_) {
                persons_obtained_.erase(p);
            }
        }
    }

    if (!persons_losed_.empty()) {
        auto backup = persons_losed_;

        for (auto p : backup) {
            if (p->fraction() == self_) {
                persons_losed_.erase(p);
            }
        }
    }

    // defered worked
    handle_delayed(tl);
}

void Aggressive_ai::do_work(const Timeline& tl) { }

void Aggressive_ai::post_work(const Timeline& tl)
{
    // clear turn states
    persons_losed_.clear();
    persons_obtained_.clear();
    cities_losed_.clear();
    cities_obtained_.clear();
}

//! \name   delayed work
//! @{
void Aggressive_ai::add_delayed(Task t)
{
    YPRE_CONDITION(t);

    delayed_tasks_.push_back(t);
}

void Aggressive_ai::handle_delayed(const Timeline& tl)
{
    YLOG() << "Handle delayed work";

    auto old_deleyed = std::move(delayed_tasks_);

    for (auto task : old_deleyed) {
        task();
    }
}
//! @}

//! init the fraction after taking control of the fraction
void Aggressive_ai::init_fraction() { }

//! arch ai cache
//! @{
yrui::uptr<Iarchitecture_ai> Aggressive_ai::make_attack_ai()
{
    if (!cache_attack_) {
        cache_attack_ = yrui::core()->make_arch_ai("core::attack");
    }

    YCONSTRAINT(cache_attack_);
    return cache_attack_->clone();
}

yrui::uptr<Iarchitecture_ai> Aggressive_ai::make_develop_ai()
{
    if (!cache_develop_) {
        cache_develop_ = core()->make_arch_ai("core::develop");
    }

    YCONSTRAINT(cache_develop_);
    return cache_develop_->clone();
}
//! @}
} // of namespace Fai
