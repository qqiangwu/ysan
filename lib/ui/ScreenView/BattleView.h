#ifndef BATTLEVIEW_H
#define BATTLEVIEW_H

#include "ScreenView/ByView.h"
#include "kernel/military/legion.h"
#include "kernel/military/strategy.h"
#include <map>

class QLabel;

namespace Bygone {
namespace Frontend {
    class GraphicsTroop;

    class BattleView : public ByView {
        Q_OBJECT

    public:
        explicit BattleView(ByScreen* screen);

    public slots:
        void startWar(yrui::Legion* attacker, yrui::Legion* defensor);
        void endWar(yrui::Legion* winner, yrui::Legion* loser);

        void combat(yrui::Person* attacker, yrui::Person* defensor);
        void defeat(yrui::Person* winner, yrui::Person* loser);

        int askForAction();

        void seigeStart(yrui::Legion* attacker, yrui::Intruded_arch* defensor);
        void seigeEnd(yrui::Legion* attacker, yrui::Intruded_arch* defensor);
        void seigeWait(yrui::Legion* attacker, yrui::Intruded_arch* defensor);
        void seigeSuccess(yrui::Legion* attacker, yrui::Intruded_arch* defensor);
        void seigeFailed(yrui::Legion* attacker, yrui::Intruded_arch* defensor);

        void seigeAttack(yrui::Person* attacker, yrui::Intruded_arch* defensor);
        void seigeAttackSuccess(yrui::Person* attacker, yrui::Intruded_arch* defensor);
        void seigeAttackFailed(yrui::Person* attacker, yrui::Intruded_arch* defensor);

        void becomeStrategyTroop(yrui::Person* p);
        void tryStrategy(yrui::Person* p);
        void strategyAttack(yrui::Person* p, const QString& str);
        void strategyFailed(yrui::Person* x, yrui::Person* y);
        void strategyDefeat(yrui::Person* x, yrui::Person* y);

    public:
        yrui::Fraction* attacker() const { return attacker_; }

        yrui::Fraction* defensor() const { return defensor_; }

    private:
        void prepareSiege(yrui::Legion* attacker, yrui::Intruded_arch* defensor);
        void prepareLegion(yrui::Legion* attacker, yrui::Legion* defensor);
        void setState(const QString& str);

    private:
        QPixmap combat_bk_;
        QPixmap seige_bk_;

    private:
        QLabel* state_ = nullptr;
        QDialog* quest_ = nullptr;

        yrui::Fraction* attacker_;
        yrui::Fraction* defensor_;

        std::map<yrui::Person*, GraphicsTroop*> troops_;
        QGraphicsItem* city_ = nullptr;
    };
}
}

#endif // BATTLEVIEW_H
