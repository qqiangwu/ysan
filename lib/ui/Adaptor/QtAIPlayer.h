#ifndef QTAIPLAYER_H
#define QTAIPLAYER_H

#include "kernel/fraction/fraction_controller.h"
#include <QObject>

namespace Bygone {
namespace Frontend {
    using yrui::fraction::Fraction;

    class QtAIPlayer : public QObject, public yrui::fraction::AI_controller {
        Q_OBJECT

    public:
        using AI_controller::AI_controller;

    public:
        virtual void prepare(const yrui::Timeline& tl) override;
        virtual void end(const yrui::Timeline& tl) override;

    signals:
        void aiThinking(const QString&);
    };
}
}

#endif // QTAIPLAYER_H
