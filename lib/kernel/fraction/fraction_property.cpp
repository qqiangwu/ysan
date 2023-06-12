#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/fraction_impl.h"
#include "kernel/military/troop.h"
#include "kernel/person/person.h"

#include <numeric>

namespace yrui {
namespace fraction {
    const std::string& Fraction::name() const noexcept { return impl().name_; }

    Person* Fraction::leader() const noexcept
    {
        YINVARIANT(impl().leader_);
        return impl().leader_;
    }

    Person* Fraction::heir() const noexcept { return impl().heir_; }

    Architecture* Fraction::capital() const noexcept
    {
        YINVARIANT(impl().capital_);
        return impl().capital_;
    }

    bool Fraction::has_seal() const noexcept { return impl().raw_.has_seal; }

    bool Fraction::has_emperor() const noexcept { return impl().raw_.has_emperor; }

    int Fraction::politics() const noexcept { return impl().politics_; }

    int Fraction::action_power() const noexcept { return impl().action_power_; }

    bool Fraction::is_alive() const noexcept { return !!impl().leader_; }

    const std::set<Architecture*>& Fraction::archs() const noexcept { return impl().archs_; }

    const std::set<Person*>& Fraction::persons() const noexcept { return impl().persons_; }

    Fraction_controller* Fraction::controller() const noexcept { return impl().controller_.get(); }

    int Fraction::food() const noexcept
    {
        YPRE_CONDITION(is_alive());
        YPRE_CONDITION(capital());

        return capital()->food();
    }

    int Fraction::money() const noexcept
    {
        YPRE_CONDITION(is_alive());
        YPRE_CONDITION(capital());

        return capital()->money();
    }

    bool Fraction::can_supply(const std::vector<Person*>& ps) const noexcept
    {
        auto troops = std::accumulate(ps.begin(), ps.end(), 0U, [](int acc, Person* p) {
            BOOST_ASSERT(p->has_troop());
            return acc + p->troop();
        });

        return can_supply(troops);
    }

    bool Fraction::can_supply(int num_of_troops) const noexcept
    {
        BOOST_ASSERT(capital());
        return capital()->can_supply(num_of_troops);
    }

    int Fraction::max_supply() const noexcept { return military::max_supply(money(), food()); }

    int Fraction::troop() const noexcept
    {
        auto arch = archs();
        return std::accumulate(arch.begin(), arch.end(), 0, [](int acc, Architecture* a) { return acc + a->troop(); });
    }
}
} // of namespace yrui::fraction
