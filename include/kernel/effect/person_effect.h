#ifndef YRUI_CORE_EFFECT_PERSON_H_H_
#define YRUI_CORE_EFFECT_PERSON_H_H_

#include "kernel/effect/effect.h"
#include "kernel/effect/effect_target.h"
#include "kernel/person/person.h"

#define DEFINE_PERSON_EFFECT(Ability, id)                                                                              \
    namespace detail {                                                                                                 \
    int get_##Ability##_effect(Person* p);                                                                             \
    void set_##Ability##_effect(Person* p, int v);                                                                     \
                                                                                                                       \
    template <id_t ID, bool Incr, Result result> class Ability##_effect : public Effect {                              \
    public:                                                                                                            \
        enum { effect_id = ID };                                                                                       \
        enum { is_positive = Incr };                                                                                   \
                                                                                                                       \
    public:                                                                                                            \
        Ability##_effect()                                                                                             \
            : Effect(ID)                                                                                               \
        {                                                                                                              \
        }                                                                                                              \
                                                                                                                       \
        virtual void exert(target_type obj) const override                                                             \
        {                                                                                                              \
            constexpr auto fix = Incr ? 1 : -1;                                                                        \
            set(obj, fix* result);                                                                                     \
        }                                                                                                              \
                                                                                                                       \
        virtual void cancel(target_type obj) const override                                                            \
        {                                                                                                              \
            constexpr auto fix = Incr ? 1 : -1;                                                                        \
            set(obj, fix* result);                                                                                     \
        }                                                                                                              \
                                                                                                                       \
    private:                                                                                                           \
        int get(target_type obj) const                                                                                 \
        {                                                                                                              \
            BOOST_ASSERT(impl::is_instance_of<person::Person>(obj));                                                   \
            return get_##Ability##_effect(impl::instance<person::Person>(obj));                                        \
        }                                                                                                              \
                                                                                                                       \
        void set(target_type obj, int v) const                                                                         \
        {                                                                                                              \
            BOOST_ASSERT(impl::is_instance_of<person::Person>(obj));                                                   \
            return set_##Ability##_effect(impl::instance<person::Person>(obj), v);                                     \
        }                                                                                                              \
    };                                                                                                                 \
    }                                                                                                                  \
    using Ability##_effect_1 = detail::Ability##_effect<id * detail::unit + 1, true, detail::minor>;                   \
    using Ability##_effect_2 = detail::Ability##_effect<id * detail::unit + 2, true, detail::mid>;                     \
    using Ability##_effect_3 = detail::Ability##_effect<id * detail::unit + 3, true, detail::large>;                   \
    using Ability##_effect_4 = detail::Ability##_effect<id * detail::unit + 4, false, detail::minor>;                  \
    using Ability##_effect_5 = detail::Ability##_effect<id * detail::unit + 5, false, detail::mid>;                    \
    using Ability##_effect_6 = detail::Ability##_effect<id * detail::unit + 6, false, detail::large>

namespace yrui {
namespace effect {
    namespace detail {
        constexpr auto unit = 10U;

        enum Result { minor = 10, mid = 20, large = 30 };
    }

    DEFINE_PERSON_EFFECT(farm, 0);
    DEFINE_PERSON_EFFECT(trade, 1);
    DEFINE_PERSON_EFFECT(security, 2);
    DEFINE_PERSON_EFFECT(steady, 3);
    DEFINE_PERSON_EFFECT(train, 4);

    DEFINE_PERSON_EFFECT(attack, 10);
    DEFINE_PERSON_EFFECT(defence, 11);

    DEFINE_PERSON_EFFECT(strategy_attack, 20);
    DEFINE_PERSON_EFFECT(strategy_defence, 21);
    DEFINE_PERSON_EFFECT(strategy_rate, 22);
    DEFINE_PERSON_EFFECT(strategy_detect_rate, 23);
    DEFINE_PERSON_EFFECT(strategy_success_rate, 24);
}
}

#undef DEFINE_PERSON_EFFECT

#endif // YRUI_CORE_EFFECT_PERSON_H_H_
