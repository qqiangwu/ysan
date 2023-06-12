#include "kernel/fraction/fraction_controller.h"
#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"

namespace yrui {
namespace fraction {
    int Fraction_controller::current_ap() const { return action_power_; }

    bool Fraction_controller::has_ap() const { return current_ap() != 0; }

    void Fraction_controller::consume_ap(int ap)
    {
        YPRE_CONDITION(has_ap());
        YPRE_CONDITION(ap <= action_power_);

        action_power_ -= ap;
    }

    void Fraction_controller::think(const Timeline& tl)
    {
        prepare(tl);

        //! fraction-level decision
        emit_decision(tl);

        YLOG() << self()->arch_count() << " archs need thinking";
        //! architecture level decision(automatically)
        for (auto arch : self()->archs()) {
            BOOST_ASSERT(arch && arch->fraction() == self());
            arch->think(tl);
        }

        end(tl);
    }

    void Fraction_controller::prepare(const Timeline& tl) { action_power_ = self()->action_power(); }

    Fraction* Fraction_controller::self()
    {
        BOOST_ASSERT(f_);
        return f_;
    }

    Fraction_controller::Fraction_controller(Fraction* f)
        : f_(f)
    {
    }
}
} // of namespace yrui::fraction

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
