#ifndef YRUI_COMMON_PERSON_FILTER_H_
#define YRUI_COMMON_PERSON_FILTER_H_

#include "kernel/person/person.h"

namespace yrui {
namespace common {
    struct Is_combatable {
        bool operator()(Person* p) const noexcept { return p->is_troop_combatable(); }
    };
}
} // of namespace yrui::common

#endif // YRUI_COMMON_PERSON_FILTER_H_
