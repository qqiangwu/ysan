#include "kernel/effect/person_effect.h"
#include "kernel/person/person.h"

namespace Impl = yrui::effect::detail;

#define IMPLEMENT_PERSON_EFFECT(Ability)                                                                               \
    void Impl::set_##Ability##_effect(Person* p, int v)                                                                \
    {                                                                                                                  \
        p->set_##Ability(v > 0 ? (p->Ability() + int(v)) : (p->Ability() - int(v)));                                   \
    }

#define IMPLEMENT_PERSON_EFFECT_1(Ability)                                                                             \
    void Impl::set_##Ability##_effect(Person* p, int v)                                                                \
    {                                                                                                                  \
        p->set_##Ability##_ability(v > 0 ? (p->Ability##_ability() + int(v)) : (p->Ability##_ability() - int(v)));     \
    }

IMPLEMENT_PERSON_EFFECT_1(farm)
IMPLEMENT_PERSON_EFFECT_1(trade)
IMPLEMENT_PERSON_EFFECT_1(security)
IMPLEMENT_PERSON_EFFECT_1(steady)
IMPLEMENT_PERSON_EFFECT_1(train)

IMPLEMENT_PERSON_EFFECT(attack)
IMPLEMENT_PERSON_EFFECT(defence)

IMPLEMENT_PERSON_EFFECT(strategy_attack)
IMPLEMENT_PERSON_EFFECT(strategy_defence)
IMPLEMENT_PERSON_EFFECT(strategy_rate)
IMPLEMENT_PERSON_EFFECT(strategy_detect_rate)
IMPLEMENT_PERSON_EFFECT(strategy_success_rate)

#undef IMPLEMENT_PERSON_EFFECT
#undef IMPLEMENT_PERSON_EFFECT_1
