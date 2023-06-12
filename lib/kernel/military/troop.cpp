#include "kernel/military/troop.h"
#include "kernel/person/person.h"

namespace Impl = yrui::military;

int Impl::required_food(const std::vector<Person*>& troops)
{
    int ret {};

    for (auto t : troops) {
        ret += t->troop() / 5;
    }

    return ret;
}

int Impl::required_money(const std::vector<Person*>& troops)
{
    int ret {};

    for (auto t : troops) {
        ret += t->troop() / 5;
    }

    return ret;
}
