#include "Widget/WorkMenu.h"
#include "Main/Byapp.h"
#include "Main/GameLogic.h"
#include "ScreenView/Mapview.h"

#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"

namespace Bygone {
namespace Frontend {
    WorkMenu::WorkMenu(MapView* parent)
        : QMenu(parent)
        , view_(parent)
    {
    }

    void WorkMenu::onSelf(yrui::Architecture* arch, QPoint pos)
    {
        this->clear();

        auto app = view_->app();
        auto logic = app->logic();

        QMenu* affair = this->addMenu("内政");
        QMenu* personel = this->addMenu("人事");
        QMenu* military = this->addMenu("军事");
        QMenu* info = this->addMenu("情报");
        QMenu* cheat = this->addMenu("作弊");

        if (!arch->has_done_farm()) {
            affair->addAction("农业", logic, SLOT(onFarm()));
        }
        if (!arch->has_done_trade()) {
            affair->addAction("商业", logic, SLOT(onTrade()));
        }
        if (!arch->has_done_security()) {
            affair->addAction("治安", logic, SLOT(onSecurity()));
        }
        if (!arch->has_done_steady()) {
            affair->addAction("城防", logic, SLOT(onSteady()));
        }

        affair->addAction("运输", logic, SLOT(onTransport()));

        personel->addAction("搜索", logic, SLOT(onSearch()));
        personel->addAction("太守", logic, SLOT(onMayor()));
        personel->addAction("移动", logic, SLOT(onMove()));
        personel->addAction("任命", logic, SLOT(onPromote()));

        military->addAction("训练", logic, SLOT(onTrain()));
        military->addAction("征兵", logic, SLOT(onConscript()));
        military->addAction("出兵", logic, SLOT(onWar()));
        military->addAction("补给", logic, SLOT(onReplenish()));
        military->addAction("解散", logic, SLOT(onDismiss()));
        // military->addAction("征召", logic, SLOT(onCenteralize()));

        info->addAction("工作", logic, SLOT(onJob()));
        info->addAction("城池", logic, SLOT(onCity()));
        info->addAction("武将", logic, SLOT(onPerson()));
        info->addAction("势力城池", logic, SLOT(onAllCity()));
        info->addAction("势力武将", logic, SLOT(onAllPerson()));
        info->addAction("全势力", logic, SLOT(onFraction()));

        cheat->addAction("工作", logic, SLOT(onJob()));
        cheat->addAction("城池", logic, SLOT(onCity()));
        cheat->addAction("武将", logic, SLOT(onPerson()));
        cheat->addAction("在野", logic, SLOT(onCheatUnOfficial()));
        cheat->addAction("全城池", logic, SLOT(onCheatAllArch()));
        cheat->addAction("全武将", logic, SLOT(onCheatAllPerson()));

        if (app->controller()->current_ap() == app->playerFraction()->action_power()) {
            this->addAction("迁都", logic, SLOT(onMoveCapital()));
        }

        this->addAction("结束", app, SLOT(endTurn()));

        this->exec(pos);
    }

    void WorkMenu::onOther(yrui::Architecture* arch, QPoint pos)
    {
        this->clear();

        auto app = view_->app();
        auto logic = app->logic();

        QMenu* cheat = this->addMenu("作弊");

        cheat->addAction("工作", logic, SLOT(onJob()));
        cheat->addAction("城池", logic, SLOT(onCity()));
        cheat->addAction("武将", logic, SLOT(onPerson()));
        cheat->addAction("在野", logic, SLOT(onCheatUnOfficial()));
        cheat->addAction("全城池", logic, SLOT(onCheatAllArch()));
        cheat->addAction("全武将", logic, SLOT(onCheatAllPerson()));

        this->addAction("结束", app, SLOT(endTurn()));

        this->exec(pos);
    }
}
}
