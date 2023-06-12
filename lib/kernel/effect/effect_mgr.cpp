#include "kernel/effect/effect_mgr.h"
#include "kernel/effect/person_effect.h"

namespace Impl = yrui::effect;

#undef INSTALL_EFFECT
#undef INSTALL_EFFECT_IMPL

#define INSTALL_EFFECT_IMPL(ability_name)                                                                              \
    effects_.emplace(ability_name##_1::effect_id, make_uptr<ability_name##_1>());                                      \
    effects_.emplace(ability_name##_2::effect_id, make_uptr<ability_name##_2>());                                      \
    effects_.emplace(ability_name##_3::effect_id, make_uptr<ability_name##_3>());                                      \
    effects_.emplace(ability_name##_4::effect_id, make_uptr<ability_name##_4>());                                      \
    effects_.emplace(ability_name##_5::effect_id, make_uptr<ability_name##_5>());                                      \
    effects_.emplace(ability_name##_6::effect_id, make_uptr<ability_name##_6>())

#define INSTALL_EFFECT(AB) INSTALL_EFFECT_IMPL(AB##_effect)

Impl::Effect_mgr::Effect_mgr()
{
    INSTALL_EFFECT(farm);
    INSTALL_EFFECT(trade);
    INSTALL_EFFECT(security);
    INSTALL_EFFECT(steady);
    INSTALL_EFFECT(train);

    INSTALL_EFFECT(attack);
    INSTALL_EFFECT(defence);

    INSTALL_EFFECT(strategy_attack);
    INSTALL_EFFECT(strategy_defence);
    INSTALL_EFFECT(strategy_rate);
    INSTALL_EFFECT(strategy_detect_rate);
    INSTALL_EFFECT(strategy_success_rate);
}

#undef INSTALL_EFFECT
#undef INSTALL_EFFECT_IMPL

void Impl::Effect_mgr::update()
{ /* Empty now */
}

yrui::Effect* Impl::Effect_mgr::get(id_t id) const noexcept
{
    YPRE_CONDITION(id != null_id);

    auto iter = effects_.find(id);

    return iter == effects_.end() ? nullptr : iter->second.get();
}

bool Impl::Effect_mgr::init_impl() { return true; }

void Impl::Effect_mgr::finalize_impl()
{ /* Empty now */
}
