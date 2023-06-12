#include "kernel/arch/architecture_mgr.h"
#include "kernel/arch/architecture.h"
#include "kernel/core/timeline.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

namespace yrui {
namespace arch {
    Architecture* Architecture_mgr::get(id_t id) const
    {
        BOOST_ASSERT(id != null_id);
        BOOST_ASSERT_MSG(archs_.count(id), "id not exists!");

        return archs_.find(id)->second;
    }

    void Architecture_mgr::load(std::vector<uptr<Architecture>> archs)
    {
        finalize();

        MLOG() << "Load " << archs.size() << " archs";

        archs_.reserve(archs.size());
        arch_vec_.reserve(archs.size());

        for (auto& x : archs) {
            archs_.insert({ x->id(), x.get() });
            all_.push_back(x.get());
            arch_vec_.push_back(std::move(x));
        }
    }

    void Architecture_mgr::update() { }

    /*! defered construction. set references in architectures. */
    bool Architecture_mgr::init_impl()
    {
        BOOST_LOG_NAMED_SCOPE("InitArchMgr");
        BOOST_ASSERT(!archs_.empty());

        for (auto arch : all_) {
            arch->init_self();
        }

        install_signals_();

        return true;
    }

    /*! clear old data. */
    void Architecture_mgr::finalize_impl()
    {
        archs_.clear();
        arch_vec_.clear();
    }

    void Architecture_mgr::on_month_begin(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("ArchMgr");
        MLOG() << "on month begin";

        BOOST_ASSERT(is_initialized());

        // TODO
        // std::random_shuffle(all_.begin(), all_.end());

        for (auto arch : all_) {
            LOG_ATTACH_ATTR(Arch, [arch] { return arch->name(); });
            arch->on_month_begin(t);
        }
    }

    void Architecture_mgr::on_month_end(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("ArchMgr");
        MLOG() << "on month end";

        BOOST_ASSERT(is_initialized());

        for (auto arch : as_range()) {
            LOG_ATTACH_ATTR(Arch, [arch] { return arch->name(); });
            arch->on_month_end(t);
        }
    }

    void Architecture_mgr::on_season_begin(Timeline& t)
    {
        //! \todo   add harvest
        if (t.month() == 7) { }
    }

    void Architecture_mgr::on_season_end(Timeline& t) { }

    void Architecture_mgr::on_year_begin(Timeline& t) { }

    void Architecture_mgr::on_year_end(Timeline& t) { }

    bool Architecture_mgr::self_check() const { return {}; }

    void Architecture_mgr::install_signals_()
    {
        auto& pmgr = use_subsys<person::Person_mgr>();

        pmgr.person_reside_arch.connect(sig_level::mutual, [](Person* p, Architecture* a) { a->on_obtain_person(p); });

        pmgr.person_leave_arch.connect(sig_level::mutual, [](Person* p, Architecture* a) { a->on_remove_person(p); });

        pmgr.person_captured.connect(sig_level::mutual, [](Person* p, Fraction* f) {
            BOOST_ASSERT(p->fraction());
            BOOST_ASSERT(p->fraction() == p->stayed_city()->fraction());

            p->stayed_city()->on_remove_person(p);
        });

        auto& fmgr = use_subsys<fraction::Fraction_mgr>();

        fmgr.fraction_died.connect(sig_level::observer, [](Fraction* f) {
            for (auto a : f->archs()) {
                a->on_fraction_die(f);
            }
        });
    }

}
} // of namespace yrui::arch

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
