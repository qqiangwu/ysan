#ifndef YRUI_CORE_COMMON_RANDUTIL_H_
#define YRUI_CORE_COMMON_RANDUTIL_H_

#include "kernel/config.h"

namespace yrui {
namespace common {
    //! generate a int in range [min, max].
    int randint(int min, int max);

    inline int rand_percent() { return randint(0, 100); }

    inline bool hit_percent(int percent) { return rand_percent() <= percent; }
}
}

#endif // YRUI_CORE_COMMON_RANDUTIL_H_
