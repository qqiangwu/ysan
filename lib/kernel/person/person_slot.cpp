#include "kernel/impl/person_impl.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

namespace Impl = yrui::person;

/*!
 * \brief Impl::Person::on_fraction_die
 *
 * Silently clear state and emit nothing.
 */
void Impl::Person::on_fraction_die(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("PersonOnFracDie");

    YPRE_CONDITION(f == fraction());

    impl().leave_office();
    mgr().emit_person_leave_offical(this, f);

    YPOST_CONDITION(fraction() == nullptr);
}

void Impl::Person::on_become_leader(Fraction* f)
{
    YPRE_CONDITION(f == fraction());

    //! 更改自己属性
    auto& raw_ = impl().raw_;

    raw_.loyalty = -1;
    raw_.max_loyalty = -1;

    //! 更改最大兵力
    impl().set_position(position_const::leader);

    YPOST_CONDITION(is_leader());
}
