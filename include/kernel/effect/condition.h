#ifndef YRUI_EFFECT_CONDITION_H_
#define YRUI_EFFECT_CONDITION_H_

#include "kernel/component_fwd.h"

namespace yrui {
namespace effect {
    class Condition {
    public:
        Condition(id_t id, Game_obj* target, Effect* eff, bool exert = true);

        virtual ~Condition();

    public:
        id_t id() const noexcept { return id_; }
        Game_obj* target() const noexcept { return target_; }
        Effect* effect() const noexcept { return effect_; }

    public:
        void trigger() const;

    public:
        virtual bool is_satisfied() const;

    private:
        id_t id_;
        Game_obj* target_;
        Effect* effect_;
        bool exert_;
    };
}
} // of namespace yrui::effect

#endif //! YRUI_EFFECT_CONDITION_H_
