#include "kernel/arch/architecture.h"
#include "kernel/military/intruded_arch.h"
#include "kernel/military/legion.h"
#include "kernel/person/person.h"

#include <QDialog>
#include <QGraphicsScene>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSignalMapper>

#include "Detail/Shim.h"
#include "Form/ByScreen.h"
#include "Main/Byapp.h"
#include "ScreenView/BattleView.h"
#include "Widget/GraphicsTroop.h"
#include "Widget/GraphicsWarCity.h"

namespace Bygone {
namespace Frontend {
    using namespace yrui;

    BattleView::BattleView(ByScreen* screen)
        : ByView(screen)
    {
        auto scene = new QGraphicsScene(this);

        scene->setSceneRect(-screen->width() / 2, -screen->height() / 2, screen->width(), screen->height());

        this->setScene(scene);

        this->setRenderHint(QPainter::Antialiasing);
        this->setCacheMode(QGraphicsView::CacheBackground);
        this->setAutoFillBackground(true);

        state_ = new QLabel(this);
        state_->resize(200, 80);
        auto font = state_->font();
        font.setBold(true);
        font.setWeight(60);
        state_->setFont(font);
        state_->move(screen->geometry().center().x(), screen->height() - 80);
    }

    void BattleView::prepareLegion(Legion* attacker, Legion* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        constexpr auto interval = 5;
        constexpr auto padding = 10;

        BOOST_ASSERT(troops_.empty());
        BOOST_ASSERT(this->scene()->items().empty());

        /*
        if (combat_bk_.isNull()) {
            combat_bk_ =
                    QPixmap("Resource/Image/Map/combat.jpg")
                         .scaled(this->size());
        }

        //! update background
        //! \fixme  Bug in background
        auto p = this->palette();
        p.setBrush(this->backgroundRole(), QBrush(combat_bk_));
        this->setPalette(p);
        */

        //! create attacker troops
        const auto att_x = -this->width() / 2 + padding;
        auto att_y = -this->height() / 2 + padding;
        for (auto troop : attacker->all()) {
            auto t = new GraphicsTroop(troop);
            auto rct = t->boundingRect();

            t->setPos(att_x + rct.width() / 2, att_y + rct.height() / 2);
            troops_.insert({ troop, t });

            att_y += rct.height() + interval;
        }

        //! create defensor troops
        const auto def_x = 0 + padding;
        auto def_y = -this->height() / 2 + padding;
        for (auto troop : defensor->all()) {
            auto t = new GraphicsTroop(troop);
            auto rct = t->boundingRect();

            t->setPos(def_x + rct.width() / 2, def_y + rct.height() / 2);
            troops_.insert({ troop, t });

            def_y += rct.height() + interval;
        }

        //! add to scene
        for (auto item : troops_) {
            this->scene()->addItem(item.second);
        }
    }

    void BattleView::prepareSiege(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        constexpr auto interval = 5;
        constexpr auto padding = 10;

        BOOST_ASSERT(this->scene()->items().empty());
        BOOST_ASSERT(troops_.empty());
        BOOST_ASSERT(city_ == nullptr);

        /*
        if (!seige_bk_.isNull()) {
            seige_bk_ =
                    QPixmap("Resource/Image/Map/seige.jpg")
                        .scaled(this->size());
        }

        //! update background
        auto p = this->palette();
        p.setBrush(this->backgroundRole(), QBrush(seige_bk_));
        this->setPalette(p);
        */

        //! create attacker troops
        const auto att_x = -this->width() / 2 + padding;
        auto att_y = -this->height() / 2 + padding;
        for (auto troop : attacker->all()) {
            auto t = new GraphicsTroop(troop);
            auto rct = t->boundingRect();

            t->setPos(att_x + rct.width() / 2, att_y + rct.height() / 2);
            troops_.insert({ troop, t });

            att_y += rct.height() + interval;
        }

        city_ = new GraphicsWarCity(defensor, defensor->fraction() == this->app()->playerFraction());
        city_->setPos(GraphicsWarCity::city_width / 2, 0);
        this->scene()->addItem(city_);

        //! add to scene
        for (auto item : troops_) {
            this->scene()->addItem(item.second);
        }
    }

    void BattleView::startWar(Legion* attacker, Legion* defensor)
    {
        BOOST_ASSERT(!this->attacker());
        BOOST_ASSERT(!this->defensor());
        BOOST_ASSERT(this->screen()->current() != this);
        BOOST_ASSERT(attacker->fraction() == this->app()->playerFraction()
            || defensor->fraction() == this->app()->playerFraction());

        attacker_ = attacker->fraction();
        defensor_ = defensor->fraction();

        this->screen()->changeView(this);
        this->prepareLegion(attacker, defensor);

        this->setState(QString("%1 军 对 %2 军: 战斗开始").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::endWar(Legion* winner, Legion* loser)
    {
        BOOST_ASSERT(this->screen()->current() == this);
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        if (this->app()->playerFraction() == winner->fraction()) {
            this->app()->reportAll("胜利", QString("%1 赢得了战争").arg(to_name(winner)));
        } else {
            this->app()->reportAll("失败", QString("%1 战败了").arg(to_name(loser)));
        }

        this->screen()->changeView(this->screen()->main());
        this->scene()->clear();
        troops_.clear();

        attacker_ = nullptr;
        defensor_ = nullptr;
    }

    void BattleView::combat(Person* attacker, Person* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 与 %2 发生了战斗").arg(to_name(attacker)).arg(to_name(defensor)));

        troops_[attacker]->update();
        troops_[defensor]->update();
    }

    void BattleView::defeat(Person* winner, Person* loser)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        BOOST_ASSERT(!winner->is_troop_crashed());
        BOOST_ASSERT(loser->is_troop_crashed());
        this->scene()->removeItem(troops_[loser]);
        this->setState(QString("%1 败退了").arg(to_name(loser)));
    }

    void BattleView::setState(const QString& str)
    {
        BOOST_ASSERT(state_);

        state_->setText(str);
    }

    int BattleView::askForAction()
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        if (!quest_) {
            quest_ = new QDialog(this);
            quest_->setLayout(new QHBoxLayout);

            auto attack = new QPushButton("攻击", quest_);
            auto defend = new QPushButton("防卫", quest_);
            auto retreat = new QPushButton("退却", quest_);

            quest_->layout()->addWidget(attack);
            quest_->layout()->addWidget(defend);
            quest_->layout()->addWidget(retreat);

            auto mapper = new QSignalMapper(quest_);
            mapper->setMapping(attack, 0);
            mapper->setMapping(defend, 1);
            mapper->setMapping(retreat, 2);

            connect(attack, SIGNAL(clicked()), mapper, SLOT(map()));
            connect(defend, SIGNAL(clicked()), mapper, SLOT(map()));
            connect(retreat, SIGNAL(clicked()), mapper, SLOT(map()));

            connect(mapper, SIGNAL(mapped(int)), quest_, SLOT(done(int)));
        }

        BOOST_ASSERT(quest_);

        this->setState("请选择要做什么");

        return quest_->exec();
    }

    void BattleView::seigeStart(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(!this->attacker());
        BOOST_ASSERT(!this->defensor());

        auto self = this->app()->playerFraction();
        BOOST_ASSERT(attacker->fraction() == self || defensor->fraction() == self);

        attacker_ = attacker->fraction();
        defensor_ = defensor->fraction();
        this->prepareSiege(attacker, defensor);
        this->screen()->changeView(this);

        this->setState(QString("%1 军 攻打城池 %2").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeEnd(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->screen()->changeView(this->screen()->main());
        this->scene()->clear();
        troops_.clear();
        city_ = nullptr;

        attacker_ = nullptr;
        defensor_ = nullptr;
    }

    void BattleView::seigeWait(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->update();
        this->setState(QString("%1 军 包围城池 %2").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeSuccess(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 军 攻陷城池 %2").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeFailed(Legion* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 军 攻打城池 %2 失败").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeAttack(Person* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        troops_[attacker]->update();
        city_->update();
        this->setState(QString("%1 向城池 %2 发起攻击").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeAttackSuccess(Person* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 攻陷城池 %2").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::seigeAttackFailed(Person* attacker, Intruded_arch* defensor)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        auto item = troops_[attacker];
        troops_.erase(attacker);
        this->scene()->removeItem(item);
        this->setState(QString("%1 从城池 %2 下败退了").arg(to_name(attacker)).arg(to_name(defensor)));
    }

    void BattleView::strategyAttack(Person* p, const QString& s)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 发动了 %2").arg(to_name(p)).arg(s));
    }

    void BattleView::strategyFailed(Person* x, Person* y)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 的计谋被 %2 识破了").arg(to_name(x)).arg(to_name(y)));
    }

    void BattleView::strategyDefeat(Person* x, Person* loser)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->scene()->removeItem(troops_[loser]);
        this->setState(QString("%1 击败了 %2").arg(to_name(x)).arg(to_name(loser)));
    }

    void BattleView::tryStrategy(yrui::Person* p)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        this->setState(QString("%1 思考中").arg(to_name(p)));
    }

    void BattleView::becomeStrategyTroop(yrui::Person* p)
    {
        BOOST_ASSERT(this->attacker());
        BOOST_ASSERT(this->defensor());

        troops_[p]->becomeStrategyTroop();
        this->setState(QString("%1 成为计谋部队").arg(to_name(p)));
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
