#ifndef INTRO_FORM_H
#define INTRO_FORM_H

#include <QDialog>

namespace Ui {
class Intro_form;
}

namespace Bygone {
namespace Frontend {
    class Intro_form : public QDialog {
        Q_OBJECT

    public:
        explicit Intro_form(QWidget* parent = 0);
        ~Intro_form();

    signals:
        void aboutClicked();

    private slots:
        void on_start_clicked();
        void on_save_clicked();
        void on_setting_clicked();
        void on_exit_clicked();
        void on_dev_clicked();

    private:
        Ui::Intro_form* ui;
    };

}
}
#endif // INTRO_FORM_H
