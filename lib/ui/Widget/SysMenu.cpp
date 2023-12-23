#include "Widget/SysMenu.h"
#include "Form/ByScreen.h"
#include "Main/Byapp.h"

namespace Bygone {
namespace Frontend {
    SysMenu::SysMenu(ByScreen* parent)
        : QMenu(parent)
        , screen_(parent)
    {
        auto app = screen_->app();

        this->addAction("存储", app, SLOT(saveGame()));
        this->addAction("载入", app, SLOT(loadGame()));
        this->addAction("退出", app, SLOT(quit()));
    }
}
}
