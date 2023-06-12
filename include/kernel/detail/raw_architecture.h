#ifndef DETAIL_RAW_ARCHITECTURE_H_
#define DETAIL_RAW_ARCHITECTURE_H_

#include "kernel/arch/architecture_type.h"
#include "kernel/config.h"

namespace yrui {
namespace detail {
    struct Raw_architecture {
        id_t id {};

        std::string name {};

        id_t fraction {};
        id_t mayor {};

        arch::Archtype type;

        int farming {};
        int max_farming {};
        int farming_growth {}; //< denote decreasing

        int trade {};
        int max_trade {};
        int trade_growth {}; //< denote decreasing

        int steady {};
        int max_steady {};

        int security {};
        int security_growth {}; //< denote decreasing

        int population {};
        int max_population {};
        int population_growth {}; //< denote decreasing

        int money {}; //< denote in debt
        int food {}; //< denote int debt

        int corruption {}; //< corrupt
        int efficiency {};

        int aristocrat {};
    };
} // of namespace detail
} // of namespace yrui

#endif // !DETAIL_RAW_ARCHITECTURE_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
