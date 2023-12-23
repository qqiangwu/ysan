#include <QPainter>

#include "kernel/person/person.h"

#include "Widget/GraphicsTroop.h"

namespace Bygone {
namespace Frontend {
    GraphicsTroop::GraphicsTroop(yrui::Person* troop)
        : troop_(troop)
    {
        BOOST_ASSERT(!troop->is_troop_crashed());
    }

    void GraphicsTroop::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
    {
        auto p = troop_;

        const auto rect = this->boundingRect();
        const auto w = rect.width();
        const auto h = rect.height();
        const auto vh = h / 2 - h / 10;

        painter->drawText({ -w / 2, -vh, w / 2, vh }, p->name().c_str());
        painter->drawRect(-w / 2, 0, w, vh);
        painter->fillRect(-w / 2, 0, w * p->troop() / p->max_troop(), vh, is_strategy_ ? Qt::gray : Qt::red);
    }

    QRectF GraphicsTroop::boundingRect() const { return { -80, -20, 160, 40 }; }
}
}
