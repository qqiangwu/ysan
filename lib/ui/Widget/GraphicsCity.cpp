#include "Widget/GraphicsCity.h"
#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include <QGraphicsColorizeEffect>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>

namespace Bygone {
namespace Frontend {
    GraphicsCity::GraphicsCity(
        yrui::Architecture* arch, const QPixmap& city, const QPixmap& flag, const QColor* color, QGraphicsItem* parent)
        : QGraphicsPixmapItem(city, parent)
        , arch_(arch)
        , raw_(flag)
    {
        using namespace yrui::arch;

        if (color) {
            auto ef = new QGraphicsColorizeEffect;
            ef->setColor(*color);
            this->setGraphicsEffect(ef);
        }

        this->setAcceptedMouseButtons(Qt::LeftButton);
        this->setAcceptHoverEvents(true);
    }

    GraphicsCity::~GraphicsCity() { }

    void GraphicsCity::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) { emit sigDoubleClick(*this); }

    void GraphicsCity::hoverEnterEvent(QGraphicsSceneHoverEvent* event) { emit sigHoverEnter(*this); }

    void GraphicsCity::hoverLeaveEvent(QGraphicsSceneHoverEvent* event) { emit sigHoverLeave(*this); }

    void GraphicsCity::mousePressEvent(QGraphicsSceneMouseEvent* event) { BaseType::mouseReleaseEvent(event); }

    void GraphicsCity::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
    {
        if (event->button() == Qt::LeftButton) {
            emit sigClick(*this);
        }

        BaseType::mouseReleaseEvent(event);
    }
}
}
