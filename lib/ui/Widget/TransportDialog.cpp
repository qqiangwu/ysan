#include "Widget/TransportDialog.h"
#include "ui_TransportDialog_imp.h"

#include "kernel/config.h"

#include <QIntValidator>

namespace Bygone {
namespace Frontend {
    TransportDialog::TransportDialog(QWidget* parent, int max_money, int max_food)
        : QDialog(parent)
        , ui(new Ui::TransportDialog)
    {
        BOOST_ASSERT(max_money > 0);
        BOOST_ASSERT(max_food > 0);

        ui->setupUi(this);

        ui->max_food->setText(QString::number(max_food));
        ui->max_money->setText(QString::number(max_money));
        ui->money->setValidator(new QIntValidator(0, max_money, this));
        ui->food->setValidator(new QIntValidator(0, max_food, this));
    }

    TransportDialog::~TransportDialog() { delete ui; }

    int TransportDialog::money() const { return ui->money->text().toInt(); }

    int TransportDialog::food() const { return ui->food->text().toInt(); }
}
}
