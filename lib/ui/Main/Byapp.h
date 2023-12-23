#ifndef BYAPP_H
#define BYAPP_H

#include "kernel/config.h"
#include "Adaptor/QtPlayer.h"
#include "ui/Frontend_config.h"
#include "Main/ImageProvider.h"

#include <QApplication>
#include <QColor>
#include <QDialog>
#include <QWidget>

#include <map>
#include <memory>

class QLabel;

namespace Bygone {
namespace Frontend {
    class GameLogic;
    class ByScreen;

    /*
     * 1. maintain global ui resource
     * 2. maintain game logic resource
     * 3. provide common service such as reporter
     */
    class Byapp : public QApplication {
        Q_OBJECT

    public:
        Byapp(int argc, char* argv[]);

        virtual ~Byapp();

    public:
        QDialog* introWin() { return intro_.get(); }
        QDialog* aboutWin() { return about_.get(); }
        QDialog* selectWin() { return select_.get(); }
        ByScreen* screen() { return screen_.get(); }
        GameLogic* logic() { return logic_.get(); }

    public:
        const QColor& color(yrui::id_t id) { return color_[id]; }

        yrui::id_t player() const { return player_fraction_; }

        yrui::Fraction* playerFraction() const;
        yrui::Fraction_controller* controller() const;

        ImageProvider& provider() { return provider_; }

    public slots:
        void runGame(const std::string& scenario, int fraction);
        void endTurn();
        void saveGame();
        void loadGame();
        void reportInfo(const QString& info);
        void reportAll(const QString& title, const QString& info);

        void reportEvent(const QString& event, const QString& info);
        void reportPerson(yrui::Person* p, const QString& info);

    private:
        void register_signals_();

    private:
        std::map<yrui::id_t, QColor> color_;

    private:
        QtPlayer* player_ = nullptr;
        yrui::id_t player_fraction_ = yrui::null_id;
        mutable yrui::Fraction* fraction_;

    private:
        std::unique_ptr<QDialog> intro_;
        std::unique_ptr<QDialog> about_;
        std::unique_ptr<QDialog> select_;
        std::unique_ptr<ByScreen> screen_;
        std::unique_ptr<GameLogic> logic_;

    private:
        ImageProvider provider_;
    };
}
}

#endif // BYAPP_H
