#ifndef BYSCREEN_H
#define BYSCREEN_H

#include <QMainWindow>
#include <memory>

#include "ScreenView/BattleView.h"
#include "ScreenView/Mapview.h"

class QLabel;
class QWidget;
class QDialog;

namespace Bygone {
namespace Frontend {
    class Byapp;
    class Reporter;
    class YesOrNo;
    class SysMenu;

    class ByScreen : public QMainWindow {
        Q_OBJECT

    public:
        explicit ByScreen(Byapp* app);
        virtual ~ByScreen();

    public:
        Byapp* app() { return app_; }

        MapView* main() { return main_; }
        BattleView* battleView() { return battle_; }

        ByView* current() { return current_; }
        void changeView(ByView* view);

    public slots:
        void onAIThinking(const QString& leader);
        void onPlayerThinking();

        void reportInfo(const QString& info);
        void reportAll(const QString& title, const QString& info);

        void onStart();
        void showSysMenu(QPoint pos);
        bool askForChoice(const QString& question);

        void reportEvent(const QString& event, const QString& info);
        void reportPerson(yrui::Person* p, const QString& info);

    private:
        Byapp* app_;

        //! \name   views
        //! @{
    private:
        MapView* main_ = nullptr;
        BattleView* battle_ = nullptr;
        ByView* current_ = nullptr;
        //! @}

        //! \name   widget
        //! @{
    private:
        QLabel* currentFraction_ = nullptr;
        Reporter* reporter_ = nullptr;
        SysMenu* sysMenu_ = nullptr;
        YesOrNo* yesOrNo_ = nullptr;
        //! @}
    };
}
}

#endif // BYSCREEN_H
