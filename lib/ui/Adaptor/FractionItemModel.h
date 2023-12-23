#ifndef FRACTIONITEMMODEL_H
#define FRACTIONITEMMODEL_H

#include "kernel/component_fwd.h"
#include <QAbstractTableModel>
#include <functional>
#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Bygone {
namespace Frontend {
    class FractionItemModel : public QAbstractTableModel {
    public:
        Q_OBJECT

    public:
        typedef FractionItemModel self_type;
        typedef QAbstractTableModel base_type;
        typedef yrui::Fraction* value_type;
        typedef std::function<bool(value_type)> filter_type;
        typedef std::vector<value_type> container_type;

    public:
        static FractionItemModel* all();

    public:
        void apply(QTableView* view);
        value_type get(int index);
        container_type get(QItemSelectionModel* selection);

    public:
        virtual int rowCount(const QModelIndex& parent) const override;
        virtual int columnCount(const QModelIndex& parent) const override;
        virtual QVariant data(const QModelIndex& index, int role) const override;
        virtual QVariant headerData(int section, Qt::Orientation, int role) const override;

    private:
        FractionItemModel() = default;

        container_type values_;
        int detail_ = 0;
    };
}
}

#endif // FRACTIONITEMMODEL_H
