#include "Widget/SelectDialog.h"
#include "ui_SelectDialog_imp.h"
#include <QDialogButtonBox>
#include <QPushButton>

namespace Bygone {
namespace Frontend {
    SelectDialog::SelectDialog(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::SelectDialog)
    {
        ui->setupUi(this);
        table_ = ui->tableView;
    }

    SelectDialog::~SelectDialog() { delete ui; }

    void SelectDialog::on_tableView_activated(const QModelIndex& index)
    {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }

    void SelectDialog::prepare_() { ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false); }
}
}
