#ifndef YRUI_JOB_H_
#define YRUI_JOB_H_

#include "kernel/component_fwd.h"
#include "kernel/config.h"

namespace yrui {
namespace job {
    class Job {
    public:
        virtual ~Job() = default;

        virtual bool execute(yrui::report::Report_mgr&) = 0;

    public:
        Person* person() const { return p_; }

    public:
        Job(Person* p)
            : p_(p)
        {
        }

    private:
        Person* p_;
    };

    template <class JobKind, class... Args> inline uptr<JobKind> make_job(Args&&... args)
    {
        return make_uptr<JobKind>(std::forward<Args>(args)...);
    }
} // of namespace job
} // of namespace yrui
#endif // !YRUI_JOB_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
