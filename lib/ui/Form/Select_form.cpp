#include "Form/Select_form.h"
#include "kernel/core.h"
#include "ui_Select_form_imp.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace Bygone {
namespace Frontend {
    Select_form::Select_form(QWidget* parent)
        : QDialog(parent)
        , ui(new Ui::Select_form)
    {
        ui->setupUi(this);

        //! load list
        using namespace boost::filesystem;

        std::for_each(directory_iterator("Resource/Scenario"), directory_iterator(), [this](const directory_entry& e) {
            if (e.path().extension() == ".bys") {
                load_config_(e.path().string());
            }
        });

        //! ship models
        QStringList sc;

        for (auto& json : detail_) {
            sc << json.get<std::string>("name").c_str();

            QStringList model;
            for (auto& child : json.get_child("leaders")) {
                auto& leader = child.second;
                model << leader.get<std::string>("name").c_str();
            }

            leaders_.push_back(Model_ptr(new QStringListModel(model)));
        }

        scenario_.reset(new QStringListModel(sc));

        ui->ScenarioList->setModel(scenario_.get());
    }

    Select_form::~Select_form() { delete ui; }

    void Select_form::load_config_(const std::string& s)
    {
        boost::property_tree::ptree json;
        boost::property_tree::json_parser::read_json(s, json);
        detail_.push_back(std::move(json));
    }

    void Select_form::accept()
    {
        QDialog::accept();

        auto& scene = detail_.at(ui->ScenarioList->currentIndex().row());
        auto frow = ui->FractionList->currentIndex().row();

        auto leader = scene.get_child("leaders").begin();
        std::advance(leader, frow);

        auto id = leader->second.get<int>("id");

        emit selectDone(scene.get<std::string>("config"), id);
    }
}
}

void Bygone::Frontend::Select_form::on_FractionList_clicked(const QModelIndex& index) { ui->Start->setEnabled(true); }

void Bygone::Frontend::Select_form::on_ScenarioList_activated(const QModelIndex& index)
{
    auto& leader = leaders_.at(index.row());
    auto& item = detail_.at(index.row());

    ui->FractionList->setModel(leader.get());
    ui->ScenarioBrief->setText(item.get<std::string>("brief").c_str());

    ui->Start->setEnabled(false);
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
