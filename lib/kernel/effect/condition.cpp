#include "kernel/effect/condition.h"
#include "kernel/core/game_obj.h"
#include "kernel/effect/effect.h"

namespace Impl = yrui::effect;

Impl::Condition::Condition(id_t id, Game_obj* target, Effect* eff, bool exert)
    : id_(id)
    , target_(target)
    , effect_(eff)
    , exert_(exert)
{
    YPOST_CONDITION(this->id());
    YPOST_CONDITION(this->target());
    YPOST_CONDITION(this->effect());
}

Impl::Condition::~Condition() { }

void Impl::Condition::trigger() const
{
    YPRE_CONDITION(id());
    YPRE_CONDITION(target());
    YPRE_CONDITION(effect());

    if (exert_) {
        effect_->exert(target_);
    } else {
        effect_->cancel(target_);
    }
}

bool Impl::Condition::is_satisfied() const { return false; }
