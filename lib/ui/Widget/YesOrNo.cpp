#include "YesOrNo.h"
#include "ui_YesOrNo_imp.h"

namespace Bygone {
namespace Frontend {
    YesOrNo::YesOrNo(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::YesOrNo)
    {
        ui->setupUi(this);
    }

    YesOrNo::~YesOrNo() { delete ui; }

    bool YesOrNo::ask(const QString& str)
    {
        ui->questioin->setText(str);

        return !!this->exec();
    }
}
}
