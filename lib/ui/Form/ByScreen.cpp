#include "Form/ByScreen.h"
#include "Main/Byapp.h"
#include "Widget/Reporter.h"
#include "Widget/SysMenu.h"
#include "Widget/YesOrNo.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>

#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    ByScreen::ByScreen(Byapp* app)
        : QMainWindow {}
        , app_ { app }
    {
        reporter_ = new Reporter(this);
        currentFraction_ = new QLabel(this);
        sysMenu_ = new SysMenu(this);

        currentFraction_->setAutoFillBackground(true);
        currentFraction_->setFixedSize({ 250, 100 });

        auto center = this->geometry().center();
        currentFraction_->move(this->mapFromGlobal(center));
        currentFraction_->setAlignment(Qt::AlignCenter);
    }

    ByScreen::~ByScreen() { }

    void ByScreen::onAIThinking(const QString& leader)
    {
        BOOST_ASSERT(currentFraction_);

        main_->setEnabled(false);
        currentFraction_->show();
        currentFraction_->raise();

        currentFraction_->setText(QString("%1 思考中").arg(leader));
    }

    void ByScreen::onPlayerThinking()
    {
        BOOST_ASSERT(currentFraction_);

        currentFraction_->hide();
        main_->setEnabled(true);
        main_->updateInfoBar();
    }

    void ByScreen::reportInfo(const QString& info) { reporter_->report(info); }

    void ByScreen::reportAll(const QString& title, const QString& info) { reporter_->report(title, info); }

    void ByScreen::onStart()
    {
        BOOST_ASSERT(main_ == nullptr);

        main_ = new MapView(this);
        battle_ = new BattleView(this);

        battle_->hide();

        main_->centeralizeCapital(app_->player());
        current_ = main_;

        this->setCentralWidget(this->current());
        this->show();
    }

    void ByScreen::showSysMenu(QPoint pos)
    {
        BOOST_ASSERT(sysMenu_ != nullptr);
        sysMenu_->exec(pos);
    }

    void ByScreen::changeView(ByView* view)
    {
        BOOST_ASSERT(current_);
        BOOST_ASSERT(this->centralWidget() == current_);
        BOOST_ASSERT(!current_->isHidden());

        currentFraction_->hide();

        if (current_ != view) {
            current_->hide();

            this->takeCentralWidget();
            this->setCentralWidget(view);

            current_ = view;
            current_->show();
        }

        BOOST_ASSERT(current_);
        BOOST_ASSERT(this->centralWidget() == current_);
        BOOST_ASSERT(!current_->isHidden());
    }

    bool ByScreen::askForChoice(const QString& question)
    {
        if (!yesOrNo_) {
            yesOrNo_ = new YesOrNo(this);
        }

        return yesOrNo_->ask(question);
    }

    void ByScreen::reportEvent(const QString& event, const QString& info)
    {
        reporter_->report(app_->provider().event(event), info);
    }

    void ByScreen::reportPerson(yrui::Person* p, const QString& info)
    {
        auto key = int(p->id());
        reporter_->report(app_->provider().person(key), info);
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
