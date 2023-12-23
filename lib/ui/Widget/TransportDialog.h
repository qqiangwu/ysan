#ifndef TRANSPORTDIALOG_H
#define TRANSPORTDIALOG_H

#include <QDialog>

namespace Ui {
class TransportDialog;
}

namespace Bygone {
namespace Frontend {
    class TransportDialog : public QDialog {
        Q_OBJECT

    public:
        explicit TransportDialog(QWidget* parent, int max_money, int max_food);
        ~TransportDialog();

    public:
        int money() const;
        int food() const;

    private:
        Ui::TransportDialog* ui;
    };
}
}

#endif // TRANSPORTDIALOG_H
