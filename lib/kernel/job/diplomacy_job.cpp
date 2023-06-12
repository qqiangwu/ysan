#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

#include "kernel/arch/architecture.h"
#include "kernel/core/timeline.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/job/diplomacy_job.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"
#include "kernel/report/report_mgr.h"

namespace yrui {
namespace job {

    bool Do_intrude_by_emperor::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_cease_war::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_ally::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_cease_alliance::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_develop_relation::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_cease_war_by_emperor::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }

    bool Do_ally_by_emperor::execute(yrui::report::Report_mgr&)
    {
        YRUI_NOT_IMPLEMENTED;
        return true;
    }
} // of namespace job
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
