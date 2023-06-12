#ifndef YRUI_EFFECT_CONDITION_MAYOR_H_
#define YRUI_EFFECT_CONDITION_MAYOR_H_

#include "kernel/detail/condition_common.h"
#include "kernel/effect/condition_def.h"

namespace yrui {
namespace effect {
    namespace condition {
        class Accede_mayor : public detail::Condition_common<Condition_def::accede_mayor> {
        public:
            using impl_base::Condition_common;

        public:
            virtual bool is_satisfied() const override;
        };

        class Dismiss_mayor : public detail::Condition_common<Condition_def::dismiss_mayor> {
        public:
            using impl_base::Condition_common;
        };
    }
}
} // of namespace yrui::effect::condition

#endif //! YRUI_EFFECT_CONDITION_MAYOR_H_
