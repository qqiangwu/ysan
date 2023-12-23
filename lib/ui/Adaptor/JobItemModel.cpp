#include "Adaptor/JobItemModel.h"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableView>

#include "kernel/arch/architecture.h"
#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    using namespace boost::range;
    using namespace boost::adaptors;

    JobItemModel* JobItemModel::inArch(yrui::Architecture* arch)
    {
        auto ret = new JobItemModel;

        for (auto& j : arch->jobs()) {
            ret->values_.push_back({ j->person()->name().c_str(), typeid(j.get()).name() });
        }

        return ret;
    }

    void JobItemModel::apply(QTableView* view) { view->setModel(this); }

    int JobItemModel::rowCount(const QModelIndex& parent) const { return values_.size(); }

    int JobItemModel::columnCount(const QModelIndex&) const { return 2; }

    QVariant JobItemModel::data(const QModelIndex& index, int role) const
    {
        if (role != Qt::DisplayRole)
            return {};

        auto j = values_.at(index.row());

        using namespace yrui::fraction;

        switch (index.column()) {
        case 0:
            return j.first;
        case 1:
            return j.second;
        }
        return {};
    }

    QVariant JobItemModel::headerData(int section, Qt::Orientation o, int role) const
    {
        if (role != Qt::DisplayRole || o != Qt::Horizontal)
            return {};

        switch (section) {
        case 0:
            return { "人物" };
        case 1:
            return { "工作" };
        }

        return {};
    }
}
}
