#ifndef CITYINFO_H
#define CITYINFO_H

#include "kernel/component_fwd.h"
#include <QWidget>

namespace Ui {
class CityInfo;
}

namespace Bygone {
namespace Frontend {
    using namespace yrui::arch;

    class CityInfo : public QWidget {
        Q_OBJECT

    public:
        explicit CityInfo(QWidget* parent = 0);
        ~CityInfo();

        void showArch(yrui::Architecture* arch, bool detail = false);

    private:
        Ui::CityInfo* ui;
    };
}
}

#endif // CITYINFO_H
