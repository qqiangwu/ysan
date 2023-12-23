#include "Widget/GraphicsWarCity.h"
#include <QPainter>

#include "kernel/military/intruded_arch.h"

namespace Bygone {
namespace Frontend {
    GraphicsWarCity::GraphicsWarCity(yrui::Intruded_arch* arch, bool draw_food)
        : arch_(arch)
        , draw_food_(draw_food)
    {
    }

    QRectF GraphicsWarCity::boundingRect() const
    {
        return { -city_width / 2, -city_height / 2, city_width, city_height };
    }

    void GraphicsWarCity::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
    {
        auto a = arch_->arch();

        auto text_h = city_height / 5;
        auto label_w = city_width / 5;
        auto slot_w = city_width / 3;
        auto box = this->boundingRect();

        painter->fillRect(box, Qt::gray);

        box.setLeft(box.x() + 10);
        box.setTop(box.y() + 10);

        painter->drawText({ box.x(), box.y(), box.width(), static_cast<qreal>(text_h) }, arch_->name().c_str());

        painter->drawText({ box.x(), box.y() + text_h, static_cast<qreal>(label_w), static_cast<qreal>(text_h) }, "城防");

        painter->drawRect(box.x() + label_w, box.y() + text_h, slot_w, text_h);
        //! steady
        painter->fillRect(box.x() + label_w, box.y() + text_h, slot_w * a->steady() / a->max_steady(), text_h, Qt::red);

        if (draw_food_) {
            painter->drawText({ box.x(), box.y() + 2 * text_h, static_cast<qreal>(label_w), static_cast<qreal>(text_h) }, "粮草");
            painter->drawRect(box.x() + label_w, box.y() + 2 * text_h, slot_w, text_h);
            painter->fillRect(box.x() + label_w, box.y() + 2 * text_h, std::min(slot_w * a->food() / 100000, slot_w),
                text_h, Qt::red);
        }
    }
}
}
