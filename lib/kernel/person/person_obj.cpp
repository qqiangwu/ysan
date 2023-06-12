#include "kernel/arch/architecture.h"
#include "kernel/common/common_util.h"
#include "kernel/common/randutil.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/person_impl.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"

namespace Impl = yrui::person;

Impl::Person::Person(const yrui::detail::Raw_person& raw) noexcept
    : base_type(raw.id, *this, raw)
{
}

Impl::Person::~Person() = default;

void Impl::Person::init_self()
{
    BOOST_LOG_NAMED_SCOPE("InitPerson");

    mgr_ = &use_subsys<Person_mgr>();
    logger_ = &mgr_->logger();

    impl().init_self();

    YINVARIANT(impl().is_valid());
}

void Impl::Person::on_month_begin(const Timeline& tl)
{
    YINVARIANT(impl().is_valid());
    YPRE_CONDITION(is_active());

    base_type::on_month_begin(tl);

    if (!is_in_office()) {
        BOOST_LOG_NAMED_SCOPE("NotOffice");
        MLOG() << "Try self-persuade";

        auto frac = stayed_city()->fraction();
        if (frac && common::hit_percent(frac->leader()->politics().val())) {
            join(stayed_city());
        }
    } else if (has_troop()) {
        BOOST_LOG_NAMED_SCOPE("InOfficeTrooped");

        auto city = stayed_city();
        auto& raw_ = impl().raw_;
        auto& is_supplied_ = impl().is_supplied_;

        if (!(is_supplied_ = city->ask_for_supply(troop()))) {
            //! 给养不足，能力下降，出现逃兵
            MLOG() << format("%1% deficient supply") % name();

            if (raw_.troop > 0) {
                raw_.troop -= 1000 / leadership().val();
                if (raw_.troop < 0)
                    raw_.troop = 0;
            }
            if (raw_.drill > 0) {
                raw_.drill -= 100 / leadership().val();
                if (raw_.drill < 0)
                    raw_.drill = 0;
            }
            if (raw_.morale > 0) {
                raw_.morale -= 100 / leadership().val();
                if (raw_.morale < 0)
                    raw_.morale = 0;
            }

            if (raw_.troop == 0) {
                impl().lose_troop();
            }
        }

        //! 自动恢复
        if (has_troop() && is_supplied() && troop() != max_troop()) {
            auto n = leadership().val() * auto_replenish_rate;
            auto ntry = 10;

            while (--ntry >= 0 && !city->can_escauge(n)) {
                n = n / 2;
            }

            if (ntry >= 0 && n != 0) {
                city->consume_escuage(n);
                raw_.troop = std::min<int>(max_troop(), raw_.troop + n);
            }

            MLOG() << format("%1% replenish %2% slightly") % name() % n;
        }
    }

    YINVARIANT(impl().is_valid());
}

void Impl::Person::on_month_end(const Timeline& tl)
{
    YINVARIANT(impl().is_valid());
    YPRE_CONDITION(is_active());

    base_type::on_month_end(tl);

    //! 改变忠诚度
    if (is_in_office()) {
        BOOST_LOG_NAMED_SCOPE("InOffice");
        if (stayed_city()->capital_connected()) {
            impl().set_loyalty(loyalty() + 1);
        }
    } else {
        BOOST_LOG_NAMED_SCOPE("NotOffice");
        //! 选择是否移动到其他城市（对于在野武将）
        if (common::hit_percent(40)) {
            //! move
            auto adj = common::adjacent(stayed_city());
            if (auto size = adj.size()) {
                move_to(adj[common::randint(0, size - 1)]);
            }
        }
    }

    YINVARIANT(impl().is_valid());
}
