#include "foundation/contract.h"
#include "foundation/log_support.h"
#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/impl/fraction_impl.h"

namespace yrui {
namespace fraction {
    void Fraction::think(const Timeline& tl)
    {
        if (auto ctrl = controller()) {
            ctrl->think(tl);
        }
    }

    void Fraction::die()
    {
        BOOST_LOG_NAMED_SCOPE("FractionDie");

        YPRE_CONDITION(is_alive());

        //! set die flag: suppress more die call
        impl().leader_ = nullptr;
        mgr().emit_fraction_died(this);

        impl().heir_ = nullptr;
        impl().archs_.clear();
        impl().persons_.clear();

        MLOG() << "fraction " << name() << " died";

        YPOST_CONDITION(!is_alive());
    }

    void Fraction::contribute_food(int food)
    {
        BOOST_ASSERT(capital());
        capital()->set_food(this->food() + food);
    }

    void Fraction::contribute_money(int money)
    {
        BOOST_ASSERT(capital());
        capital()->set_money(this->money() + money);
    }

    void Fraction::supply_troop(int num_of_troops)
    {
        BOOST_ASSERT(capital());
        capital()->supply(num_of_troops);
    }
}
} // of namespace yrui::fraction
