#include "kernel/military/intruded_arch.h"
#include "kernel/common/common_util.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/military/legion.h"
#include "kernel/person/person.h"

//! \todo   modify log

namespace yrui {
namespace military {
    Intruded_arch::Intruded_arch(Architecture* arch, Legion* enemy)
        : arch_(arch)
        , enemy_(enemy)
    {
    }

    Intruded_arch::~Intruded_arch() { }

    const std::string& Intruded_arch::name() const { return arch_->name(); }

    Fraction_controller* Intruded_arch::controller() const { return common::get_controller(arch_); }

    Architecture* Intruded_arch::arch() const { return arch_; }

    Fraction* Intruded_arch::fraction() const { return arch_->fraction(); }

    bool Intruded_arch::is_conquered() const { return arch_->is_conquered() || is_conquered_; }

    bool Intruded_arch::is_out_of_defence() const { return is_conquered_; }

    void Intruded_arch::out_of_defence() { is_conquered_ = true; }

    int Intruded_arch::suffer_wreck(Strategy_power power) { return arch_->suffer_wreck(power); }

    Attack_power Intruded_arch::suffer_attack(Attack_power power) { return arch_->suffer_attack(power); }

    int Intruded_arch::suffer_seige() { return arch_->suffer_siege(); }

    std::pair<Person*, Person*> Intruded_arch::strategy_troops() const { return { strategy_1_, strategy_2_ }; }
    ////////////////////////////////////////////////////////////////////////
    //! 挑选用计者
    namespace {
        std::pair<Person*, Person*> prepare_strategy_troops(Legion::Troop_container&& troops)
        {
            switch (troops.size()) {
            case 1:
                return { troops.front(), nullptr };

            case 0:
                // BOOST_ASSERT_MSG(false, "Intruded arch must have troops");
                // break;
                //! \fixme
                YLOG() << "No troops in arch can defend";
                return { nullptr, nullptr };

            default: {
                std::sort(troops.begin(), troops.end(),
                    [](Person* x, Person* y) { return x->wisdom().val() > y->wisdom().val(); });

                return { troops[0], troops[1] };
            }
            }
        }
    }

    void Intruded_arch::on_battle_start(Military_mgr& mgr)
    {
        BOOST_ASSERT(arch_->num_in_city() != 0);

        YLOG_SEV(trace) << format("Arch %1% on battle start") % name();

        //! abort jobs
        arch_->abort_jobs();

        //! decide whether to use strategy
        auto ctrl = controller();
        auto in_city_men = arch_->in_city() | filtered([](Person* p) { return !p->is_troop_crashed(); });

        auto candidate = prepare_strategy_troops({ in_city_men.begin(), in_city_men.end() });

        if (candidate.first && ctrl->permit_strategy(candidate.first)) {
            strategy_1_ = candidate.first;

            YLOG_SEV(trace) << format("%1% in arch prepare to use strategy") % strategy_1_->name();
        }

        if (candidate.second && ctrl->permit_strategy(candidate.second)) {
            strategy_2_ = candidate.second;

            YLOG_SEV(trace) << format("%1% in arch prepare to use strategy") % strategy_2_->name();
        }
    }

    void Intruded_arch::on_battle_end(Military_mgr& mgr) { }

    void Intruded_arch::on_day_begin()
    {
        //! do nothing
    }

    void Intruded_arch::on_day_end()
    {
        arch_->suffer_siege();

        YLOG_SEV(trace) << format("Arch %1% end day[food %2% | st %3%]") % name() % arch_->food() % arch_->steady();
    }
}
}
