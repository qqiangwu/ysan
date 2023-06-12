#ifndef SPLASHFORM_H
#define SPLASHFORM_H

#include <QtCore/QString>
#include <QtWidgets/QWidget>

#include <memory>

#include "foundation/config_dict.h"

namespace Ui {
class SplashForm;
}

namespace Bygone {
class SplashForm : public QWidget {
    Q_OBJECT

public:
    SplashForm(const yrui::Config_dict& conf, QWidget* parent);

    virtual ~SplashForm();

public slots:
    void showMessage(QString message);

private:
    std::unique_ptr<Ui::SplashForm> ui;
};
} // of namespace Bygone

#endif // SPLASHFORM_H
