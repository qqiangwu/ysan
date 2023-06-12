#ifndef YRUI_DIPLOMACY_JOB_H_
#define YRUI_DIPLOMACY_JOB_H_

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include "kernel/job/job.h"

#define DEFINE_DIPLOMACY_JOB(job_name)                                                                                 \
    class job_name : public Diplomacy_job {                                                                            \
    public:                                                                                                            \
        job_name(Person* p, Fraction* target)                                                                          \
            : Diplomacy_job(p, target)                                                                                 \
        {                                                                                                              \
        }                                                                                                              \
        virtual bool execute(yrui::report::Report_mgr&) override;                                                      \
    }

namespace yrui {
namespace job {
    class Diplomacy_job : public Job {
    public:
        Diplomacy_job(Person* p, Fraction* target);

        Fraction* target() const;

    private:
        Fraction* target_;
    };

    class Do_intrude_by_emperor : public Diplomacy_job {
    public:
        Do_intrude_by_emperor(Person* p, Fraction* target, Fraction* related);

        Fraction* related() const;

    public:
        virtual bool execute(yrui::report::Report_mgr&) override;

    private:
        Fraction* related_;
    };

    DEFINE_DIPLOMACY_JOB(Do_cease_war);

    DEFINE_DIPLOMACY_JOB(Do_ally);

    DEFINE_DIPLOMACY_JOB(Do_cease_alliance);

    DEFINE_DIPLOMACY_JOB(Do_develop_relation);

    DEFINE_DIPLOMACY_JOB(Do_cease_war_by_emperor);

    DEFINE_DIPLOMACY_JOB(Do_ally_by_emperor);

} // of namespace job
} // of namespace yrui

//! implementation
namespace yrui {
namespace job {
    inline Diplomacy_job::Diplomacy_job(Person* p, Fraction* target)
        : Job(p)
        , target_(target)
    {
    }

    inline Do_intrude_by_emperor::Do_intrude_by_emperor(Person* p, Fraction* target, Fraction* related)
        : Diplomacy_job(p, target)
        , related_(related)
    {
    }

    inline Fraction* Diplomacy_job::target() const { return target_; }

    inline Fraction* Do_intrude_by_emperor::related() const { return related_; }
} // of namespace job
} // of namespace yrui

#undef DEFINE_DIPLOMACY_JOB
#endif // !YRUI_DIPLOMACY_JOB_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
