#include "Widget/InfoBar.h"
#include "Form/ByScreen.h"
#include "Main/Byapp.h"
#include "ScreenView/Mapview.h"
#include "ui_InfoBar_imp.h"

#include "kernel/arch/architecture.h"
#include "kernel/core/timeline.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_controller.h"

namespace Bygone {
namespace Frontend {
    InfoBar::InfoBar(MapView* parent, yrui::Fraction* f)
        : QWidget(parent)
        , ui(new Ui::InfoBar)
        , view_(parent)
    {
        ui->setupUi(this);

        using yrui::Timeline;

        auto& timeline = yrui::use_subsys<Timeline>();

        timeline.month_begin.connect(
            boost::signals2::at_front, [this](Timeline& t) { ui->month->setText(QString::number(t.month())); });
        timeline.year_begin.connect(
            boost::signals2::at_front, [this](Timeline& t) { ui->year->setText(QString::number(t.year())); });

        ui->month->setText(QString::number(timeline.month()));
        ui->year->setText(QString::number(timeline.year()));

        ui->cityCount->setText(QString::number(f->arch_count()));
        ui->capital->setText(f->capital()->name().c_str());

        this->setAutoFillBackground(true);

        this->move(330, 5);
    }

    void InfoBar::updateBar()
    {
        auto self = view_->app()->playerFraction();
        auto ctrl = view_->app()->controller();
        this->ui->cityCount->setText(QString::number(self->arch_count()));
        this->ui->pp->setText(QString::number(self->politics()));
        this->ui->ap->setText(QString("%1/%2").arg(ctrl->current_ap()).arg(self->action_power()));
        this->ui->resource->setText(QString("%1/%2").arg(self->money()).arg(self->food()));
    }

    InfoBar::~InfoBar() { delete ui; }
}
}
