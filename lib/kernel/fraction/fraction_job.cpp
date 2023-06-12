#include "kernel/arch/architecture.h"
#include "kernel/core/subsys.h"
#include "kernel/fraction/fraction.h"
#include "kernel/job/diplomacy_job.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"

//! \todo   add log

namespace yrui {
namespace fraction {
    namespace {
        inline Architecture* to_architecture(Person* p)
        {
            BOOST_ASSERT(p->is_in_office());
            return p->stayed_city();
        }
    } // of namespace unnamed

    /*! cease war with others
     */
    void Fraction::cease_war(Person* p, Fraction* other)
    {
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_cease_war>(p, other);
    }

    /*!
     * ally with other fraction
     */
    void Fraction::ally(Person* p, Fraction* other)
    {
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_ally>(p, other);
    }

    /*!
     * cease alliance
     */
    void Fraction::cease_alliance(Person* p, Fraction* other)
    {
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_cease_alliance>(p, other);
    }

    /*!
     * develop relation with other fraction.
     * \return  growth of intimation
     */
    void Fraction::develop_relation(Person* p, Fraction* other)
    {
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_develop_relation>(p, other);
    }

    /*! emperor: cease war with others
     * \return succeeded or failed
     */
    void Fraction::cease_war_by_emperor(Person* p, Fraction* other)
    {
        BOOST_ASSERT(has_emperor());
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_cease_war_by_emperor>(p, other);
    }

    /*!
     * ally with other fraction
     * \return succeeded or failed.
     */
    void Fraction::ally_by_emperor(Person* p, Fraction* other)
    {
        BOOST_ASSERT(has_emperor());
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != other->id());

        to_architecture(p)->do_job<job::Do_ally_by_emperor>(p, other);
    }

    /*!
     * make \a source intrade \a target
     * \return succeeded or failed.
     */
    void Fraction::intrade_by_emperor(Person* p, Fraction* source, Fraction* target)
    {
        BOOST_ASSERT(has_emperor());
        BOOST_ASSERT(p->fraction() == this);
        BOOST_ASSERT(id() != target->id());
        BOOST_ASSERT(id() != source->id());
        BOOST_ASSERT(source->id() != target->id());

        to_architecture(p)->do_job<job::Do_intrude_by_emperor>(p, target, source);
    }

    /*!
     *  conscript from all architectures and move them to capital.
     *  \todo    implementing this in the future version
     */
    int Fraction::conscript()
    {
        YRUI_NOT_IMPLEMENTED;
        return 0;
    }

    /*!
     * intrade a architecture.
     * \pre persons' distances from \target is below a specific value.
     *
     * \todo     implementing militaray system
     */
    Legion* Fraction::intrade(const std::vector<Person*>& troops, Architecture* target)
    {
        BOOST_LOG_NAMED_SCOPE("FractionIntrade");
        BOOST_ASSERT(!troops.empty());
        BOOST_ASSERT(target);

        MLOG() << format("Intrade from %1% to %2%") % troops.front()->stayed_city()->name() % target->name();

        auto& mmgr = use_subsys<military::Military_mgr>();

        auto from = troops.front()->stayed_city();
        auto legion = from->create_troop(target, troops);
        auto ret = legion.get();

        mmgr.go_battle(std::move(legion));

        return ret;
    }
}
} // of namespace yrui::fraction
