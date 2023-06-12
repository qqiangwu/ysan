#ifndef YRUI_CORE_EFFECT_MGR_H_
#define YRUI_CORE_EFFECT_MGR_H_

#include <boost/range/adaptor/map.hpp>

#include <unordered_map>

#include "kernel/core/subsys.h"
#include "kernel/detail/range_view.h"
#include "kernel/effect/effect.h"

namespace yrui {
namespace effect {
    class Effect_mgr : public Isubsys {
    public:
        Effect_mgr();

    public:
        virtual void update() override;

    public:
        auto as_range() const noexcept;

        Effect* get(id_t id) const noexcept;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    private:
        std::unordered_map<id_t, uptr<Effect>> effects_;
    };
}
} // of namespace yrui::effect

namespace yrui {
namespace effect {
    inline auto Effect_mgr::as_range() const noexcept { return make_range(effects_ | boost::adaptors::map_values); }
}
} // of namespace yrui

#endif //! YRUI_CORE_EFFECT_MGR_H_
