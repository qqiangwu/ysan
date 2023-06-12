#ifndef BYGONE_FRONTEND_BYSCREEN_H_
#define BYGONE_FRONTEND_BYSCREEN_H_

#include <Qtwidgets/QStackedWidget>

namespace Bygone {
class Bygame;

class ByScreen : public QStackedWidget {
    Q_OBJECT

public:
    ByScreen(Bygame& game, QWidget* parent);
};
} // of namespace Bygone

#endif //! BYGONE_FRONTEND_BYSCREEN_H_
