#include "kernel/impl/person_impl.h"
#include "kernel/person/person.h"

namespace Impl = yrui::person;

//! \name   Affairs and Military Ability
//! @{
#undef IMPLEMENT_ABILITY_SETTER
#define IMPLEMENT_ABILITY_SETTER(ability)                                                                              \
    void Impl::Person::set_##ability(Act_ability n) noexcept { impl().ability##_ = n; }

IMPLEMENT_ABILITY_SETTER(farm_ability)
IMPLEMENT_ABILITY_SETTER(trade_ability)
IMPLEMENT_ABILITY_SETTER(security_ability)
IMPLEMENT_ABILITY_SETTER(steady_ability)
IMPLEMENT_ABILITY_SETTER(train_ability)

IMPLEMENT_ABILITY_SETTER(attack)
IMPLEMENT_ABILITY_SETTER(defence)

IMPLEMENT_ABILITY_SETTER(strategy_rate)
IMPLEMENT_ABILITY_SETTER(strategy_success_rate)
IMPLEMENT_ABILITY_SETTER(strategy_detect_rate)
IMPLEMENT_ABILITY_SETTER(strategy_attack)
IMPLEMENT_ABILITY_SETTER(strategy_defence)

#undef IMPLEMENT_ABILITY_SETTER
//! @}

void Impl::Person::set_position(Position pos) noexcept
{
    YPRE_CONDITION(!pos || fraction());
    YPRE_CONDITION(stayed_city());
    YPRE_CONDITION(feat() >= pos * 100);

    impl().set_position(pos);
}

//! \name   gain exp
//! @{
namespace {
bool try_level_up(yrui::Basic_ability& ab, int& potential)
{
    const auto consume = ab.val() / 10 + 1;
    if (potential >= consume) {
        potential -= consume;
        ab = ab + 1;
    }
    return false;
}
}

void Impl::Person::gain_fight_exp(int n) noexcept
{
    if (n >= max_exp) {
        n -= max_exp;
        if (try_level_up(impl().fight_, impl().raw_.potential)) {
            impl().calculate_abilities();
            MLOG() << format("%1% fight + 1") % name();
        }
    }

    impl().raw_.fight_exp = n;
}

void Impl::Person::gain_leadership_exp(int n) noexcept
{
    if (n >= max_exp) {
        n -= max_exp;
        if (try_level_up(impl().leadership_, impl().raw_.potential)) {
            impl().calculate_abilities();
            MLOG() << format("%1% leadership + 1") % name();
        }
    }

    impl().raw_.leadership_exp = n;
}

void Impl::Person::gain_wisdom_exp(int n) noexcept
{
    if (n >= max_exp) {
        n -= max_exp;
        if (try_level_up(impl().wisdom_, impl().raw_.potential)) {
            impl().calculate_abilities();
            MLOG() << format("%1% wisdom + 1") % name();
        }
    }

    impl().raw_.wisdom_exp = n;
}

void Impl::Person::gain_politics_exp(int n) noexcept
{
    if (n >= max_exp) {
        n -= max_exp;
        if (try_level_up(impl().politic_, impl().raw_.potential)) {
            impl().calculate_abilities();
            MLOG() << format("%1% politics + 1") % name();
        }
    }

    impl().raw_.politics_exp = n;
}

void Impl::Person::add_feat(int n) noexcept { impl().raw_.feat += n; }
//! @}

//! \name   military
//! @{
void Impl::Person::morale_up(int n) noexcept { impl().raw_.morale = std::min(100, impl().raw_.morale + n); }

void Impl::Person::drill_up(int n) noexcept { impl().raw_.drill = std::min(100, impl().raw_.drill + n); }

void Impl::Person::morale_loss(int n) noexcept { impl().raw_.morale = std::max(0, impl().raw_.morale - n); }
//! @}
