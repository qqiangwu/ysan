#include <QtWidgets/QPushButton>

#include <cassert>

#include "ui/form/introform.h"
#include "ui/support/utility.h"
#include "ui_introformimp.h"

Bygone::IntroForm::IntroForm(const yrui::Config_dict& conf, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::IntroForm)
{
    ui->setupUi(this);

    // set up signals
    connect(ui->start, &QPushButton::clicked, this, &startClicked);
    connect(ui->memory, &QPushButton::clicked, this, &memoryClicked);
    connect(ui->about, &QPushButton::clicked, this, &aboutClicked);
    connect(ui->setting, &QPushButton::clicked, this, &settingClicked);
    connect(ui->exit, &QPushButton::clicked, this, &exitClicked);

    ui->start->setText(conf.get("start", "Start").c_str());
    ui->memory->setText(conf.get("memory", "Memory").c_str());
    ui->about->setText(conf.get("about", "About").c_str());
    ui->setting->setText(conf.get("setting", "Setting").c_str());
    ui->exit->setText(conf.get("exit", "Exit").c_str());

    if (auto bk = conf.get_optional<std::string>("background")) {
        setBackground(this, bk.get());
    }
}

Bygone::IntroForm::~IntroForm() = default;

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
