#ifndef YRUI_EFFECT_CONDITION_LEADER_H_
#define YRUI_EFFECT_CONDITION_LEADER_H_

#include "kernel/detail/condition_common.h"
#include "kernel/effect/condition_def.h"

namespace yrui {
namespace effect {
    namespace condition {
        class Accede_leader : public detail::Condition_common<Condition_def::accede_leader> {
        public:
            using impl_base::Condition_common;

        public:
            virtual bool is_satisfied() const override;
        };

        class Dismiss_leader : public detail::Condition_common<Condition_def::dismiss_leader> {
        public:
            using impl_base::Condition_common;
        };
    }
}
} // of namespace yrui::effect::condition

#endif //! YRUI_EFFECT_CONDITION_LEADER_H_
