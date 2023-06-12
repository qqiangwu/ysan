#ifndef YRUI_IMPL_EFFECT_TARGET_H_
#define YRUI_IMPL_EFFECT_TARGET_H_

#include "kernel/core/game_obj.h"

namespace yrui {
namespace impl {
    template <class Target> inline bool is_instance_of(Game_obj* obj) { return !!dynamic_cast<Target*>(obj); }

    template <class Target> inline auto instance(Game_obj* obj)
    {
        BOOST_ASSERT(is_instance_of<Target>(obj));

        return dynamic_cast<Target*>(obj);
    }
}
} // of namespace yrui::impl

#endif //! YRUI_IMPL_EFFECT_TARGET_H_
