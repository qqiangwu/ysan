#ifndef BYGONE_UI_VIEW_MAP_VIEW_H_
#define BYGONE_UI_VIEW_MAP_VIEW_H_

#include "foundation/config_dict.h"
#include "ui/view/basicview.h"

namespace Bygone {
class MapView : public BasicView {
public:
    MapView(Bygame& game, yrui::Config_dict& conf, QWidget* parent);
};
} // of namespace Bygone

#endif //! BYGONE_UI_VIEW_MAP_VIEW_H_
