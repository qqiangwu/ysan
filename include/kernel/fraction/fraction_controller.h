#ifndef YRUI_FRACTION_CONTROLLER_H_
#define YRUI_FRACTION_CONTROLLER_H_

/*!
 *
 * \file     Fraction/fraction_controller.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    interface used to manage fractions.
 * \ingroup  Fraction
 *
 */

#include "kernel/component_fwd.h"

namespace yrui {
namespace fraction {
    class Fraction_controller {
    public:
        virtual ~Fraction_controller() = default;

    protected:
        virtual void prepare(const Timeline& tl);
        virtual void emit_decision(const Timeline& tl) = 0;
        virtual void end(const Timeline& tl) { }

    public:
        virtual uptr<Legion> defend(Legion* attacker) = 0;

        virtual void command_troop(Legion* self, Legion* enemy) = 0;
        virtual void command_troop(Legion* self, Intruded_arch* enemy) = 0;

        virtual bool permit_strategy(Person* p) = 0;

    public:
        void think(const Timeline& tl);

        int current_ap() const;
        bool has_ap() const;

        void consume_ap(int ap = 1);

        Fraction* self();

        explicit Fraction_controller(Fraction* f);

    private:
        Fraction* f_;
        int action_power_ = 0;
    };

    class AI_controller : public Fraction_controller {
    public:
        virtual void emit_decision(const Timeline& tl) override;

    public:
        virtual uptr<Legion> defend(Legion* attacker) override;

        virtual void command_troop(Legion*, Legion*) override;
        virtual void command_troop(Legion*, Intruded_arch*) override;
        virtual bool permit_strategy(Person* p) override;

    public:
        void install_ai(uptr<Ifraction_ai> ai);

        using Fraction_controller::Fraction_controller;

    private:
        uptr<Ifraction_ai> ai_;
    };

    class Player_controller : public Fraction_controller {
    public:
        using Fraction_controller::Fraction_controller;
    };
} // of namespace fraction
} // of namespace yrui

#endif // !YRUI_FRACTION_CONTROLLER_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
