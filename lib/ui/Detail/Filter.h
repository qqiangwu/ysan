#ifndef YRUI_FRONTEND_FILTER_H_
#define YRUI_FRONTEND_FILTER_H_

/*!
 *
 * \brief   This is just a temporary solution, which will be revised later.
 *
 */

#include "kernel/component_fwd.h"
#include "kernel/detail/characteristic_type.h"
#include "kernel/person/position.h"
#include <QString>

namespace Bygone {
namespace Frontend {
    QString filter_position(yrui::person::Position val);
    QString filter_ability(yrui::Basic_ability val);

    QString filter_troop(yrui::Person* p);
    QString filter_morale(yrui::Person* p);
    QString filter_drill(yrui::Person* p);
    QString filter_supply(yrui::Person* p);
} // of namespace Frontend
} // of namespace Bygone

#endif // YRUI_FRONTEND_FILTER_H_
