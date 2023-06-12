#ifndef YRUI_DETAIL_CONDITION_COMMON_H_
#define YRUI_DETAIL_CONDITION_COMMON_H_

#include "kernel/effect/condition.h"
#include "kernel/effect/condition_def.h"

namespace yrui {
namespace effect {
    namespace detail {
        template <Condition_def ID> class Condition_common : public Condition {
        public:
            enum { condition_id = id_t(ID) };

        protected:
            using impl_base = Condition_common;

        public:
            Condition_common(Game_obj* target, Effect* eff, bool exert)
                : Condition(condition_id, target, eff, exert)
            {
            }
        };
    }
}
} // of namespace yrui::effect::detail

#endif //! YRUI_DETAIL_CONDITION_COMMON_H_
