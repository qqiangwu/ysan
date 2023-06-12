#ifndef SELECT_FORM_H
#define SELECT_FORM_H

#include <QtCore/QStringListModel>
#include <QtWidgets/QWidget>

#include <memory>
#include <string>
#include <vector>

#include "foundation/config_dict.h"

namespace Ui {
class SelectForm;
}

namespace Bygone {
namespace Detail {
    struct ScenarioInfo {
        QString name;
        QString brief;
        std::unique_ptr<QStringListModel> fractions;
        std::string configFile;
        std::vector<int> fractionId;
    };
} // of namespace Detail

class SelectForm : public QWidget {
    Q_OBJECT

public:
    SelectForm(const yrui::Config_dict& conf, std::vector<std::string> scenarios, QWidget* parent);

    virtual ~SelectForm();

signals:
    void cancelClicked();
    void selectionDone(const std::string& scenarioConfig, const int leaderId);

private:
    std::unique_ptr<Ui::SelectForm> ui;
    std::vector<Detail::ScenarioInfo> scenarios_;
};
}

#endif // SELECT_FORM_H
