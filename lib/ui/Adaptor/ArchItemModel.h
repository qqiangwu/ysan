#ifndef ARCHITEMMODEL_H
#define ARCHITEMMODEL_H

#include "kernel/component_fwd.h"
#include <QAbstractTableModel>
#include <functional>
#include <vector>

class QTableView;
class QItemSelectionModel;

namespace Bygone {
namespace Frontend {
    constexpr struct Select_neighbor_tag {
    } select_neighbor;
    constexpr struct Cheat_tag {
    } cheat;

    class ArchItemModel : public QAbstractTableModel {
    public:
        Q_OBJECT

    public:
        typedef ArchItemModel self_type;
        typedef QAbstractTableModel base_type;
        typedef yrui::Architecture* value_type;
        typedef std::function<bool(value_type)> filter_type;
        typedef std::vector<value_type> container_type;

    public:
        static ArchItemModel* excludes(yrui::Fraction* f, yrui::Architecture* a);
        static ArchItemModel* all();
        static ArchItemModel* filter(filter_type flt);
        static ArchItemModel* select(container_type&& archs);

    public:
        explicit ArchItemModel(yrui::Fraction* fraction);
        explicit ArchItemModel(yrui::Architecture* arch);

        ArchItemModel(yrui::Architecture*, Select_neighbor_tag);
        ArchItemModel(yrui::Architecture*, Cheat_tag);

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
        ArchItemModel() = default;

        container_type values_;
        int detail_ = 0;
    };
}
}

#endif // ARCHITEMMODEL_H
