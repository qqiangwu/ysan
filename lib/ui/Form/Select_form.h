#ifndef SELECT_FORM_H
#define SELECT_FORM_H

#include <QDialog>
#include <QStringListModel>
#include <memory>
#include <vector>

#include <boost/property_tree/ptree.hpp>

namespace Ui {
class Select_form;
}

namespace Bygone {
namespace Frontend {
    class Select_form : public QDialog {
        Q_OBJECT

    public:
        explicit Select_form(QWidget* parent = 0);
        ~Select_form();

    public:
        virtual void accept() override;

    signals:
        void selectDone(const std::string& sc, int fraction);

    private slots:
        void on_FractionList_clicked(const QModelIndex& index);
        void on_ScenarioList_activated(const QModelIndex& index);

    private:
        void load_config_(const std::string& s);

    private:
        using Model_ptr = std::unique_ptr<QStringListModel>;

        Model_ptr scenario_;
        std::vector<Model_ptr> leaders_;
        std::vector<boost::property_tree::ptree> detail_;

    private:
        Ui::Select_form* ui;
    };
}
}
#endif // SELECT_FORM_H
