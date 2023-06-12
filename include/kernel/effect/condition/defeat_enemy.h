#ifndef YRUI_EFFECT_CONDITION_DEFEAT_ENEMY_H_
#define YRUI_EFFECT_CONDITION_DEFEAT_ENEMY_H_

#include "kernel/detail/condition_common.h"
#include "kernel/effect/condition_def.h"

namespace yrui {
namespace effect {
    namespace condition {
        class Defeat_enemy : public detail::Condition_common<Condition_def::defeat_enemy> {
        public:
            using impl_base::Condition_common;
        };
    }
}
} // of namespace yrui

#endif //! YRUI_EFFECT_CONDITION_DEFEAT_ENEMY_H_
