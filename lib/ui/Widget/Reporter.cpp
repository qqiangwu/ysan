#include "Widget/Reporter.h"
#include "ui_Reporter_imp.h"

#include <QLabel>

namespace Bygone {
namespace Frontend {

    Reporter::Reporter(QWidget* parent)
        : QDialog(parent, Qt::FramelessWindowHint)
        , ui(new Ui::Reporter)
    {
        ui->setupUi(this);

        ui->image->setAutoFillBackground(true);
    }

    Reporter::~Reporter() { delete ui; }

    namespace {
        void setPixmap(QLabel* l, const QPixmap& img = {})
        {
            auto pal = l->palette();
            pal.setBrush(l->backgroundRole(), QBrush(img));
            l->setPalette(pal);
        }
    }

    void Reporter::report(const QString& info)
    {
        setPixmap(this->ui->image);
        this->ui->info->setText(info);
        this->raise();
        this->exec();
    }

    void Reporter::report(const QString& title, const QString& info)
    {
        this->setWindowTitle(title);
        this->report(info);
    }

    void Reporter::report(const QPixmap& img, const QString& info)
    {
        setPixmap(ui->image, img);
        ui->info->setText(info);

        this->raise();
        this->exec();
    }
}
}
