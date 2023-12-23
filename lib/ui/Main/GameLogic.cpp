#include <type_traits>

#include "Debugger.h"
#include "Form/ByScreen.h"
#include "Main/Byapp.h"
#include "Main/GameLogic.h"
#include "ScreenView/Mapview.h"

#include "Widget/GraphicsCity.h"
#include "Widget/PromoteSelect.h"
#include "Widget/SelectDialog.h"
#include "Widget/TransportDialog.h"

#include "Adaptor/ArchItemModel.h"
#include "Adaptor/FractionItemModel.h"
#include "Adaptor/JobItemModel.h"
#include "Adaptor/PersonItemModel.h"

#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    namespace {
        template <class GObj>
        inline typename std::enable_if<std::is_base_of<yrui::Game_obj, GObj>::value, QString>::type to_n(GObj* p)
        {
            return p->name().c_str();
        }
    }

    GameLogic::GameLogic(Byapp* app)
        : QObject()
        , app_(app)
    {
    }

    ///////////////////////////////////////////////////////////////////////
    SelectDialog* GameLogic::dialog_() { return app_->screen()->main()->selectDialog(); }

    yrui::Architecture* GameLogic::currentCity_() { return app_->screen()->main()->currentCity()->arch(); }

    ///////////////////////////////////////////////////////////////////////
    GameLogic::Action_power_checker GameLogic::check_ap_()
    {
        auto ctrl = app_->controller();
        if (ctrl->has_ap()) {
            return Action_power_checker(app_);
        } else {
            app_->reportAll("Error", "行动力不足");
            return Action_power_checker(nullptr);
        }
    }

    GameLogic::Action_power_checker::~Action_power_checker()
    {
        if (this->is_ok() && this->is_commited()) {
            app_->controller()->consume_ap();
            app_->screen()->main()->updateInfoBar();
        }
    }

    ///////////////////////////////////////////////////////////////////////
#define IMPLEMENT_JOB(job)                                                                                             \
    void GameLogic::job()                                                                                              \
    {                                                                                                                  \
        auto checker = this->check_ap_();                                                                              \
        if (checker.is_ok()) {                                                                                         \
            [[maybe_unused]] auto select = dialog_();                                                                                   \
            [[maybe_unused]] auto city = currentCity_();                                                                                \
            BOOST_ASSERT(city != nullptr);

#define END_IMP()                                                                                                      \
    }                                                                                                                  \
    }

#define IMPLEMENT_SPY(job)                                                                                             \
    void GameLogic::job()                                                                                              \
    {                                                                                                                  \
        auto select = dialog_();                                                                                       \
        auto city = currentCity_();                                                                                    \
        BOOST_ASSERT(city != nullptr);

#define END_SPY() }

    ////////////////////////////////////////////////////////////////////////
    IMPLEMENT_JOB(onFarm)
    {
        BOOST_ASSERT(!city->has_done_farm());

        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% do farming in %2%") % p->name() % city->name();

            city->develop_farming(p);

            app_->reportPerson(p, QString("%1: 农业乃国之根本").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onTrade)
    {
        BOOST_ASSERT(!city->has_done_trade());

        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% do trade in %2%") % p->name() % city->name();
            city->develop_trade(p);

            app_->reportPerson(p, QString("%1: 天下交而万物通").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onSteady)
    {
        BOOST_ASSERT(!city->has_done_steady());

        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% do steady in %2%") % p->name() % city->name();
            city->develop_steady(p);

            app_->reportPerson(p, QString("%1: 高筑墙").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onSecurity)
    {
        BOOST_ASSERT(!city->has_done_security());

        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% do security in %2%") % p->name() % city->name();
            city->develop_security(p);

            app_->reportPerson(p, QString("%1: 得民心者得天下").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onTransport)
    {
        if (auto a = select->exec_one(ArchItemModel::excludes(city->fraction(), city))) {
            if (city->money() == 0 && city->food() == 0) {
                app_->reportAll("错误", "资源不足");
                return;
            }

            if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
                qDebug() << yrui::format("%1% do transport from %2% to %3%") % p->name() % city->name() % a->name();

                TransportDialog dialog(nullptr, city->money(), city->food());

                if (dialog.exec() == QDialog::Rejected) {
                    return;
                }

                city->transport(p, a, dialog.money(), dialog.food());

                app_->reportEvent("transport", QString("%1: 天之道，损有余而补不足").arg(p->name().c_str()));

                checker.commit();
            }
        }
    }
    END_IMP()

    //////////////////////////////////////////////////////////////////////
    IMPLEMENT_JOB(onSearch)
    {
        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% do search in %2%") % p->name() % city->name();
            city->search_person(p);

            app_->reportEvent("search", QString("%1: 人才何在").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onMayor)
    {
        if (auto p = select->exec_one(new PersonItemModel(city, not_work))) {
            qDebug() << yrui::format("%1% take on mayor in %2%") % p->name() % city->name();
            city->set_mayor(p);

            app_->reportEvent("mayor", QString("%1: 承君深恩，敢不赴死？").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onPromote)
    {
        if (auto p = select->exec_one(PersonItemModel::canPromote(city))) {
            PromoteSelect sel(p->position(), yrui::person::max_position(p));

            if (sel.exec() == QDialog::Rejected)
                return;

            p->set_position(sel.result());

            app_->reportEvent("promote", QString("%1: 承君深恩，敢不赴死？").arg(p->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onMove)
    {
        auto f = city->fraction();
        if (auto target = select->exec_one(ArchItemModel::excludes(f, city))) {
            auto ps = select->exec_multiply(new PersonItemModel(city));
            if (!ps.empty()) {
                for (auto p : ps) {
                    BOOST_ASSERT(p->stayed_city() != target);
                    p->reside(target);

                    qDebug() << QString("%1 move to city %2").arg(to_n(p)).arg(to_n(target));
                }

                app_->reportPerson(ps.front(), QString("%1: 且赴新城，以立奇功!").arg(to_n(ps.front())));

                checker.commit();
            }
        }
    }
    END_IMP()

    //////////////////////////////////////////////////////////////////////
    IMPLEMENT_SPY(onPerson)
    {
        qDebug() << "Display persons";
        select->exec_display(new PersonItemModel(city));
    }
    END_SPY()

    IMPLEMENT_SPY(onCity)
    {
        qDebug() << "Display cities";
        select->exec_display(new ArchItemModel(city));
    }
    END_SPY()

    IMPLEMENT_SPY(onAllPerson)
    {
        qDebug() << "Display fraction persons";

        select->exec_display(new PersonItemModel(city->fraction()));
    }
    END_SPY()

    IMPLEMENT_SPY(onAllCity)
    {
        qDebug() << "Display fraction cities";

        select->exec_display(new ArchItemModel(city->fraction()));
    }
    END_SPY()

    IMPLEMENT_SPY(onFraction)
    {
        qDebug() << "Display fractions";

        select->exec_display(FractionItemModel::all());
    }
    END_SPY()

    IMPLEMENT_SPY(onJob)
    {
        qDebug() << "Display jobs";

        select->exec_display(JobItemModel::inArch(city));
    }
    END_SPY()

    ///////////////////////////////////////////////////////////////////////
    //! 向各城池发布集中命令，集中武将到特定城池
    IMPLEMENT_JOB(onCenteralize) { YRUI_NOT_IMPLEMENTED; }
    END_IMP()

    IMPLEMENT_JOB(onConscript)
    {
        if (auto p = select->exec_one(PersonItemModel::without_troops(city))) {
            qDebug() << yrui::format("Prepare conscript troop for %1%") % p->name();
            city->conscript(p);

            app_->reportEvent("conscript", QString("%1: 愿将腰下剑，直为斩楼兰").arg(to_n(p)));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onTrain)
    {
        auto ps = select->exec_multiply(PersonItemModel::canBeTrained(city));
        if (!ps.empty()) {
            for (auto p : ps) {
                city->train(p);

                qDebug() << yrui::format("%1% prepare to train troop") % p->name();
            }

            app_->reportEvent("train", QString("%1: 百练之兵，其势如奔").arg(to_n(ps.front())));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onDismiss)
    {
        if (auto p = select->exec_one(PersonItemModel::hasTroop(city))) {
            qDebug() << yrui::format("Dismiss troop of %1% in %2%") % p->name() % city->name();

            BOOST_ASSERT(p->has_troop());
            city->dismiss_troop(p);

            app_->reportPerson(p, QString("%1: 精兵简政").arg(to_n(p)));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onReplenish)
    {
        auto ps = select->exec_multiply(PersonItemModel::need_replenish(city));
        if (!ps.empty()) {
            for (auto p : ps) {
                city->replenish(p);

                qDebug() << yrui::format("%1% prepare to train troop") % p->name();
            }

            app_->reportPerson(ps.front(), QString("%1: 百练之兵，其势如奔").arg(to_n(ps.front())));

            checker.commit();
        }
    }
    END_IMP()

    IMPLEMENT_JOB(onWar)
    {
        qDebug() << "Select target";
        auto target = select->exec_one(new ArchItemModel(city, select_neighbor));
        if (!target)
            return;

        qDebug() << "Select troops";
        auto f = city->fraction();
        auto choice = select->exec_multiply(new PersonItemModel(city, not_crashed));
        if (choice.empty())
            return;

        auto consume = std::accumulate(
            choice.begin(), choice.end(), 0, [](int acc, yrui::Person* x) { return acc + x->troop(); });

        if (!city->fraction_can_supply(consume)) {
            app_->reportInfo("粮食供应不足");
        } else if (choice.size() > yrui::military::Legion::troop_num) {
            app_->reportInfo(QString("选择的部队过多，部队数最多为%1").arg(yrui::military::Legion::troop_num));
        } else {
            auto legion = f->intrade(choice, target);

            app_->reportPerson(legion->leader(), QString("必定攻克%1").arg(target->name().c_str()));

            checker.commit();
        }
    }
    END_IMP()

    void GameLogic::onMoveCapital()
    {
        YPRE_CONDITION(app_->playerFraction()->action_power() == app_->controller()->current_ap());

        auto c = currentCity_();
        auto f = app_->playerFraction();

        if (f->arch_count() == 1) {
            qDebug() << "Too less city";
            return;
        }

        auto vec = to_vec(f->archs()
            | yrui::filtered([c](yrui::Architecture* a) { return a != c && a->type() != yrui::arch::pass; }));
        if (vec.size() <= 1) {
            qDebug() << "Too less city";
            return;
        }

        if (auto t = dialog_()->exec_one(ArchItemModel::select(std::move(vec)))) {
            f->set_capital(t);

            app_->controller()->consume_ap(app_->controller()->current_ap());
            app_->screen()->main()->updateInfoBar();

            app_->screen()->main()->updateInfoBar();

            app_->reportPerson(f->leader(), QString("%1: 都城者，一国之本也").arg(to_n(f->leader())));
        }
    }

    ////////////////////////////////////////////////////////////////////////

    IMPLEMENT_SPY(onCheatUnOfficial)
    {
        qDebug() << "Cheat unofficial";

        select->exec_display(PersonItemModel::unofficial(city));
    }
    END_SPY()

    IMPLEMENT_SPY(onCheatAllPerson)
    {
        qDebug() << "Cheat all person";

        select->exec_display(PersonItemModel::all());
    }
    END_SPY()

    IMPLEMENT_SPY(onCheatAllArch)
    {
        qDebug() << "Cheat all arch";

        select->exec_display(ArchItemModel::all());
    }
    END_SPY()

    ////////////////////////////////////////////////////////////////////////
#undef IMPLEMENT_JOB
#undef END_IMP

}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
