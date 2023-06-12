#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/impl/fraction_impl.h"
#include "kernel/person/person.h"

namespace yrui {
namespace fraction {
    namespace detail {
        bool is_heir(Person* p) noexcept
        {
            BOOST_ASSERT(p->fraction());
            return p->relative() && p->relative() == p->fraction()->leader();
        }
    }

    /*! change leader of the fraction. */
    void Fraction::set_leader(Person* p)
    {
        BOOST_LOG_NAMED_SCOPE("SetLeader");
        impl().leader_ = p;

        if (p) {
            YINVARIANT(p->is_in_office());
            YINVARIANT(p->fraction() == this);

            impl().name_ = p->name();
            mgr().emit_succession(this, p);
        }

        impl().invalidate();
    }

    void Fraction::set_heir(Person* p)
    {
        YPRE_CONDITION(detail::is_heir(p));
        YPRE_CONDITION(p->is_in_office());
        YPRE_CONDITION(p->fraction() == this);

        MLOG() << format("%1% become heir of %2%") % p->name() % name();
        impl().heir_ = p;
    }

    void Fraction::set_capital(Architecture* arch)
    {
        YPRE_CONDITION(!arch || arch->fraction() == this);

        impl().capital_ = arch;
    }

    void Fraction::get_seal() { }

    void Fraction::get_emperor() { }

    void Fraction::lose_seal() { }

    void Fraction::lose_emperor() { }

    void Fraction::set_controller(uptr<Fraction_controller> ctrl)
    {
        YPRE_CONDITION(ctrl);
        impl().controller_ = std::move(ctrl);
    }
}
} // of namespace yrui::fraction

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
