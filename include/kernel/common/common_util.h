#ifndef YRUI_CORE_COMMON_UTIL_H_
#define YRUI_CORE_COMMON_UTIL_H_

#include "kernel/component_fwd.h"
#include "kernel/resource/map.h"
#include "kernel/resource/string_table.h"

namespace yrui {
namespace common {
    const resource::String_table& table();
    const resource::Map& map();

    Person* find_person(id_t id);
    Architecture* find_arch(id_t id);
    Fraction* find_fraction(id_t id);

    Fraction_controller* get_controller(Architecture* arch);
    Fraction_controller* get_controller(Legion* lg);

    std::vector<Architecture*> adjacent(Architecture* arch);
}
}

#endif // YRUI_CORE_COMMON_UTIL_H_
