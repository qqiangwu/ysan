#ifndef ABOUT_FORM_H
#define ABOUT_FORM_H

#include <QDialog>

namespace Ui {
class About_form;
}

namespace Bygone {
namespace Frontend {
    class About_form : public QDialog {
        Q_OBJECT

    public:
        explicit About_form(QWidget* parent = 0);
        ~About_form();

    private slots:
        void on_Confirm_clicked();

    private:
        Ui::About_form* ui;
    };
}
}
#endif // ABOUT_FORM_H
