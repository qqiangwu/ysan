#ifndef QTPLAYER_H
#define QTPLAYER_H

#include "kernel/fraction/fraction_controller.h"
#include <QObject>
#include <condition_variable>
#include <mutex>

namespace Bygone {
namespace Frontend {
    using yrui::fraction::Fraction;

    class QtPlayer : public QObject, public yrui::fraction::Player_controller {
        Q_OBJECT

    public:
        using Player_controller::Player_controller;

    protected:
        virtual void prepare(const yrui::Timeline& tl) override;
        virtual void emit_decision(const yrui::Timeline& tl) override;

    public:
        yrui::uptr<yrui::Legion> defend(yrui::Legion* attacker) override;

        void command_troop(yrui::Legion*, yrui::Legion*) override;
        void command_troop(yrui::Legion*, yrui::Intruded_arch*) override;
        bool permit_strategy(yrui::Person* p) override;

    public:
        void notify_self();

    signals:
        void playerThinking();
        int actionRequired();
        bool yesOrNo(const QString& question);

    private:
        std::condition_variable decision_ends_;
        std::mutex mu_;
    };
}
}

#endif // QTPLAYER_H
