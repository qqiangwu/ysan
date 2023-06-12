#ifndef YRUI_IMPL_ARCH_IMPL_H_
#define YRUI_IMPL_ARCH_IMPL_H_

#include <set>

#include "kernel/component_fwd.h"
#include "kernel/detail/pimpl.h"
#include "kernel/detail/raw_architecture.h"

namespace yrui {
namespace aux {
    using namespace yrui::arch;
    using yrui::detail::Raw_architecture;

    template <> struct Pimpl<Architecture>::implementation {
        implementation(Architecture& self, const Raw_architecture& raw) noexcept;

        void init_self();

        void abort_jobs();
        void clear_jobs() { abort_jobs(); }

        void replenish_escuage();

        void calculate_troop();
        void calculate_connectivity();

        void clear_state();

        ylogger::logger_type& logger() const;

        //! \name   representations
        //! @{
        Architecture& self;

        Raw_architecture raw_;

        Fraction* fraction_;
        Person* mayor_;

        int troop_num_ {};
        int escuage_ {};

        bool beleaguered_ {};
        bool connected_to_capital_ {};

        std::set<Person*> all_;
        std::set<Person*> in_city_;
        std::set<Person*> out_city_;

        std::map<Person*, bool> has_worked_;

        bool has_done_farm_ = false;
        bool has_done_trade_ = false;
        bool has_done_steady_ = false;
        bool has_done_security_ = false;

        std::set<uptr<job::Job>> jobs_;

        uptr<Iarchitecture_ai> ai_;
        //! @}
    };
}
} // of namespace yrui::aux

#endif // YRUI_IMPL_ARCH_IMPL_H_
