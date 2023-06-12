#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/common/common_util.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

#include "kernel/impl/fraction_impl.h"

namespace Impl = yrui::fraction;

Impl::Fraction::Fraction(const yrui::detail::Raw_fraction& raw) noexcept
    : base_type(raw.id, *this, raw)
{
    YPOST_CONDITION(id());
}

Impl::Fraction::~Fraction() = default;

void Impl::Fraction::init_self()
{
    BOOST_LOG_NAMED_SCOPE("InitFraction");

    mgr_ = &use_subsys<Fraction_mgr>();
    logger_ = &mgr_->logger();

    auto& raw_ = impl().raw_;

    impl().leader_ = common::find_person(raw_.leader);
    impl().capital_ = common::find_arch(raw_.capital);
    impl().name_ = leader()->name();

    yrui::core()->add_post_initializer([this] {
        auto& persons_ = impl().persons_;
        for (auto p : use_subsys<person::Person_mgr>().as_range()) {
            if (p->fraction() == this) {
                persons_.insert(p);
            }
        }

        auto& archs_ = impl().archs_;
        for (auto a : use_subsys<arch::Architecture_mgr>().as_range()) {
            if (a->fraction() == this) {
                archs_.insert(a);
            }
        }

        impl().invalidate();
        impl().calculate_troop();

        MLOG() << format("Find [%1% P/%2% A] @ %3%") % persons_.size() % archs_.size() % name();
    });
}
