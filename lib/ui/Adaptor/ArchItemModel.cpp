#include "Adaptor/ArchItemModel.h"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableView>

#include <set>

#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/common/common_util.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person.h"
#include "kernel/resource/string_table.h"

namespace Bygone {
namespace Frontend {
    using namespace boost::range;
    using namespace boost::adaptors;

    ArchItemModel* ArchItemModel::all()
    {
        using yrui::arch::Architecture_mgr;

        auto& mgr = yrui::use_subsys<Architecture_mgr>();

        auto ret = new ArchItemModel;

        ret->detail_ = 14;
        push_back(ret->values_, mgr.as_range());

        return ret;
    }

    ArchItemModel* ArchItemModel::excludes(yrui::Fraction* f, yrui::Architecture* a)
    {
        BOOST_ASSERT(f == a->fraction());

        auto ret = new ArchItemModel(f);

        ret->detail_ = 14;
        ret->values_.erase(std::find(ret->values_.begin(), ret->values_.end(), a));

        return ret;
    }

    ArchItemModel* ArchItemModel::select(container_type&& archs)
    {
        auto ret = new ArchItemModel;

        ret->detail_ = 14;
        ret->values_ = std::move(archs);

        return ret;
    }

    ArchItemModel* ArchItemModel::filter(filter_type flt)
    {
        using yrui::arch::Architecture_mgr;

        auto& mgr = yrui::use_subsys<Architecture_mgr>();

        auto ret = new ArchItemModel;

        ret->detail_ = 14;
        push_back(ret->values_, mgr.as_range() | filtered(flt));

        return ret;
    }

    ArchItemModel::ArchItemModel(yrui::Fraction* fraction)
        : detail_(14)
    {
        auto& mgr = yrui::use_subsys<yrui::arch::Architecture_mgr>();
        auto in_fraction = [fraction](yrui::Architecture* p) { return p->fraction() == fraction; };

        push_back(values_, mgr.as_range() | filtered(in_fraction));
    }

    ArchItemModel::ArchItemModel(yrui::Architecture* city, Select_neighbor_tag)
        : detail_(5)
    {
        auto ads = yrui::common::map().adjacent_city(city);
        auto& mgr = yrui::use_subsys<yrui::military::Military_mgr>();

        auto is_enemy
            = [=, &mgr](yrui::Architecture* c) { return c->fraction() != city->fraction() && !mgr.has_troop(city, c); };

        push_back(values_, ads | filtered(is_enemy));
    }

    ArchItemModel::ArchItemModel(yrui::Architecture* arch, Cheat_tag)
        : detail_(14)
    {
        values_.push_back(arch);
    }

    ArchItemModel::ArchItemModel(yrui::Architecture* arch)
        : detail_(14)
    {
        values_.push_back(arch);
    }

    void ArchItemModel::apply(QTableView* view) { view->setModel(this); }

    ArchItemModel::value_type ArchItemModel::get(int index) { return values_.at(index); }

    ArchItemModel::container_type ArchItemModel::get(QItemSelectionModel* selection)
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

    int ArchItemModel::rowCount(const QModelIndex& parent) const { return values_.size(); }

    int ArchItemModel::columnCount(const QModelIndex&) const { return detail_; }

    QVariant ArchItemModel::data(const QModelIndex& index, int role) const
    {
        if (role != Qt::DisplayRole)
            return {};

        auto arch = values_.at(index.row());

        using namespace yrui::fraction;

        switch (index.column()) {
        case 0:
            return arch->name().c_str();
        case 1:
            if (arch->fraction()) {
                return arch->fraction()->name().c_str();
            } else {
                return "--";
            }

        case 2:
            if (auto mayor = arch->mayor()) {
                return mayor->name().c_str();
            } else {
                return "--";
            }

        case 3:
            return arch->security();
        case 4:
            return arch->steady();
        case 5:
            return arch->escuage();
        case 6:
            return arch->farming();
        case 7:
            return arch->trade();
        case 8:
            return arch->population();
        case 9:
            return arch->aristocrat();
        case 10:
            return arch->corruption();
        case 11:
            return arch->money();
        case 12:
            return arch->food();
        case 13:
            return arch->troop();
        }
        return {};
    }

    QVariant ArchItemModel::headerData(int section, Qt::Orientation o, int role) const
    {
        if (role != Qt::DisplayRole || o != Qt::Horizontal)
            return {};

        switch (section) {
        case 0:
            return { "城名" };
        case 1:
            return { "势力" };
        case 2:
            return { "太守" };
        case 3:
            return { "统治" };
        case 4:
            return { "城防" };

        case 5:
            return { "兵役" };
        case 6:
            return { "农业" };
        case 7:
            return { "商业" };
        case 8:
            return { "人口" };

        case 9:
            return { "世家" };
        case 10:
            return { "腐败" };

        case 11:
            return { "金" };
        case 12:
            return { "米" };
        case 13:
            return { "兵力" };
        }

        return {};
    }
}
}
