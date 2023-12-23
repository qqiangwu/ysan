#include "kernel/person/person.h"
#include <Detail/Filter.h>

namespace BF = Bygone::Frontend;

QString BF::filter_position(yrui::person::Position val)
{
    using namespace yrui::person;

    switch (val) {
    case position_const::duwei:
        return "都尉";
    case position_const::general:
        return "将军";
    case position_const::leader:
        return "君主";
    case position_const::marshal:
        return "元帅";
    case position_const::xiaowei:
        return "校尉";
    case position_const::subgeneral:
        return "偏将";
    default:
        return "无";
    }
}

QString BF::filter_ability(yrui::Basic_ability ability)
{
    const auto val = ability.val();

    if (val <= 5) {
        return "平庸";
    } else if (val <= 10) {
        return "不彰";
    } else if (val <= 15) {
        return "良好";
    } else if (val <= 20) {
        return "优秀";
    } else if (val <= 25) {
        return "一流";
    } else {
        return "卓越";
    }
}

QString BF::filter_troop(yrui::Person* p) { return p->has_troop() ? QString::number(p->troop()) : "--"; }

QString BF::filter_morale(yrui::Person* p)
{
    return p->has_troop() ? (p->morale() >= yrui::standard_morale ? "能战" : (p->is_troop_crashed() ? "溃散" : "低下"))
                          : "--";
}

QString BF::filter_drill(yrui::Person* p) { return p->has_troop() ? QString::number(p->drill()) : "--"; }

QString BF::filter_supply(yrui::Person* p) { return p->has_troop() ? (p->is_supplied() ? "正常" : "不足") : "--"; }
