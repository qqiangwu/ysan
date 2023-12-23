#ifndef REPORTER_H
#define REPORTER_H

#include <QDialog>

namespace Ui {
class Reporter;
}

namespace Bygone {
namespace Frontend {
    class Reporter : public QDialog {
        Q_OBJECT

    public:
        explicit Reporter(QWidget* parent = 0);
        virtual ~Reporter();

    public:
        void report(const QString& info);
        void report(const QString& title, const QString& info);
        void report(const QPixmap& img, const QString& info);

    private:
        Ui::Reporter* ui;
    };
}
}
#endif // REPORTER_H
