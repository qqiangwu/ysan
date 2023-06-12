#include "kernel/impl/fraction_impl.h"
#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/person/person.h"

#include <numeric>

namespace yrui {
namespace aux {
    Pimpl<Fraction>::implementation::implementation(Fraction& self_, const Raw_fraction& raw) noexcept
        : self(self_)
        , raw_(raw)
    {
    }

    Pimpl<Fraction>::implementation::~implementation() = default;

    void Pimpl<Fraction>::implementation::invalidate()
    {
        politics_ = self.leader()->politics().val() / 3 + self.arch_count() + (int(self.capital()->type()) + 1);

        action_power_ = base_ap + std::sqrt(politics_);
    }

    void Pimpl<Fraction>::implementation::calculate_troop()
    {
        troops_ = std::accumulate(
            persons_.begin(), persons_.end(), 0U, [](int acc, Person* p) { return acc + p->troop(); });
    }
}
} // of namespace yrui::aux
