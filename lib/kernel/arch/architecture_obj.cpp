#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/core/ai_architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/arch_impl.h"
#include "kernel/person/person.h"
#include "kernel/report/report_mgr.h"

namespace yrui {
namespace arch {
    Architecture::Architecture(const yrui::detail::Raw_architecture& raw)
        : base_type(raw.id, *this, raw)
    {
        YPOST_CONDITION(id());
    }

    Architecture::~Architecture() = default;

    //! 初始化城池人物列表
    void Architecture::init_self()
    {
        BOOST_LOG_NAMED_SCOPE("InitArch");

        mgr_ = &use_subsys<Architecture_mgr>();
        logger_ = &mgr_->logger();

        impl().init_self();
    }

    void Architecture::on_month_begin(const Timeline& t)
    {
        base_type::on_month_begin(t);

        BOOST_ASSERT(num_in_city() == num_unworked() + num_worked());
    }

    namespace {
        inline int base(const Architecture* arch) { return arch->population() * arch->security() / 10000; }
    }

    //! work and 清理所有上月状态
    void Architecture::on_month_end(const Timeline& t)
    {
        MLOG() << format("%1% @ %2%") % name() % (fraction() ? fraction()->name() : "NULL");

        base_type::on_month_end(t);

        //! resource increasing if not beleaguered
        if (!has_beleaguered()) {
            BOOST_LOG_NAMED_SCOPE("NotBeleaguered");

            const auto vbase = base(this);

            if (auto mayor = this->mayor()) {
                BOOST_LOG_NAMED_SCOPE("MayorOnDuty");
                BOOST_ASSERT(fraction());

                if (type() != pass) {
                    //! money and food incr
                    auto m_incr = vbase * trade() / 5;
                    auto f_incr = vbase * farming() / 40;

                    //! contribution
                    //! \todo   设置税率
                    //! Non-capital and non-pass and connected cities will contribute
                    if (!is_capital() && capital_connected()) {
                        constexpr auto r = 0.4;

                        MLOG() << format("contribute M %1%/F %2%") % int(m_incr * r) % int(f_incr * r);

                        fraction()->contribute_money(int(m_incr * r));
                        fraction()->contribute_food(int(f_incr * r));

                        m_incr *= (1 - r);
                        f_incr *= (1 - r);
                    }

                    set_money(money() + m_incr);
                    set_food(food() + f_incr);

                    set_farming(farming() + mayor->farm_ability().val() / 3);
                    set_trade(trade() + mayor->trade_ability().val() / 3);

                    MLOG() << format("M/F up: %1%/%2%") % m_incr % f_incr;
                }

                set_steady(steady() + mayor->steady_ability().val() / 3);
                set_security(security() + mayor->security_ability().val() / 3);

                //! gain escuage
                impl().replenish_escuage();
            }

            if (type() != pass) {
                //! population increasing
                auto pop_incr = vbase;
                set_population(population() + pop_incr);

                MLOG() << format("People up: %1%") % pop_incr;
            }
        }

        if (fraction()) {
            BOOST_LOG_NAMED_SCOPE("BelongFraction");
            BOOST_LOG_NAMED_SCOPE("DoJob");

            //! 执行工作
            auto& reporter = use_subsys<report::Report_mgr>();
            for (auto& job : jobs()) {
                job->execute(reporter);
            }

            impl().clear_jobs();

            impl().beleaguered_ = false;

            impl().has_done_farm_ = false;
            impl().has_done_trade_ = false;
            impl().has_done_steady_ = false;
            impl().has_done_security_ = false;

            impl().calculate_troop();
            impl().calculate_connectivity(); //! \fixme bug here?
        }

        BOOST_ASSERT(jobs().empty());
        BOOST_ASSERT(num_out_of_city() == 0);
        BOOST_ASSERT(num_in_city() == num_all());
    }

    void Architecture::install_ai(uptr<Iarchitecture_ai> ai)
    {
        auto& ai_ = impl().ai_;

        ai_ = std::move(ai);
        ai_->take_control_of(this);
    }

    void Architecture::think(const Timeline& tl)
    {
        // BOOST_ASSERT(ai_);
        //! Arch that was just conquered from neutral fraction may
        //! have no ai

        if (auto& ai_ = impl().ai_) {
            MLOG() << format("%1% is thinking") % name();
            ai_->think(tl);
        }
    }
}
} // of namespace yrui::arch
