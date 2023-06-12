#ifndef YRUI_DETAIL_RAW_FRACTION_H_
#define YRUI_DETAIL_RAW_FRACTION_H_

#include "kernel/config.h"

namespace yrui {
namespace detail {
    struct Raw_fraction {
        id_t id {};
        id_t leader {};
        id_t capital {};

        bool has_seal {};
        bool has_emperor {};

        int money {};
        int food {};
    };
} // of namespace detail
} // of namespace yrui

#endif // !YRUI_DETAIL_RAW_FRACTION_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
