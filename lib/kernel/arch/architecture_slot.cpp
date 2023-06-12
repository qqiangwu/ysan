#include "kernel/arch/architecture.h"
#include "kernel/impl/arch_impl.h"
#include "kernel/person/person.h"

namespace yrui {
namespace arch {
    //! 势力中武将加入此城池
    void Architecture::on_obtain_person(Person* p)
    {
        BOOST_LOG_NAMED_SCOPE("AddPerson");

        BOOST_ASSERT(p);
        BOOST_ASSERT(p->fraction() == fraction());
        BOOST_ASSERT(p->is_in_office());
        BOOST_ASSERT(p->stayed_city() == this);

        impl().all_.insert(p);
        impl().in_city_.insert(p);
        impl().has_worked_.insert({ p, true }); // so comes abort_job bug

        impl().calculate_troop();

        MLOG() << format("%1% obtain person %2%") % name() % p->name();
    }

    void Architecture::on_remove_person(Person* p)
    {
        BOOST_ASSERT(p);

        BOOST_ASSERT(p->stayed_city() == this || p->is_in_office());
        BOOST_ASSERT(detail::contains(all(), p));

        //! remove mayor or minister
        if (mayor() == p)
            set_mayor(nullptr);

        impl().all_.erase(p);
        if (!impl().in_city_.erase(p))
            impl().out_city_.erase(p);
        impl().has_worked_.erase(p);

        impl().calculate_troop();

        MLOG() << format("City %1% lost person %2%") % name() % p->name();
    }

    void Architecture::on_fraction_die(Fraction* f)
    {
        BOOST_LOG_NAMED_SCOPE("ArchOnFracDie");

        set_mayor(nullptr);

        impl().all_.clear();
        impl().in_city_.clear();
        impl().out_city_.clear();

        transfer_to(nullptr);
    }

    /* go out to do something */
    void Architecture::on_person_go_out_(Person* p)
    {
        BOOST_ASSERT(p->fraction() == fraction());
        BOOST_ASSERT(p->stayed_city() == this);
        BOOST_ASSERT(detail::contains(all(), p));
        BOOST_ASSERT(detail::contains(in_city(), p));
        BOOST_ASSERT(!detail::contains(out_of_city(), p));

        MLOG() << p->name() << " leave arch " << name();

        impl().in_city_.erase(p);
        impl().out_city_.insert(p);

        impl().calculate_troop();

        BOOST_ASSERT(detail::contains(all(), p));
        BOOST_ASSERT(!detail::contains(in_city(), p));
        BOOST_ASSERT(detail::contains(out_of_city(), p));
    }

    /* go back to city */
    void Architecture::on_person_back_(Person* p)
    {
        YPRE_CONDITION(p->fraction() == fraction());
        YPRE_CONDITION(p->stayed_city() == this);
        YPRE_CONDITION(detail::contains(all(), p));
        YPRE_CONDITION(!detail::contains(in_city(), p));
        YPRE_CONDITION(detail::contains(out_of_city(), p));

        MLOG() << p->name() << " return arch " << name();

        impl().out_city_.erase(p);
        impl().in_city_.insert(p);
        impl().calculate_troop();

        if (p->has_troop() && p->is_troop_crashed()) {
            p->morale_up(10);
        }

        BOOST_ASSERT(p->fraction() == fraction());
        BOOST_ASSERT(p->stayed_city() == this);
        BOOST_ASSERT(detail::contains(all(), p));
        BOOST_ASSERT(detail::contains(in_city(), p));
        BOOST_ASSERT(!detail::contains(out_of_city(), p));
    }
}
} // of namespace yrui::arch
