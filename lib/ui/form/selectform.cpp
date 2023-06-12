#include <QtCore/QStringList>

#include <boost/property_tree/json_parser.hpp>

#include "foundation/config_dict.h"

#include "ui/form/SelectForm.h"
#include "ui/support/utility.h"
#include "ui_SelectFormimp.h"

namespace {
Bygone::Detail::ScenarioInfo loadScenario(const std::string& scenario)
{
    yrui::Config_dict detail;

    boost::property_tree::read_json(scenario, detail);

    Bygone::Detail::ScenarioInfo info;

    info.name = detail.get<std::string>("name").c_str();
    info.brief = detail.get<std::string>("brief").c_str();
    info.configFile = detail.get<std::string>("config").c_str();

    QStringList fractions;

    for (auto& child : detail.get_child("leaders")) {
        auto name = child.second.get<std::string>("name");
        auto id = child.second.get<int>("id");

        info.fractionId.push_back(id);

        fractions.append(name.c_str());
    }

    info.fractions = std::make_unique<QStringListModel>(fractions);

    return info;
}
}

Bygone::SelectForm::SelectForm(const yrui::Config_dict& conf, std::vector<std::string> scenarios, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SelectForm)
{
    ui->setupUi(this);

    connect(ui->cancel, &QPushButton::clicked, this, &cancelClicked);

    // setup ui
    if (auto bk = conf.get_optional<std::string>("background")) {
        setBackground(this, bk.get());
    }

    ui->start->setText(conf.get("start", "Start").c_str());
    ui->cancel->setText(conf.get("cancel", "Cancel").c_str());

    // load scenarios
    QStringList scenarioLists;

    for (const auto& scenario : scenarios) {
        auto sc = loadScenario(scenario);

        scenarioLists.append(sc.name);
        scenarios_.push_back(std::move(sc));
    }

    // populate scenario list
    ui->scenarioList->setModel(new QStringListModel(scenarioLists, ui->scenarioList));

    // setup connections
    connect(ui->scenarioList, &QListView::activated, [this](const QModelIndex& i) {
        auto& info = scenarios_[i.row()];

        ui->fractionList->setModel(info.fractions.get());
        ui->scenarioBrief->setText(info.brief);
    });

    connect(ui->fractionList, &QListView::activated, [this](const QModelIndex& i) { ui->start->setEnabled(this); });

    connect(ui->start, &QPushButton::clicked, [this] {
        const auto& sc = scenarios_[ui->scenarioList->currentIndex().row()];
        const auto& id = sc.fractionId[ui->fractionList->currentIndex().row()];

        emit selectionDone(sc.configFile, id);
    });
}

Bygone::SelectForm::~SelectForm() = default;

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
