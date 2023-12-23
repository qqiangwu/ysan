#include "Adaptor/PersonItemModel.h"

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/algorithm_ext/push_back.hpp>
#include <set>

#include <QHeaderView>
#include <QItemSelectionModel>
#include <QTableView>

#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"
#include "kernel/person/person_mgr.h"
#include "kernel/resource/string_table.h"
#include <Detail/Filter.h>

namespace Bygone {
namespace Frontend {
    using namespace boost::range;
    using namespace boost::adaptors;

    PersonItemModel* PersonItemModel::all()
    {
        using yrui::person::Person_mgr;

        auto& mgr = yrui::use_subsys<Person_mgr>();

        auto ret = new PersonItemModel;

        push_back(ret->values_, mgr.appeared());

        return ret;
    }

    PersonItemModel* PersonItemModel::canPromote(yrui::Architecture* arch)
    {
        auto promotable = [](yrui::Person* p) { return p->position() < yrui::person::max_position(p); };

        auto ret = new PersonItemModel;

        push_back(ret->values_, arch->in_city() | filtered(promotable));

        return ret;
    }

    PersonItemModel* PersonItemModel::without_troops(yrui::Architecture* arch)
    {
        auto have_no_troop = [](yrui::Person* p) { return !p->has_troop(); };

        auto ret = new PersonItemModel;

        push_back(ret->values_, arch->unworked() | filtered(have_no_troop));

        return ret;
    }

    PersonItemModel* PersonItemModel::canBeTrained(yrui::Architecture* arch)
    {
        auto has_troop = [](yrui::Person* p) { return p->has_troop() && p->is_supplied(); };

        auto ret = new PersonItemModel;

        push_back(ret->values_, arch->unworked() | filtered(has_troop));

        return ret;
    }

    PersonItemModel* PersonItemModel::need_replenish(yrui::Architecture* arch)
    {
        auto has_troop = [](yrui::Person* p) { return p->has_troop() && p->troop() != p->max_troop(); };

        auto ret = new PersonItemModel;

        push_back(ret->values_, arch->unworked() | filtered(has_troop));

        return ret;
    }

    PersonItemModel* PersonItemModel::hasTroop(yrui::Architecture* arch)
    {
        auto has_troop = [](yrui::Person* p) { return p->has_troop(); };

        auto ret = new PersonItemModel;

        push_back(ret->values_, arch->unworked() | filtered(has_troop));

        return ret;
    }

    PersonItemModel* PersonItemModel::unofficial(yrui::Architecture* arch)
    {
        using yrui::person::Person_mgr;

        auto& mgr = yrui::use_subsys<Person_mgr>();

        auto is_unofficial
            = [arch](yrui::Person* p) { return p->stayed_city() == arch && p->fraction() == nullptr; };

        auto ret = new PersonItemModel;

        push_back(ret->values_, mgr.out_of_office() | filtered(is_unofficial));

        return ret;
    }

    PersonItemModel::PersonItemModel(yrui::Architecture* arch) { push_back(values_, arch->all()); }

    PersonItemModel::PersonItemModel(yrui::Fraction* fraction) { push_back(values_, fraction->persons()); }

    PersonItemModel::PersonItemModel(yrui::Architecture* arch, not_work_t) { push_back(values_, arch->unworked()); }

    PersonItemModel::PersonItemModel(yrui::Architecture* arch, not_crashed_t)
    {
        push_back(values_, arch->unworked() | filtered([](yrui::Person* p) { return !p->is_troop_crashed(); }));
    }

    void PersonItemModel::apply(QTableView* view) { view->setModel(this); }

    PersonItemModel::value_type PersonItemModel::get(int index) { return values_.at(index); }

    PersonItemModel::container_type PersonItemModel::get(QItemSelectionModel* selection)
    {
        BOOST_ASSERT(selection);

        container_type ret;
        std::set<int> rows;

        for (auto& idx : selection->selectedIndexes()) {
            rows.insert(idx.row());
        }

        for (auto i : rows)
            ret.push_back(values_.at(i));

        return ret;
    }

    int PersonItemModel::rowCount(const QModelIndex& parent) const { return values_.size(); }

    int PersonItemModel::columnCount(const QModelIndex&) const { return 13; }

    QVariant PersonItemModel::data(const QModelIndex& index, int role) const
    {
        if (role != Qt::DisplayRole)
            return {};

        using namespace yrui::arch;
        using namespace yrui::fraction;
        using namespace yrui::person;

        auto p = values_.at(index.row());

        switch (index.column()) {
        case 0:
            return p->name().c_str();
        case 1: {
            if (p->fraction()) {
                auto l = p->fraction()->leader();
                return l->name().c_str();
            } else {
                return "";
            }
        }

            //! \todo   fix this
        case 2:
            return p->stayed_city()->name().c_str();
        case 3:
            return filter_position(p->position());
        case 4:
            return p->feat();
        case 5:
            return filter_ability(p->fight());
        case 6:
            return filter_ability(p->leadership());
        case 7:
            return filter_ability(p->wisdom());
        case 8:
            return filter_ability(p->politics());

        case 9:
            return filter_troop(p);
        case 10:
            return filter_morale(p);
        case 11:
            return filter_drill(p);
        case 12:
            return filter_supply(p);
        }

        return {};
    }

    QVariant PersonItemModel::headerData(int section, Qt::Orientation o, int role) const
    {
        if (role != Qt::DisplayRole || o != Qt::Horizontal)
            return {};

        switch (section) {
        case 0:
            return { "姓名" };
        case 1:
            return { "势力" };
        case 2:
            return { "驻地" };
        case 3:
            return { "官位" };
        case 4:
            return { "功勋" };
        case 5:
            return { "武力" };
        case 6:
            return { "统率" };
        case 7:
            return { "谋略" };
        case 8:
            return { "政治" };
        case 9:
            return { "军队" };
        case 10:
            return { "士气" };
        case 11:
            return { "训练" };
        case 12:
            return { "补给" };
        }

        return {};
    }
}
}
