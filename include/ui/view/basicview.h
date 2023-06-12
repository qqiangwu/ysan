#ifndef BYGONE_UI_BASIC_VIEW_H_
#define BYGONE_UI_BASIC_VIEW_H_

#include <QtWidgets/QGraphicsView>

namespace Bygone {
class Bygame;

class BasicView : public QGraphicsView {
public:
    BasicView(Bygmae& game, QWidget* parent);

private:
    Bygame& game_;
};
} // of namespace Bygone

#endif //! BYGONE_UI_BASIC_VIEW_H_
