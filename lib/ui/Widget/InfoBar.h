#ifndef INFOBAR_H
#define INFOBAR_H

#include "kernel/component_fwd.h"
#include <QWidget>

namespace Ui {
class InfoBar;
}

namespace Bygone {
namespace Frontend {
    class MapView;

    class InfoBar : public QWidget {
        Q_OBJECT

    public:
        explicit InfoBar(MapView* parent, yrui::Fraction* f);
        ~InfoBar();

    public:
        void updateBar();

    private:
        Ui::InfoBar* ui;
        MapView* view_;
    };
}
}

#endif // INFOBAR_H
