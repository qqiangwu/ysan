#include "kernel/person/position.h"
#include "kernel/person/person.h"

namespace Impl = yrui::person;

Impl::Position Impl::max_position(Person* p)
{
    auto val = std::min<int>(p->feat() / 100, position_const::leader);

    return static_cast<Position>(val);
}
