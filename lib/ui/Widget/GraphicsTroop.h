#ifndef GRAPHICSTROOP_H
#define GRAPHICSTROOP_H

#include "kernel/person/person.h"
#include <QGraphicsItem>

namespace Bygone {
namespace Frontend {
    class GraphicsTroop : public QGraphicsItem {
    public:
        GraphicsTroop(yrui::Person* troop);

    public:
        bool isAlive() const { return !troop_->is_troop_crashed(); }

    public:
        virtual QRectF boundingRect() const override;

    public:
        void becomeStrategyTroop() { is_strategy_ = true; }

    protected:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        yrui::Person* troop_;
        bool is_strategy_ = false;
    };
}
}

#endif // GRAPHICSTROOP_H
