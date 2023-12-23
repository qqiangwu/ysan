#ifndef YESORNO_H
#define YESORNO_H

#include <QDialog>

namespace Ui {
class YesOrNo;
}

namespace Bygone {
namespace Frontend {
    class YesOrNo : public QDialog {
        Q_OBJECT

    public:
        explicit YesOrNo(QWidget* parent = 0);
        ~YesOrNo();

    public:
        bool ask(const QString& str);

    private:
        Ui::YesOrNo* ui;
    };
}
}

#endif // YESORNO_H
