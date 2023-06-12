#include <boost/range/algorithm/find_if.hpp>

#include "foundation/log_support.h"
#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person.h"

#include "kernel/impl/fraction_impl.h"

namespace Impl = yrui::fraction;

void Impl::Fraction::on_obtain_person(Person* p)
{
    BOOST_LOG_NAMED_SCOPE("ObtainPerson");

    YPRE_CONDITION(is_alive());
    YPRE_CONDITION(!persons().count(p));
    YPRE_CONDITION(p->stayed_city());
    YPRE_CONDITION(p->fraction() == this);
    YPRE_CONDITION(archs().count(p->stayed_city()));
    YPRE_CONDITION(p->stayed_city()->fraction() == this);

    MLOG() << format("%1% obtain person %2%") % name() % p->name();

    impl().persons_.insert(p);
    mgr().emit_person_obtained(this, p);
}

void Impl::Fraction::on_lose_person(Person* p)
{
    BOOST_LOG_NAMED_SCOPE("LosePerson");
    YPRE_CONDITION(is_alive());
    YPRE_CONDITION(p->fraction() != this);
    YPRE_CONDITION(persons().count(p));

    MLOG() << format("%1% lose person %2%") % name() % p->name();

    impl().persons_.erase(p);

    if (heir() == p) {
        impl().heir_ = nullptr;
    }

    if (persons().empty()) {
        MLOG() << "Lose all persons results in fraction's death";
        die();
    } else if (p == leader()) {
        if (heir()) {
            MLOG() << format("Leader deliver from %1% to %2%") % p->name() % heir()->name();

            //! \todo   emit leader die signal

            auto h = heir();
            impl().heir_ = nullptr;

            set_leader(h);
        } else {
            MLOG() << format("Leader %1% @ %2% die. Fraction die") % p->name() % name();
            die();
        }
    }

    impl().calculate_troop();
    mgr().emit_person_lost(this, p);
}

void Impl::Fraction::on_obtain_arch(Architecture* arch)
{
    BOOST_LOG_NAMED_SCOPE("ObtainArch");

    YPRE_CONDITION(is_alive());
    YPRE_CONDITION(arch->mayor() == nullptr);
    YPRE_CONDITION(arch->fraction() == this);
    YPRE_CONDITION(!archs().count(arch));

    MLOG() << format("%1% obtain arch %2%") % name() % arch->name();

    impl().archs_.insert(arch);
    impl().invalidate();
    impl().calculate_troop();

    mgr().emit_arch_obtained(this, arch);
}

void Impl::Fraction::on_lose_arch(Architecture* arch)
{
    BOOST_LOG_NAMED_SCOPE("LoseArch");

    YPRE_CONDITION(is_alive());
    YPRE_CONDITION(archs().count(arch));

    MLOG() << format("%1% lose arch %2%") % name() % arch->name();

    //! \todo   cope with persons in arch

    impl().archs_.erase(arch);

    if (capital() == arch) {
        set_capital(nullptr);

        MLOG() << format("%1% captial %2% is lost") % name() % arch->name();

        if (arch_count() != 0) {
            //! find a new capital
            MLOG() << format("%1% pick new capital") % name();

            auto new_capital = boost::range::find_if(archs(), [](Architecture* a) { return a->type() != arch::pass; });
            if (new_capital != archs().end()) {
                set_capital(*new_capital);
            } else {
                die();
            }
        }
    }

    impl().calculate_troop();
    mgr().emit_arch_lost(this, arch);

    if (arch_count() == 0 && is_alive()) {
        die();
    }

    if (is_alive()) {
        impl().invalidate();
    }

    YPOST_CONDITION(arch->mayor() == nullptr);
}
