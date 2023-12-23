#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "ScreenView/ByView.h"
#include "Widget/CityInfo.h"
#include <QPixmap>
#include <map>

class QMouseEvent;
class QWheelEvent;

namespace Bygone {
namespace Frontend {
    class GraphicsCity;
    class WorkMenu;
    class SelectDialog;
    class InfoBar;

    class MapView : public ByView {
        Q_OBJECT

    public:
        explicit MapView(ByScreen* p);

    private:
        virtual void mouseMoveEvent(QMouseEvent* event) override;
        virtual void mousePressEvent(QMouseEvent* event) override;
        virtual void mouseReleaseEvent(QMouseEvent* event) override;
        virtual void wheelEvent(QWheelEvent* event) override;

    private:
        virtual void drawBackground(QPainter* painter, const QRectF& rect) override;

    public:
        GraphicsCity* currentCity() { return current_; }
        CityInfo* cityInfo() { return cityInfo_; }
        SelectDialog* selectDialog() { return select_; }

    public slots:
        void centeralize(GraphicsCity& item);
        void onHoverEnter(GraphicsCity& city);
        void onHoverLeave(GraphicsCity& city);
        void onClick(GraphicsCity& city);
        void changeCityColor(yrui::Architecture* arch);

    public:
        void zoom(int value);
        void centerialize();
        void centeralizeCapital(yrui::id_t fraction);
        void updateInfoBar();

    private:
        GraphicsCity* current_ = nullptr;
        CityInfo* cityInfo_ = nullptr;
        WorkMenu* workMenu_ = nullptr;
        SelectDialog* select_ = nullptr;
        InfoBar* info_ = nullptr;

    private:
        std::map<yrui::id_t, GraphicsCity*> cities_;

    private:
        QPixmap map_layer_;

        int center_x_ = 0;
        int center_y_ = 0;

        QPointF prev_;

        int zoom_;
        int min_zoom_;
        int max_zoom_;
    };
}
}

#endif // MAPVIEW_H
