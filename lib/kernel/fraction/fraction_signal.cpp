#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"

namespace Impl = yrui::fraction;

void Impl::Fraction_mgr::emit_fraction_died(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitFractionDie");

    fraction_died(f);
}

void Impl::Fraction_mgr::emit_succession(Fraction* f, Person* p)
{
    BOOST_LOG_NAMED_SCOPE("EmitSuccession");

    succession(f, p);
}

void Impl::Fraction_mgr::emit_arch_obtained(Fraction* f, Architecture* a)
{
    BOOST_LOG_NAMED_SCOPE("EmitArchObtain");

    arch_obtained(f, a);
}

void Impl::Fraction_mgr::emit_arch_lost(Fraction* f, Architecture* a)
{
    BOOST_LOG_NAMED_SCOPE("EmitArchLost");

    arch_lost(f, a);
}

void Impl::Fraction_mgr::emit_person_obtained(Fraction* f, Person* p)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonObtain");

    person_obtained(f, p);
}

void Impl::Fraction_mgr::emit_person_lost(Fraction* f, Person* p)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonLost");

    person_lost(f, p);
}

void Impl::Fraction_mgr::emit_nation_unified(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitNationUnified");

    nation_unified(f);
}
