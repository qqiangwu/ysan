#ifndef YRUI_PERSON_POSITION_H_
#define YRUI_PERSON_POSITION_H_

/*!
 *
 * \file     Person/position.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    define person officail position.
 * \ingroup  Person
 *
 */

#include "kernel/component_fwd.h"

namespace yrui {
namespace person {
    namespace position_const {
        enum Position {
            nothing, // 2000
            duwei, // 2500
            xiaowei, // 3000
            subgeneral, // 3500
            general, // 4000
            marshal, // 4500
            leader // 5000
        };
    }

    using position_const::Position;

    Position max_position(Person* p);
} // of namespace person
} // of namespace yrui

#endif // !YRUI_PERSON_POSITION_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
