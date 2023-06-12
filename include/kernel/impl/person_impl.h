#ifndef YRUI_DETAIL_PERSON_IMPL_H_
#define YRUI_DETAIL_PERSON_IMPL_H_

#include "kernel/component_fwd.h"
#include "kernel/detail/pimpl.h"
#include "kernel/detail/characteristic_type.h"
#include "kernel/detail/raw_person.h"

namespace yrui {
namespace aux {
    using namespace yrui::person;
    using yrui::detail::Raw_person;

    template <> struct Pimpl<Person>::implementation {
    public:
        implementation(Person& interf, const Raw_person& raw) noexcept;

        void init_self();

    public:
        void calculate_abilities() noexcept;

        void troop_loss(int n) noexcept;
        void lose_troop() noexcept;

    public:
        void leave_office() noexcept;

    public:
        void set_loyalty(int val) noexcept;
        void set_position(Position pos) noexcept;
        void set_stayed_city(Architecture* arch) noexcept;
        void set_fraction(Fraction* frac) noexcept;

    public:
        bool is_valid() noexcept;

    private:
        ylogger::logger_type& logger() const;

        //! \name   representation
        //! @{
    public:
        Person& self;

        Raw_person raw_;

        Basic_ability fight_;
        Basic_ability leadership_;
        Basic_ability wisdom_;
        Basic_ability politic_;

        Act_ability farm_ability_;
        Act_ability trade_ability_;
        Act_ability security_ability_;
        Act_ability steady_ability_;
        Act_ability train_ability_;

        Act_ability attack_;
        Act_ability defence_;

        Act_ability strategy_rate_;
        Act_ability strategy_success_rate_;
        Act_ability strategy_detect_rate_;
        Act_ability strategy_attack_;
        Act_ability strategy_defence_;

        bool is_supplied_ = true;

        Architecture* stayed_city_;
        Architecture* showed_city_;
        Person* relative_;
        Fraction* fraction_;

        std::vector<Effect*> fixed_skill_;
        //! @}
    };
}
} // of namespace yrui::aux

#endif // YRUI_DETAIL_PERSON_IMPL_H_
