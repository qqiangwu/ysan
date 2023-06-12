#ifndef YRUI_REPORT_MGR_H_
#define YRUI_REPORT_MGR_H_

#include "kernel/core/subsys.h"

namespace yrui {
namespace report {
    class Report_mgr : public Isubsys {
    public:
        Report_mgr() = default;

        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    private:
        DECLARE_MEMBER_LOGGER("ReportMgr");
    };
} // of namespace report
} // of namespace yrui

#endif // !YRUI_REPORT_MGR_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
