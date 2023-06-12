#ifndef YRUI_CORE_GAME_OBJ_COMMON_H_
#define YRUI_CORE_GAME_OBJ_COMMON_H_

#include "kernel/core/game_obj.h"
#include "kernel/detail/pimpl.h"

namespace yrui {
namespace detail {
    template <class GameObj, class GameMgr>
    class Game_obj_common : public Game_obj, protected aux::Pimpl<GameObj>::value_sementics {
    public:
        using base_type = Game_obj_common;

        template <class... Args>
        Game_obj_common(id_t id, Args&&... args)
            : Game_obj(id)
            , aux::Pimpl<GameObj>::value_sementics(std::forward<Args>(args)...)
        {
        }

    public:
        using logger_type = ylogger::logger_type;
        using entity_type = GameObj;
        using mgr_type = GameMgr;

    public:
        mgr_type& mgr() const { return *mgr_; }

        logger_type& logger() const { return *logger_; }

    protected:
        mgr_type* mgr_ = nullptr;
        logger_type* logger_ = nullptr;
    };
}
} // of namespace yrui

#endif // YRUI_CORE_GAME_OBJ_COMMON_H_
