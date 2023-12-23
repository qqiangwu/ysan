#ifndef WORKMENU_H
#define WORKMENU_H

#include "kernel/component_fwd.h"
#include <QMenu>

namespace Bygone {
namespace Frontend {
    class MapView;

    class WorkMenu : public QMenu {
        Q_OBJECT

    public:
        WorkMenu(MapView* parent);

    public:
        void onSelf(yrui::Architecture* arch, QPoint pos);
        void onOther(yrui::Architecture* arch, QPoint pos);

    private:
        MapView* view_;
    };
}
}
#endif // WORKMENU_H
