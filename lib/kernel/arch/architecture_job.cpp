#include "kernel/arch/architecture.h"
#include "kernel/impl/arch_impl.h"
#include "kernel/job/arch_job.h"
#include "kernel/person/person.h"

namespace yrui {
namespace arch {
    /*! develop farming.
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return  growth of farming.
     */
    void Architecture::develop_farming(Person* p)
    {
        BOOST_ASSERT(!has_done_farm());

        do_job<job::Do_farming>(p, this);

        impl().has_done_farm_ = true;

        MLOG() << format("%1% @ %2% dev farm") % p->name() % name();
    }

    /*! develop trade.
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return growth of trade
     */
    void Architecture::develop_trade(Person* p)
    {
        BOOST_ASSERT(!has_done_trade());

        do_job<job::Do_trade>(p, this);

        impl().has_done_trade_ = true;

        MLOG() << format("%1% @ %2% dev trade") % p->name() % name();
    }

    /*! develop security
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return growth of security
     */
    void Architecture::develop_security(Person* p)
    {
        BOOST_ASSERT(!has_done_security());

        do_job<job::Do_security>(p, this);

        impl().has_done_security_ = true;

        MLOG() << format("%1% @ %2% dev security") % p->name() % name();
    }

    /*! develop steady
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return growth of steady
     */
    void Architecture::develop_steady(Person* p)
    {
        BOOST_ASSERT(!has_done_steady());

        do_job<job::Do_steady>(p, this);

        impl().has_done_steady_ = true;

        MLOG() << format("%1% @ %2% dev steady") % p->name() % name();
    }

    /*!
     *  search for persons out of office in this city
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return searching result
     */
    void Architecture::search_person(Person* p)
    {
        do_job<job::Do_search>(p, this);

        MLOG() << format("%1% @ %2% dev search") % p->name() % name();
    }

    /*!
     *  conscript from this architecutre.
     *
     *  \pre     \a p has not worked
     *  \post    \a p worked
     *  \return  number of new soldiers.
     */
    void Architecture::conscript(Person* p)
    {
        do_job<job::Do_conscript>(p, this);

        MLOG() << format("%1% @ %2% dev conscript") % p->name() % name();
    }

    /*!
     * \brief 进行征兵，以大量恢复军队数目。
     */
    void Architecture::replenish(Person* p)
    {
        do_job<job::Do_replenish>(p, this);

        MLOG() << format("%1% @ %2% dev replenish") % p->name() % name();
    }

    void Architecture::train(Person* p)
    {
        do_job<job::Do_train>(p, this);

        MLOG() << format("%1% @ %2% dev train") % p->name() % name();
    }

    void Architecture::transport(Person* p, Architecture* target, int money, int food)
    {
        BOOST_ASSERT(p->has_troop());
        BOOST_ASSERT(p->fraction() == target->fraction());
        BOOST_ASSERT(p->stayed_city() != target);

        do_job<job::Do_transport>(p, this, target, money, food);

        MLOG() << format("%1% @ %2% dev transport to %3% [M:%4% F:%5%]") % p->name() % name() % target->name() % money
                % food;
    }
}
} // of namespace yrui::arch
