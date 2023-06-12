#ifndef YRUI_IMPL_FRACTION_IMPL_H_
#define YRUI_IMPL_FRACTION_IMPL_H_

#include "kernel/component_fwd.h"
#include "kernel/detail/raw_fraction.h"
#include "kernel/fraction/fraction.h"

namespace yrui {
namespace aux {
    using namespace yrui::fraction;
    using yrui::detail::Raw_fraction;

    template <> struct Pimpl<Fraction>::implementation {
    public:
        implementation(Fraction& self, const Raw_fraction& raw) noexcept;

        ~implementation();

        //! recalculate politics and action_power
        void invalidate();
        void calculate_troop();

    public:
        Fraction& self;

        Raw_fraction raw_;

        uptr<Fraction_controller> controller_;

        std::string name_;

        int politics_ {};
        int action_power_ {};
        int troops_ {};

        Person* leader_;
        Person* heir_;
        Architecture* capital_;

        std::set<Architecture*> archs_;
        std::set<Person*> persons_;
    };
}
} // of namespace yrui::aux

#endif // YRUI_IMPL_FRACTION_IMPL_H_
