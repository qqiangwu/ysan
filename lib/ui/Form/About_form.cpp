#include "About_form.h"
#include "ui_About_form_imp.h"

namespace Bygone {
namespace Frontend {

    About_form::About_form(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::About_form)
    {
        ui->setupUi(this);
    }

    About_form::~About_form() { delete ui; }

    void About_form::on_Confirm_clicked() { this->hide(); }

}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
