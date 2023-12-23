#include "CityInfo.h"
#include "ui_CityInfo_imp.h"

#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    CityInfo::CityInfo(QWidget* parent)
        : QWidget(parent)
        , ui(new Ui::CityInfo)
    {
        ui->setupUi(this);
        this->layout()->setSizeConstraint(QLayout::SetFixedSize);
    }

    CityInfo::~CityInfo() { delete ui; }

    void CityInfo::showArch(yrui::Architecture* arch, bool detail)
    {
        using namespace yrui::person;
        using namespace yrui::fraction;

        // Clear to default value
        ui->leader_name->setText("");
        ui->mayor_name->setText("");

        //! set detail
        //! @{
        ui->detail->setVisible(detail);
        //! @}

        //! prepare values
        //! @{
        ui->city_name->setText(arch->name().c_str());
        if (arch->fraction()) {
            auto f = arch->fraction();
            auto l = f->leader();
            ui->leader_name->setText(l->name().c_str());
            if (arch->mayor()) {
                ui->mayor_name->setText(arch->mayor()->name().c_str());
            }
        }

        ui->steady->setText(QString::number(arch->steady()));
        ui->security->setText(QString::number(arch->security()));

        if (detail) {
            ui->money->setText(QString::number(arch->money()));
            ui->food->setText(QString::number(arch->food()));
            ui->person_count->setText(
                QString("%1/%2/%3").arg(arch->num_unworked()).arg(arch->num_in_city()).arg(arch->num_all()));
            ui->population->setText(QString("%1 户").arg(arch->population()));
            ui->trade->setText(QString::number(arch->trade()));
            ui->farm->setText(QString::number(arch->farming()));
            ui->corrupt->setText(QString::number(arch->corruption()));
            ui->escuage->setText(QString::number(arch->escuage()));
            ui->noble->setText(QString::number(arch->aristocrat()));
            ui->troop->setText(QString::number(arch->troop()));
        }
        //! @}

        this->show();
    }
}
}
