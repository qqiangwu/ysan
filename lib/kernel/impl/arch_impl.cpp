#include "kernel/impl/arch_impl.h"
#include "kernel/arch/architecture.h"
#include "kernel/common/common_util.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

#include <numeric>

namespace yrui {
namespace aux {
    Pimpl<Architecture>::implementation::implementation(Architecture& self_, const Raw_architecture& raw) noexcept
        : self(self_)
        , raw_(raw)
    {
    }

    void Pimpl<Architecture>::implementation::init_self()
    {
        if (raw_.mayor) {
            mayor_ = common::find_person(raw_.mayor);
        }

        if (raw_.fraction) {
            fraction_ = common::find_fraction(raw_.fraction);
        }

        if (fraction_) {
            yrui::core()->add_post_initializer([this] {
                auto& persons = use_subsys<person::Person_mgr>();

                for (auto p : persons.appeared()) {
                    if (p->fraction() == fraction_ && p->stayed_city() == &self) {
                        all_.insert(p);
                        in_city_.insert(p);
                        has_worked_.insert({ p, false });
                    }
                }

                calculate_troop();
                calculate_connectivity();

                MLOG() << format("Find %1% persons @ %2%") % self.num_all() % self.name();
            });
        }
    }

    void Pimpl<Architecture>::implementation::replenish_escuage()
    {
        const int max_escuage = self.population() / 10;
        const int delta = max_escuage - self.escuage();
        if (delta > 0) {
            escuage_ += delta;
            raw_.population -= delta * 2;
        }
    }

    void Pimpl<Architecture>::implementation::abort_jobs()
    {
        for (auto& item : has_worked_) {
            item.second = false;
        }

        jobs_.clear();
    }

    void Pimpl<Architecture>::implementation::calculate_troop()
    {
        troop_num_
            = std::accumulate(in_city_.begin(), in_city_.end(), 0, [](int acc, Person* p) { return acc + p->troop(); });
    }

    void Pimpl<Architecture>::implementation::clear_state()
    {
        all_.clear();
        out_city_.clear();
        has_worked_.clear();
        jobs_.clear();

        troop_num_ = 0;
        connected_to_capital_ = false;
    }

    void Pimpl<Architecture>::implementation::calculate_connectivity()
    {
        YPRE_CONDITION(fraction_);

        connected_to_capital_ = common::map().is_connected(self.fraction()->capital(), &self);
    }

    ylogger::logger_type& Pimpl<Architecture>::implementation::logger() const { return self.logger(); }
}
} // of namespace yrui::aux
