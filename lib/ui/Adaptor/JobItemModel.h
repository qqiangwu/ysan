#ifndef JOBITEMMODEL_H
#define JOBITEMMODEL_H

#include "kernel/component_fwd.h"
#include "kernel/job/arch_job.h"
#include <QAbstractTableModel>
#include <functional>
#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Bygone {
namespace Frontend {
    class JobItemModel : public QAbstractTableModel {
    public:
        Q_OBJECT

    public:
        typedef JobItemModel self_type;
        typedef QAbstractTableModel base_type;
        typedef std::pair<QString, QString> value_type;
        typedef std::vector<value_type> container_type;

    public:
        static JobItemModel* inArch(yrui::Architecture* arch);

    public:
        void apply(QTableView* view);

    public:
        virtual int rowCount(const QModelIndex& parent) const override;
        virtual int columnCount(const QModelIndex& parent) const override;
        virtual QVariant data(const QModelIndex& index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation, int role) const override;

    private:
        JobItemModel() = default;

        container_type values_;
    };
} // of namespace Frontend
} // of namespace Bygone

#endif // JOBITEMMODEL_H
