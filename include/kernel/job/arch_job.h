#ifndef YRUI_ARCH_JOB_H_
#define YRUI_ARCH_JOB_H_

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include "kernel/job/job.h"

namespace yrui {
namespace job {
    class Arch_job : public Job {
    public:
        Arch_job(Person* p, Architecture* arch);

        Architecture* architecture() const;

    private:
        Architecture* arch_;
    };

    class Do_farming : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_trade : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_security : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_steady : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_search : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_conscript : public Arch_job {
    public:
        using Arch_job::Arch_job;

        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_train : public Arch_job {
    public:
        using Arch_job::Arch_job;
        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_replenish : public Arch_job {
    public:
        using Arch_job::Arch_job;
        virtual bool execute(yrui::report::Report_mgr&) override;
    };

    class Do_transport : public Arch_job {
    public:
        Do_transport(Person* p, Architecture* source, Architecture* target, int m, int f);
        virtual ~Do_transport();

        virtual bool execute(yrui::report::Report_mgr&) override;

    private:
        Architecture* target_;
        int money_;
        int food_;
    };

} // of namespace job
} // of namespace yrui

//! implementation
namespace yrui {
namespace job {
    inline Arch_job::Arch_job(Person* p, Architecture* arch)
        : Job(p)
        , arch_(arch)
    {
    }

    inline Architecture* Arch_job::architecture() const { return arch_; }
} // of namespace job
} // of namespace yrui
#endif // !YRUI_ARCH_JOB_H_

#undef USE_BASE_CONSTRUCTOR
/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
