#include "kernel/person/person_mgr.h"

namespace Impl = yrui::person;

void Impl::Person_mgr::emit_person_die(Person* p)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonDie");

    person_die(p);
}

void Impl::Person_mgr::emit_person_appear(Person* p)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonAppear");

    person_appear(p);
}

void Impl::Person_mgr::emit_person_be_official(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonJoinFraction");

    person_join_fraction(p, f);
}

void Impl::Person_mgr::emit_person_leave_offical(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonLeaveFrac");
    person_leave_fraction(p, f);
}

void Impl::Person_mgr::emit_person_reside_arch(Person* p, Architecture* a)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonResideArch");
    person_reside_arch(p, a);
}

void Impl::Person_mgr::emit_person_leave_arch(Person* p, Architecture* a)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonLeaveArch");

    person_leave_arch(p, a);
}

void Impl::Person_mgr::emit_person_captured(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonCaptured");

    person_captured(p, f);
}

void Impl::Person_mgr::emit_person_capitulated(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonCapitulated");

    person_capitulated(p, f);
}

void Impl::Person_mgr::emit_person_killed(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonKilled");

    person_killed(p, f);
}

void Impl::Person_mgr::emit_person_released(Person* p, Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("EmitPersonReleased");

    person_released(p, f);
}
