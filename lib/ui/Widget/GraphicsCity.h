#ifndef GRAPHICSCITY_H
#define GRAPHICSCITY_H

#include "kernel/component_fwd.h"
#include <QGraphicsPixmapItem>

namespace Bygone {
namespace Frontend {
    class GraphicsCity : public QObject, public QGraphicsPixmapItem {
    public:
        Q_OBJECT

    public:
        typedef QGraphicsPixmapItem BaseType;

    public:
        GraphicsCity(yrui::Architecture* arch, const QPixmap& city, const QPixmap& flag, const QColor* color = nullptr,
            QGraphicsItem* parent = nullptr);

        virtual ~GraphicsCity();

    public:
        yrui::Architecture* arch() { return arch_; }

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

    signals:
        void sigClick(GraphicsCity& city);
        void sigRighClick(GraphicsCity& city);
        void sigDoubleClick(GraphicsCity& city);
        void sigHoverEnter(GraphicsCity& city);
        void sigHoverLeave(GraphicsCity& city);

    private:
        yrui::Architecture* arch_;
        QPixmap raw_;
        QPixmap flag_;
    };
}
}

#endif // GRAPHICSCITY_H
