#include "ScreenView/ByView.h"
#include "Form/ByScreen.h"

namespace Bygone {
namespace Frontend {
    ByView::ByView(ByScreen* parent)
        : QGraphicsView(parent)
        , app_(parent->app())
        , screen_(parent)
    {
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
