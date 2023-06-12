#ifndef YRUI_CORE_EFFECT_H_
#define YRUI_CORE_EFFECT_H_

#include <vector>

#include "kernel/component_fwd.h"

namespace yrui {
namespace effect {
    /* atom of the effect system */
    class Effect {
    public:
        typedef Game_obj* target_type;

    public:
        Effect(id_t id)
            : id_(id)
        {
            BOOST_ASSERT(id != null_id);
        }

        virtual ~Effect() = default;

        id_t id() const { return id_; }

    public:
        virtual void exert(target_type obj) const = 0;
        virtual void cancel(target_type obj) const = 0;

    private:
        id_t id_;
    };

    /* structural effect */
    class Compound_effect : public Effect {
    public:
        Compound_effect(id_t id, const std::vector<Effect*>& efs);

    public:
        virtual void exert(target_type obj) const override;
        virtual void cancel(target_type obj) const override;

    private:
        using Effect_container = std::vector<Effect*>;

        Effect_container effects_;
    };

    class Periodic_effect : public Effect {
    public:
        Periodic_effect(id_t id, Effect* eff);

    public:
        virtual void exert(target_type obj) const override;
        virtual void cancel(target_type obj) const override;

    private:
        Effect* effect_;
    };
}
}

#endif // YRUI_CORE_EFFECT_H_
