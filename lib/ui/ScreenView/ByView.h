#ifndef BYVIEW_H
#define BYVIEW_H

#include <QGraphicsView>

namespace Bygone {
namespace Frontend {
    class Byapp;
    class ByScreen;

    class ByView : public QGraphicsView {
        Q_OBJECT

    public:
        explicit ByView(ByScreen* parent);

    public:
        Byapp* app() { return app_; }
        ByScreen* screen() { return screen_; }

    private:
        Byapp* app_;
        ByScreen* screen_;
    };
}
}

#endif // BYVIEW_H
