#ifndef YRUI_CORE_MILITARY_INTRUDED_ARCH_H_
#define YRUI_CORE_MILITARY_INTRUDED_ARCH_H_

#include "kernel/arch/architecture.h"
#include "kernel/component_fwd.h"
#include "kernel/detail/characteristic_type.h"

namespace yrui {
namespace military {
    class Intruded_arch {
    public:
        Intruded_arch(Architecture* arch, Legion* enemy);
        ~Intruded_arch();

    public:
        const std::string& name() const;

        Architecture* arch() const;
        Fraction* fraction() const;

        bool is_conquered() const;
        bool is_out_of_defence() const;

        Fraction_controller* controller() const;

        std::pair<Person*, Person*> strategy_troops() const;

    public:
        //! 内奸陷城
        void out_of_defence();
        int suffer_seige();
        int suffer_wreck(Strategy_power power);
        Attack_power suffer_attack(Attack_power power);

    public:
        void on_battle_start(Military_mgr& mgr);
        void on_battle_end(Military_mgr& mgr);

        void on_day_begin();
        void on_day_end();

    private:
        Architecture* arch_;
        [[maybe_unused]] Legion* enemy_;

        Person* strategy_1_;
        Person* strategy_2_;

        bool is_conquered_ = false;
    };
}
}

#endif // YRUI_CORE_MILITARY_INTRUDED_ARCH_H_
