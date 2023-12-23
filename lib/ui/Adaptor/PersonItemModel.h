#ifndef PERSONITEMMODEL_H
#define PERSONITEMMODEL_H

#include "kernel/component_fwd.h"
#include <QAbstractTableModel>
#include <functional>
#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Bygone {
namespace Frontend {
    constexpr struct not_work_t {
    } not_work;
    constexpr struct not_crashed_t {
    } not_crashed;

    class PersonItemModel : public QAbstractTableModel {
    public:
        Q_OBJECT

    public:
        typedef PersonItemModel self_type;
        typedef QAbstractTableModel base_type;
        typedef yrui::Person* value_type;
        typedef std::function<bool(value_type)> filter_type;
        typedef std::vector<value_type> container_type;

    public:
        static PersonItemModel* unofficial(yrui::Architecture* arch);
        static PersonItemModel* all();
        static PersonItemModel* without_troops(yrui::Architecture* arch);
        static PersonItemModel* canBeTrained(yrui::Architecture* arch);
        static PersonItemModel* need_replenish(yrui::Architecture* arch);
        static PersonItemModel* canPromote(yrui::Architecture* arch);
        static PersonItemModel* hasTroop(yrui::Architecture* arch);

    public:
        explicit PersonItemModel(yrui::Architecture* arch);
        explicit PersonItemModel(yrui::Fraction* fraction);

        PersonItemModel(yrui::Architecture* arch, not_work_t);
        PersonItemModel(yrui::Architecture* arch, not_crashed_t);

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
        PersonItemModel() = default;

        container_type values_;
    };
}
}

#endif // PERSONITEMMODEL_H
