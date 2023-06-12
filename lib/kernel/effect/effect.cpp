#include "kernel/effect/effect.h"
#include "kernel/core/game_obj.h"

namespace yrui {
namespace effect {
    Compound_effect::Compound_effect(id_t id, const std::vector<Effect*>& efs)
        : Effect(id)
        , effects_(efs)
    {
    }

    void Compound_effect::exert(target_type obj) const
    {
        for (auto ef : effects_) {
            ef->exert(obj);
        }
    }

    void Compound_effect::cancel(target_type obj) const
    {
        for (auto ef : effects_) {
            ef->cancel(obj);
        }
    }

    Periodic_effect::Periodic_effect(id_t id, Effect* eff)
        : Effect(id)
        , effect_(eff)
    {
    }

    namespace {
        inline Game_obj::effect_id make_eid(Game_obj* gobj, id_t eid) { return { gobj->id(), eid }; }
    }

    void Periodic_effect::exert(target_type obj) const
    {
        BOOST_ASSERT(!obj->has_effect(make_eid(obj, id())));

        obj->add_effect(make_eid(obj, id()), effect_);
    }

    void Periodic_effect::cancel(target_type obj) const
    {
        BOOST_ASSERT(obj->has_effect(make_eid(obj, id())));

        obj->remove_effect(make_eid(obj, id()));
    }
}
}
