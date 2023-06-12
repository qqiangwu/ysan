#ifndef YRUI_CORE_GAME_OBJ_H_
#define YRUI_CORE_GAME_OBJ_H_

#include <map>

#include "kernel/component_fwd.h"

namespace yrui {
/*!
 * 为什么要添加这个一个基类，完成没有任何理由的乱搞！如果只是Effects系统中需要，那么
 * 用boost::any效果没什么差别！
 *
 * 所有游戏对象的基类，提供以下公有功能：
 *      periodic effect
 *
 * 之后的某个时间，我会将以下内容也移入去这个类中
 *      1. id 及 name 支持
 */
class Game_obj {
public:
    virtual ~Game_obj();

protected:
    Game_obj(id_t id)
        : id_(id)
    {
    }

public:
    id_t id() const { return id_; }

public:
    virtual void on_month_begin(const Timeline& tl);
    virtual void on_month_end(const Timeline& tl);

public:
    //! \name   periodic effects
    //! @{
    /* effect 标识符, 即effect_id + target_id */
    typedef std::pair<id_t, id_t> effect_id;

    void add_effect(effect_id id, Effect*);
    bool has_effect(effect_id id);
    bool remove_effect(effect_id id);
    //! @}

private:
    id_t id_;

private:
    std::map<effect_id, Effect*> effects_;
};
}

#endif // YRUI_CORE_GAME_OBJ_H_
