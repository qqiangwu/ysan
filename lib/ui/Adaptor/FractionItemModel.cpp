#include "Adaptor/FractionItemModel.h"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableView>

#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"

namespace Bygone {
namespace Frontend {
    using namespace boost::range;
    using namespace boost::adaptors;

    FractionItemModel* FractionItemModel::all()
    {
        using yrui::fraction::Fraction_mgr;

        auto& mgr = yrui::use_subsys<Fraction_mgr>();

        auto ret = new FractionItemModel;

        push_back(ret->values_, mgr.as_range());

        return ret;
    }

    void FractionItemModel::apply(QTableView* view) { view->setModel(this); }

    FractionItemModel::value_type FractionItemModel::get(int index) { return values_.at(index); }

    FractionItemModel::container_type FractionItemModel::get(QItemSelectionModel* selection)
    {
        container_type ret;

        std::set<int> rows;

        for (auto& idx : selection->selectedIndexes()) {
            rows.insert(idx.row());
        }

        for (auto i : rows)
            ret.push_back(values_.at(i));

        return ret;
    }

    int FractionItemModel::rowCount(const QModelIndex& parent) const { return values_.size(); }

    int FractionItemModel::columnCount(const QModelIndex&) const { return 5; }

    QVariant FractionItemModel::data(const QModelIndex& index, int role) const
    {
        if (role != Qt::DisplayRole)
            return {};

        auto f = values_.at(index.row());

        using namespace yrui::fraction;

        switch (index.column()) {
        case 0:
            return f->name().c_str();
        case 1:
            return f->capital()->name().c_str();
        case 2:
            return f->arch_count();
        case 3:
            return f->politics();
        case 4:
            return f->action_power();
        }
        return {};
    }

    QVariant FractionItemModel::headerData(int section, Qt::Orientation o, int role) const
    {
        if (role != Qt::DisplayRole || o != Qt::Horizontal)
            return {};

        switch (section) {
        case 0:
            return { "势力" };
        case 1:
            return { "都城" };
        case 2:
            return { "城池" };
        case 3:
            return { "政治" };
        case 4:
            return { "行动" };
        }

        return {};
    }
}
}
