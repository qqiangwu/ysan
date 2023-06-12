#include "kernel/effect/condition/mayor_condition.h"
#include "kernel/arch/architecture.h"
#include "kernel/effect/effect_target.h"
#include "kernel/person/person.h"

namespace Impl = yrui::effect::condition;

bool Impl::Accede_mayor::is_satisfied() const
{
    YPRE_CONDITION(target());

    auto ptr = impl::instance<person::Person>(target());
    YCONSTRAINT(ptr);

    return ptr->stayed_city() && ptr->stayed_city()->mayor() == ptr;
}
