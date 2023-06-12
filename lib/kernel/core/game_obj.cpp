#include <boost/core/null_deleter.hpp>

#include "kernel/core.h"
#include "kernel/core/game_obj.h"
#include "kernel/effect/effect.h"

namespace yrui {
Game_obj::~Game_obj() { }

//! 触发效果
void Game_obj::on_month_begin(const Timeline&)
{
    for (auto item : effects_) {
        item.second->exert(this);
    }
}

void Game_obj::on_month_end(const Timeline&) { }

void Game_obj::add_effect(effect_id id, Effect* e) { effects_.emplace(id, e); }

bool Game_obj::has_effect(effect_id id) { return effects_.find(id) != effects_.end(); }

bool Game_obj::remove_effect(effect_id id) { return effects_.erase(id) != 0; }
}
