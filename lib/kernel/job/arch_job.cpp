#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include "kernel/arch/architecture.h"
#include "kernel/common/randutil.h"
#include "kernel/job/arch_job.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"
#include "kernel/report/report_mgr.h"

namespace yrui {
namespace job {
    //! 所有工作都会被放到相应城市的任务队列中去，这些任务会在每月结束时
    //! 被执行

    using boost::adaptors::filtered;
    using boost::range::push_back;

    bool Do_farming::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoFarm");

        BOOST_ASSERT(architecture()->has_done_farm());

        auto p = person();
        auto a = architecture();

        auto val = p->farm_ability().val();

        if (a->money() >= val) {
            a->consume_money(val);
            a->set_farming(a->farming() + val);

            p->gain_politics_exp(3);
            p->add_feat(5);
        }

        NLOG(architecture()->logger()) << format("%1% at %2% do farm [%3% Up]") % p->name() % architecture()->name()
                % val;

        YRUI_UNUSED_VAR(reporter);
        return true;
    }

    bool Do_trade::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoTrade");
        BOOST_ASSERT(architecture()->has_done_trade());
        auto p = person();
        auto a = architecture();

        auto val = p->trade_ability().val();

        if (a->money() >= val) {
            a->consume_money(val);
            a->set_trade(a->trade() + val);

            p->gain_politics_exp(3);
            p->add_feat(5);
        }

        NLOG(architecture()->logger()) << format("%1% at %2% do trade [%3% Up]") % p->name() % architecture()->name()
                % val;

        YRUI_UNUSED_VAR(reporter);

        return true;
    }

    bool Do_security::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoSecurity");
        BOOST_ASSERT(architecture()->has_done_security());

        auto p = person();
        auto a = architecture();

        auto val = p->security_ability().val();

        if (a->money() >= val) {
            a->consume_money(val);
            a->set_security(a->security() + val);

            p->gain_politics_exp(3);
            p->gain_leadership_exp(1);
            p->add_feat(5);
        }

        NLOG(architecture()->logger()) << format("%1% at %2% do security [%3% Up]") % p->name() % architecture()->name()
                % val;

        YRUI_UNUSED_VAR(reporter);

        return true;
    }

    bool Do_steady::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoSteady");
        BOOST_ASSERT(architecture()->has_done_steady());

        auto p = person();
        auto a = architecture();

        auto val = p->farm_ability().val();

        if (a->money() >= val) {
            a->consume_money(val);
            a->set_steady(a->steady() + val);

            p->gain_politics_exp(3);
            p->gain_leadership_exp(1);
            p->add_feat(5);
        }

        NLOG(architecture()->logger()) << format("%1% at %2% do steady [%3% Up]") % p->name() % architecture()->name()
                % val;

        YRUI_UNUSED_VAR(reporter);

        return true;
    }

    bool Do_search::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoSearch");

        auto& mgr = use_subsys<person::Person_mgr>();

        auto a = architecture();
        auto p = person();

        NLOG(architecture()->logger()) << format("%1% @ %2% do search") % p->name() % architecture()->name();

        std::vector<Person*> found;

        auto in_city = [a](Person* p) { return p->stayed_city() == a; };

        push_back(found, mgr.out_of_office() | filtered(in_city));

        NLOG(architecture()->logger()) << found.size() << " persons have been found";

        if (!found.empty()) {
            const auto sample = common::randint(0, found.size() - 1);
            auto np = found[sample];

            NLOG(architecture()->logger()) << format("%1% try to persuade %2%") % p->name() % np->name();

            if (common::hit_percent(p->politics().val())) {
                NLOG(architecture()->logger()) << "Persuade succeeded";

                np->join(a);

                p->gain_politics_exp(2);
                p->add_feat(5);
            }
        }

        YRUI_UNUSED_VAR(reporter);

        return true;
    }

    ///////////////////////////////////////////////////////////////////////
    bool Do_conscript::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoConscript");

        auto a = architecture();
        auto p = person();

        NLOG(architecture()->logger()) << format("%1% @ %2% do conscript") % p->name() % a->name();

        auto cn = std::min(p->leadership().val() * 20, a->escuage());

        while (cn != 0 && !a->can_escauge(cn))
            cn = cn * 2 / 3;

        if (cn != 0) {
            a->consume_escuage(cn);

            p->gain_troop(cn);
            p->add_feat(5);

            NLOG(architecture()->logger()) << format("%2% @ %1% conscript Troop %3%") % a->name() % p->name() % cn;
        }

        YRUI_UNUSED_VAR(reporter);
        return true;
    }

    bool Do_train::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoTrain");
        BOOST_ASSERT(person()->has_troop());
        BOOST_ASSERT(person()->is_supplied());

        auto p = person();
        auto val = p->train_ability().val();

        auto morale_up = val * 10 / std::max(p->morale(), 10);
        auto drill_up = val * 10 / std::max(p->drill(), 10);

        p->morale_up(morale_up);
        p->drill_up(drill_up);

        p->gain_leadership_exp(1);
        p->add_feat(1);

        NLOG(architecture()->logger()) << format("%1% @ %2% trained his troop[M+%3% D+%4%]") % p->name()
                % architecture()->name() % morale_up % drill_up;

        YRUI_UNUSED_VAR(reporter);
        return true;
    }

    bool Do_replenish::execute(yrui::report::Report_mgr& reporter)
    {
        BOOST_LOG_NAMED_SCOPE("DoReplenish");
        BOOST_ASSERT(person()->has_troop());

        auto a = architecture();
        auto p = person();

        NLOG(architecture()->logger()) << format("%1% @ %2% do replenish") % p->name() % architecture()->name();

        auto cn = std::min(p->leadership().val() * 20, a->escuage());

        while (cn != 0 && !a->can_escauge(cn))
            cn = cn * 2 / 3;

        if (cn != 0) {
            a->consume_escuage(cn);

            p->gain_troop(cn);
            p->add_feat(1);

            NLOG(architecture()->logger()) << format("%2% @ %1%replenish Troop %3%") % a->name() % p->name() % cn;
        }

        YRUI_UNUSED_VAR(reporter);
        return true;
    }

    Do_transport::Do_transport(Person* p, Architecture* source, Architecture* target, int m, int f)
        : Arch_job(p, source)
        , target_(target)
        , money_(m)
        , food_(f)
    {
        source->on_person_go_out_(p);
    }

    Do_transport::~Do_transport() { person()->go_back(); }

    bool Do_transport::execute(yrui::report::Report_mgr&)
    {
        BOOST_LOG_NAMED_SCOPE("DoTransport");
        auto p = person();
        auto a = architecture();
        auto t = target_;

        if (t->fraction() == p->fraction()) {
            if (a->money() >= money_ && a->food() >= food_) {
                a->consume_money(money_);
                a->consume_food(food_);

                t->set_money(t->money() + money_);
                t->set_food(t->food() + food_);

                p->gain_leadership_exp(1);
                p->add_feat(1);

                NLOG(architecture()->logger()) << format("%1% transport[%4%/%5%] from %2% to %3%") % p->name()
                        % a->name() % t->name() % money_ % food_;
            }
        }

        return true;
    }
} // of namespace job
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
