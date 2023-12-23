#ifndef GRAPHICSWARCITY_H
#define GRAPHICSWARCITY_H

#include "kernel/arch/architecture.h"
#include <QGraphicsItem>

namespace Bygone {
namespace Frontend {
    class GraphicsWarCity : public QGraphicsItem {
    public:
        enum { city_width = 400 };
        enum { city_height = 100 };

    public:
        explicit GraphicsWarCity(yrui::Intruded_arch* arch, bool draw_food = false);

    public:
        virtual QRectF boundingRect() const override;

    protected:
        virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    private:
        yrui::Intruded_arch* arch_;
        bool draw_food_ = false;
    };
}
}

#endif // GRAPHICSWARCITY_H
