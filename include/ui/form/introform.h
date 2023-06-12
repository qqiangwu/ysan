#ifndef INTRO_FORM_H
#define INTRO_FORM_H

#include <QtWidgets/QWidget>

#include <memory>

#include "foundation/config_dict.h"

namespace Ui {
class IntroForm;
}

namespace Bygone {
class IntroForm : public QWidget {
    Q_OBJECT

public:
    IntroForm(const yrui::Config_dict& conf, QWidget* parent);

    virtual ~IntroForm();

signals:
    void startClicked();
    void memoryClicked();
    void aboutClicked();
    void settingClicked();
    void exitClicked();

private:
    std::unique_ptr<Ui::IntroForm> ui;
};
}
#endif // INTRO_FORM_H
