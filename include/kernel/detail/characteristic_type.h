#ifndef YRUI_CORE_BASIC_TYPE_H_
#define YRUI_CORE_BASIC_TYPE_H_

#include "kernel/config/characteristic_config.h"
#include "kernel/detail/semantic_int.h"

namespace yrui {
using Basic_ability = detail::Semantic_int<1, basic_ability_max, struct basic_ability_tag>;
using Act_ability = detail::Semantic_int<1, act_ability_max, struct act_ability_tag>;
using Attack_power = detail::Semantic_int<0, attack_power_max, struct attack_power_tag>;
using Strategy_power = detail::Semantic_int<0, strategy_power_max, struct strategy_power_tag>;
}

#endif // YRUI_CORE_BASIC_TYPE_H_
