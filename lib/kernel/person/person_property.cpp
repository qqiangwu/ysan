#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

#include "kernel/impl/person_impl.h"

namespace Impl = yrui::person;

const std::string& Impl::Person::name() const noexcept { return impl().raw_.name; }

int Impl::Person::born_year() const noexcept { return impl().raw_.born_year; }

int Impl::Person::showed_year() const noexcept { return impl().raw_.showed_year; }

int Impl::Person::lifespan() const noexcept { return impl().raw_.lifespan; }

int Impl::Person::age() const noexcept
{
    YRUI_NOT_IMPLEMENTED;
    return 0;
}

int Impl::Person::feat() const noexcept { return impl().raw_.feat; }

Impl::Position Impl::Person::position() const noexcept { return impl().raw_.position; }

yrui::Architecture* Impl::Person::stayed_city() const noexcept { return impl().stayed_city_; }

yrui::Architecture* Impl::Person::showed_city() const noexcept { return impl().showed_city_; }

yrui::Person* Impl::Person::relative() const noexcept { return impl().relative_; }

yrui::Fraction* Impl::Person::fraction() const noexcept { return impl().fraction_; }

//! \name   state queries
//! @{
bool Impl::Person::is_leader() const noexcept { return fraction() && fraction()->leader() == this; }

bool Impl::Person::is_in_office() const noexcept { return !!fraction(); }

bool Impl::Person::is_alive() const noexcept
{
    //! a trick
    return lifespan() != 0;
}

bool Impl::Person::is_active() const noexcept { return !!stayed_city(); }

bool Impl::Person::is_supplied() const noexcept
{
    YPRE_CONDITION(has_troop());

    return impl().is_supplied_;
}
//! @}

//! \name   person abilities(currently)
//! @{
yrui::Basic_ability Impl::Person::fight() const noexcept { return impl().fight_; }

yrui::Basic_ability Impl::Person::leadership() const noexcept { return impl().leadership_; }

yrui::Basic_ability Impl::Person::wisdom() const noexcept { return impl().wisdom_; }

yrui::Basic_ability Impl::Person::politics() const noexcept { return impl().politic_; }

int Impl::Person::potential() const noexcept { return impl().raw_.potential; }
//! @}

//! \name   exp
//! @{
int Impl::Person::fight_exp() const noexcept { return impl().raw_.fight_exp; }

int Impl::Person::leadership_exp() const noexcept { return impl().raw_.leadership_exp; }

int Impl::Person::wisdom_exp() const noexcept { return impl().raw_.wisdom_exp; }

int Impl::Person::politics_exp() const noexcept { return impl().raw_.politics_exp; }
//! @}

//! \name   loyalty
//! @{
int Impl::Person::loyalty() const noexcept { return impl().raw_.loyalty; }

int Impl::Person::max_loyalty() const noexcept { return impl().raw_.max_loyalty; }
//! @}

//! \name   troop related
//! @{
int Impl::Person::controlled_troop() const noexcept { return std::min<int>(leadership().val() * troop_unit, troop()); }

bool Impl::Person::has_troop() const noexcept { return !!impl().raw_.troop; }

bool Impl::Person::is_troop_combatable() const noexcept { return has_troop() && impl().raw_.morale >= base_morale; }

bool Impl::Person::is_troop_crashed() const noexcept { return !is_troop_combatable(); }

int Impl::Person::troop() const noexcept { return impl().raw_.troop; }

int Impl::Person::max_troop() const noexcept { return impl().raw_.max_troop; }

int Impl::Person::morale() const noexcept { return impl().raw_.morale; }

int Impl::Person::drill() const noexcept { return impl().raw_.drill; }
//! @}

//! \name   Affairs and Military Ability
//! @{
yrui::Act_ability Impl::Person::farm_ability() const noexcept { return impl().farm_ability_; }

yrui::Act_ability Impl::Person::trade_ability() const noexcept { return impl().trade_ability_; }

yrui::Act_ability Impl::Person::security_ability() const noexcept { return impl().security_ability_; }

yrui::Act_ability Impl::Person::steady_ability() const noexcept { return impl().steady_ability_; }

yrui::Act_ability Impl::Person::train_ability() const noexcept { return impl().train_ability_; }

yrui::Act_ability Impl::Person::attack() const noexcept { return impl().attack_; }

yrui::Act_ability Impl::Person::defence() const noexcept { return impl().defence_; }

yrui::Act_ability Impl::Person::strategy_rate() const noexcept { return impl().strategy_rate_; }

yrui::Act_ability Impl::Person::strategy_success_rate() const noexcept { return impl().strategy_success_rate_; }

yrui::Act_ability Impl::Person::strategy_detect_rate() const noexcept { return impl().strategy_detect_rate_; }

yrui::Act_ability Impl::Person::strategy_attack() const noexcept { return impl().strategy_attack_; }

yrui::Act_ability Impl::Person::strategy_defence() const noexcept { return impl().strategy_defence_; }
//! @}
