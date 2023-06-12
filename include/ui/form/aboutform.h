#ifndef ABOUT_FORM_H
#define ABOUT_FORM_H

#include <QtWidgets/QWidget>

#include <memory>

#include "foundation/config_dict.h"

namespace Ui {
class AboutForm;
}

namespace Bygone {
class AboutForm : public QWidget {
    Q_OBJECT

public:
    AboutForm(const yrui::Config_dict& conf, QWidget* parent);

    virtual ~AboutForm();

signals:
    void confirmClicked();

private:
    std::unique_ptr<Ui::AboutForm> ui;
};
}
#endif // ABOUT_FORM_H
