#include "ui/form/splashform.h"
#include "ui/support/utility.h"
#include "ui_splashform.h"

Bygone::SplashForm::SplashForm(const yrui::Config_dict& conf, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SplashForm)
{
    ui->setupUi(this);

    if (auto bk = conf.get_optional<std::string>("background")) {
        setBackground(this, bk.get());
    }
}

Bygone::SplashForm::~SplashForm() = default;

void Bygone::SplashForm::showMessage(QString msg) { ui->message->setText(msg); }
