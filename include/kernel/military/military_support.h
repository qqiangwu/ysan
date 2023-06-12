#ifndef YRUI_CORE_MILITARY_SUPPORT_H_
#define YRUI_CORE_MILITARY_SUPPORT_H_

#include "kernel/component_fwd.h"

namespace yrui {
namespace military {
    class Military_support {
    public:
        static void combat(Person* attacker, Person* defensor, bool is_defence);
    };
}
}

#endif // YRUI_CORE_MILITARY_SUPPORT_H_
