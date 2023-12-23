#ifndef SELECTDIALOG_H
#define SELECTDIALOG_H

#include <QDialog>
#include <QTableView>
#include <vector>

class QAbstractItemModel;

namespace Ui {
class SelectDialog;
}

namespace Bygone {
namespace Frontend {
    class SelectDialog : public QDialog {
        Q_OBJECT

    public:
        explicit SelectDialog(QWidget* parent = nullptr);
        virtual ~SelectDialog();

        template <class ItemModel> typename ItemModel::value_type exec_one(ItemModel* model)
        {
            prepare_();
            model->apply(table_);
            table_->setSelectionMode(QTableView::SingleSelection);
            if (this->exec() == QDialog::Accepted) {
                return model->get(table_->currentIndex().row());
            }

            return {};
        }

        template <class ItemModel> std::vector<typename ItemModel::value_type> exec_multiply(ItemModel* model)
        {
            prepare_();
            model->apply(table_);
            table_->setSelectionMode(QTableView::MultiSelection);
            if (this->exec() == QDialog::Accepted) {
                return model->get(table_->selectionModel());
            }
            return {};
        }

        template <class ItemModel> void exec_display(ItemModel* model)
        {
            prepare_();
            model->apply(table_);
            table_->setSelectionMode(QTableView::NoSelection);
            this->exec();
        }

    private:
        void prepare_();

    private slots:
        void on_tableView_activated(const QModelIndex& index);

    private:
        Ui::SelectDialog* ui;
        QTableView* table_;
    };
}
}

#endif // SELECTDIALOG_H
