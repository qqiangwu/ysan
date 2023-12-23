#include <cassert>

#include "Intro_form.h"
#include "ui_Intro_form_imp.h"

namespace Bygone {
namespace Frontend {
    Intro_form::Intro_form(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::Intro_form)
    {
        ui->setupUi(this);

        //! setbackground
        QPixmap bk("Resource/Image/Intro/index.jpg");
        auto pal = this->palette();
        pal.setBrush(this->backgroundRole(), QBrush(bk.scaled(this->size())));

        this->setPalette(pal);
        this->setAutoFillBackground(true);

        QString style("\
                QPushButton#%1{\
                    color: white;\
                    border-image: url(Resource/Image/Intro/%1.png)\
                }\
                QPushButton#%1:hover{\
                    border-image: url(Resource/Image/Intro/%1Select.png)\
                }\
                QPushButton#%1:pressed {\
                    border-image: url(Resource/Image/Intro/%1Press.png)\
                }");

        ui->start->setStyleSheet(style.arg("start"));
        ui->save->setStyleSheet(style.arg("save"));
        ui->setting->setStyleSheet(style.arg("setting"));
        ui->dev->setStyleSheet(style.arg("dev"));
        ui->exit->setStyleSheet(style.arg("exit"));
    }

    Intro_form::~Intro_form() { delete ui; }
}
}

void Bygone::Frontend::Intro_form::on_start_clicked() { this->accept(); }

void Bygone::Frontend::Intro_form::on_save_clicked() { }

void Bygone::Frontend::Intro_form::on_setting_clicked() { }

void Bygone::Frontend::Intro_form::on_exit_clicked() { this->reject(); }

void Bygone::Frontend::Intro_form::on_dev_clicked() { emit aboutClicked(); }

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
