#ifndef SYSMENU_H
#define SYSMENU_H

#include <QMenu>

namespace Bygone {
namespace Frontend {
    class ByScreen;

    class SysMenu : public QMenu {
        Q_OBJECT

    public:
        explicit SysMenu(ByScreen* screen);

    private:
        ByScreen* screen_;
    };
}
}

#endif // SYSMENU_H
