#include <boost/range/adaptors.hpp>

#include "foundation/contract.h"
#include "foundation/log_support.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person_mgr.h"

namespace yrui {

namespace fraction {
    void Fraction_mgr::on_month_begin(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("FractionMgr");
        MLOG() << "on month begin";

        BOOST_ASSERT(!fractions_.empty());

        auto rng = as_range();
        std::vector<Fraction*> fractions(rng.begin(), rng.end());
        // std::random_shuffle(fractions.begin(), fractions.end());

        BOOST_LOG_NAMED_SCOPE("Think");
        MLOG() << "Totally " << fractions.size() << " fractions";
        for (auto f : fractions) {
            LOG_ATTACH_ATTR(Frac, [f] { return f->name(); });
            MLOG() << format("%1%: %2% Archs | %3% Persons") % f->name() % f->arch_count() % f->person_count();
            f->think(t);
        }
    }

    void Fraction_mgr::on_month_end(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("FractionMgr");
        MLOG() << "on month end";
    }

    void Fraction_mgr::on_season_begin(Timeline& t) { }

    void Fraction_mgr::on_season_end(Timeline& t) { }

    void Fraction_mgr::on_year_begin(Timeline& t) { }

    void Fraction_mgr::on_year_end(Timeline& t) { }

    Fraction* Fraction_mgr::get(id_t id) const
    {
        BOOST_ASSERT(fractions_.count(id));

        return fractions_.find(id)->second;
    }

    void Fraction_mgr::load(std::vector<uptr<Fraction>> frac)
    {
        fractions_.clear();
        fractions_.reserve(frac.size());

        for (auto& x : frac) {
            fractions_.insert({ x->id(), x.get() });
            frac_vec_.push_back(std::move(x));
        }

        MLOG() << format("Load %1% fractions") % fractions_.size();
    }

    //! delayed delete fractions
    void Fraction_mgr::update()
    {
        while (!died_.empty()) {
            died_.pop();
        }
    }

    bool Fraction_mgr::init_impl()
    {
        BOOST_LOG_NAMED_SCOPE("InitFractionMgr");

        fraction_died.connect([this](Fraction* f) {
            fractions_.erase(f->id());
            died_.push(f);

            if (fractions_.size() == 1) {
                BOOST_LOG_NAMED_SCOPE("NationUnified");
                nation_unified(fractions_.begin()->second);
            }
        });

        for (auto f : fractions_ | boost::adaptors::map_values) {
            f->init_self();
        }

        install_signals_();

        return true;
    }

    void Fraction_mgr::finalize_impl() { }

    bool Fraction_mgr::self_check() const { return {}; }

    void Fraction_mgr::install_signals_()
    {
        auto& pmgr = use_subsys<person::Person_mgr>();

        pmgr.person_join_fraction.connect(sig_level::mutual, [](Person* p, Fraction* f) {
            BOOST_ASSERT(p);
            BOOST_ASSERT(f);

            if (f->is_alive()) {
                f->on_obtain_person(p);
            }
        });

        pmgr.person_leave_fraction.connect(sig_level::mutual, [](Person* p, Fraction* f) {
            BOOST_ASSERT(p);
            BOOST_ASSERT(f);

            if (f->is_alive()) {
                f->on_lose_person(p);
            }
        });

        auto& amgr = use_subsys<arch::Architecture_mgr>();

        amgr.ownership_transfer.connect(sig_level::mutual, [](Architecture* a, Fraction* from, Fraction* to) {
            BOOST_ASSERT(a);

            if (from && from->is_alive()) {
                from->on_lose_arch(a);
            }

            if (to) {
                BOOST_ASSERT(to->is_alive());
                to->on_obtain_arch(a);
            }
        });
    }

} // of namespace fraction
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
