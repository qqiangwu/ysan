#include <QtWidgets/QAbstractButton>

#include "ui/form/aboutform.h"
#include "ui/support/utility.h"
#include "ui_aboutformimp.h"

Bygone::AboutForm::AboutForm(const yrui::Config_dict& conf, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::AboutForm)
{
    ui->setupUi(this);

    connect(ui->confirm, &QPushButton::clicked, this, &confirmClicked);

    // setup ui
    ui->confirm->setText(conf.get("confirm", "confirm").c_str());
    ui->aboutMe->setText(conf.get("content", "bad content").c_str());

    if (auto bk = conf.get_optional<std::string>("background")) {
        setBackground(this, bk.get());
    }
}

Bygone::AboutForm::~AboutForm() = default;

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
