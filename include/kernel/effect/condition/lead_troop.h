#ifndef YRUI_EFFECT_CONDITION_LEAD_LEGION_H_
#define YRUI_EFFECT_CONDITION_LEAD_LEGION_H_

#include "kernel/detail/condition_common.h"
#include "kernel/effect/condition_def.h"

namespace yrui {
namespace effect {
    namespace condition {
        class Lead_legion : public detail::Condition_common<Condition_def::lead_legion> {
        public:
            using impl_base::Condition_common;
        };
    }
}
} // of namespace yrui

#endif //! YRUI_EFFECT_CONDITION_LEAD_LEGION_H_
