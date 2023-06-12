#include "kernel/effect/condition/leader_condition.h"
#include "kernel/effect/effect_target.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

namespace Impl = yrui::effect::condition;

bool Impl::Accede_leader::is_satisfied() const
{
    YPRE_CONDITION(target());

    auto ptr = impl::instance<person::Person>(target());
    YCONSTRAINT(ptr);

    return ptr->fraction() && ptr->fraction()->leader() == ptr;
}
