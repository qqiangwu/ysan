#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/person_impl.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

namespace Impl = yrui::person;

void Impl::Person::dismiss_troop()
{
    BOOST_ASSERT_MSG(has_troop(), "Cannot dismiss a none troop");
    MLOG() << format("%1% dismiss troop") % name();

    impl().lose_troop();
}

void Impl::Person::gain_troop(int n)
{
    auto& raw_ = impl().raw_;

    if (has_troop()) {
        //! 增加部队，并减少morale及drill
        const auto total = troop() + n;
        raw_.morale = (raw_.morale * raw_.troop + n * base_morale) / total;
        raw_.drill = (raw_.drill * raw_.troop + n * base_drill) / total;
        raw_.troop = total;

        MLOG() << format("%1% 的部队得到了补充") % name();
    } else {
        raw_.troop = n;
        raw_.morale = base_morale;
        raw_.drill = base_drill;

        MLOG() << format("%1% 获得了军队") % name();
    }

    MLOG() << format("%1% - T:%2%[D:%3% M:%4%]") % name() % troop() % drill() % morale();
}

bool Impl::Person::is_persuaded_by(Fraction* f) const noexcept
{
    BOOST_ASSERT(f);
    BOOST_ASSERT(fraction() != f);

    if (is_leader()) {
        MLOG() << "Try persuade leader";
        return politics() < 15 && !fraction()->is_alive();
    } else {
        MLOG() << "Try persuade trivial commander";
        return loyalty() < 10;
    }
}

void Impl::Person::killed_by(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("KillBy");
    BOOST_ASSERT(f);
    BOOST_ASSERT(is_alive());

    mgr().emit_person_killed(this, f);
    die();

    BOOST_ASSERT(!is_alive());
}

void Impl::Person::captured_by(Fraction* f)
{
    BOOST_ASSERT(is_in_office());
    //! do what?
    //! \todo   emit a signal
    mgr().emit_person_captured(this, f);
}

void Impl::Person::capitulate(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("Capitulate");
    MLOG() << format("%1% capitulate to %2%") % name() % f->name();

    if (is_in_office()) {
        leave_from_office();
    }

    join(f->capital());
    mgr().emit_person_capitulated(this, f);
}

void Impl::Person::release(Fraction* f)
{
    BOOST_LOG_NAMED_SCOPE("PersonReleased");

    /*!
     * \todo   move person->leave() to private method, and update state
     *         by handling the signal fraction_die.
     *         其实现在的逻辑挺乱的。因为leave实际上是person的主动动作。当一个
     *         势力灭亡时，调用人的leave方法，而改变人的状态，不如让人来响应信号。
     *         像join/leave/move_to这些方法应该是人的自发方法，reside等才是
     *         应该由fraction来调用的。否则，反正逻辑挺乱的。
     */
    auto frac = fraction();

    if (is_in_office() && loyalty() > 5) {
        BOOST_ASSERT(frac);
        BOOST_ASSERT(frac->is_alive());
        MLOG() << name() << " go back to capital " << frac->capital()->name();
        impl().set_stayed_city(frac->capital());
        mgr().emit_person_reside_arch(this, frac->capital());
    } else {
        MLOG() << name() << " leave fraction when released";

        if (is_in_office()) {
            leave_from_office();
        }
    }

    mgr().emit_person_released(this, f);
}

void Impl::Person::go_back()
{
    YPRE_CONDITION(is_active());

    BOOST_LOG_NAMED_SCOPE("GoBack");

    if (is_in_office()) {
        YCONSTRAINT(fraction()->is_alive());

        if (fraction() == stayed_city()->fraction()) {
            stayed_city()->on_person_back_(this);
        } else {
            reside(fraction()->capital());
        }
    }
}

//! 加入新的势力
//! 更新势力及自身信息
void Impl::Person::join(Architecture* arch)
{
    BOOST_LOG_NAMED_SCOPE("JoinFraction");
    BOOST_ASSERT(!is_in_office());

    impl().set_fraction(arch->fraction());
    impl().set_stayed_city(arch);

    mgr().emit_person_reside_arch(this, arch);
    mgr().emit_person_be_official(this, arch->fraction());

    BOOST_ASSERT(stayed_city() == arch);
    BOOST_ASSERT(fraction() == arch->fraction());
    BOOST_ASSERT(is_in_office());
}

//! 离开当前势力
void Impl::Person::leave_from_office()
{
    auto old = fraction();
    auto stayed = stayed_city();

    impl().leave_office();

    if (stayed->fraction() == old) {
        // BOOST_ASSERT(old->is_alive());
        // When called in the process of death of the fraction.
        mgr().emit_person_leave_arch(this, stayed_city());
    }

    mgr().emit_person_leave_offical(this, old);
}

//! 移动到新的城池
//! 更新城池及自身信息
void Impl::Person::move_to(Architecture* other)
{
    BOOST_ASSERT(is_active());
    BOOST_ASSERT(stayed_city() != other);

    MLOG() << format("%1% move from %2% to %3%") % name() % stayed_city()->name() % other->name();

    impl().set_stayed_city(other);
}

void Impl::Person::reside(Architecture* other)
{
    BOOST_LOG_NAMED_SCOPE("Reside");

    BOOST_ASSERT(is_in_office());
    BOOST_ASSERT(other);
    BOOST_ASSERT(other->fraction() == fraction());

    MLOG() << format("%1% move from %2% to %3%") % name() % stayed_city()->name() % other->name();

    auto old = stayed_city();

    impl().set_stayed_city(other);

    if (old->fraction() == fraction()) { // \fixme odd!
        mgr().emit_person_leave_arch(this, old);
    }

    mgr().emit_person_reside_arch(this, other);
}

//! 死亡事件由系统进行检查，我认为，这是不受人自身所控制的。
//! 人所做的只是清理资源
void Impl::Person::die()
{
    BOOST_LOG_NAMED_SCOPE("PersonDie");

    BOOST_ASSERT(is_active());
    BOOST_ASSERT(is_alive());

    MLOG() << format("%1% is going to die") % name();

    //! 清理自身资源
    if (has_troop()) {
        impl().lose_troop();
    }

    //! 分为在野武将与在野武将两种
    if (is_in_office()) {
        leave_from_office();
    }

    //! clear city: set unactive
    impl().set_stayed_city(nullptr);

    //! 设置死亡标识: set unalive
    impl().raw_.lifespan = 0;

    mgr().emit_person_die(this);

    BOOST_ASSERT(!stayed_city());
    BOOST_ASSERT(!fraction());
    BOOST_ASSERT(troop() == 0);
    BOOST_ASSERT(morale() == 0);
    BOOST_ASSERT(drill() == 0);
    BOOST_ASSERT(!is_alive());
}

//! 人物长大成人: 出现在某一城市
void Impl::Person::appear()
{
    BOOST_LOG_NAMED_SCOPE("PersonAppear");
    BOOST_ASSERT(!is_active());

    auto rel = relative();

    if (rel && rel->is_active()) {
        BOOST_LOG_NAMED_SCOPE("RelativeExist");

        //! put the person into active state
        impl().set_stayed_city(rel->stayed_city());
    } else {
        BOOST_ASSERT(showed_city());

        MLOG() << format("%1% appears in %2%") % name() % showed_city()->name();

        impl().set_stayed_city(showed_city());
    }

    mgr().emit_person_appear(this);

    BOOST_ASSERT(!is_in_office());
    BOOST_ASSERT(is_active());
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
