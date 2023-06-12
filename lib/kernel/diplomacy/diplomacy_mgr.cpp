#include <algorithm>

#include "kernel/diplomacy/diplomacy_mgr.h"

namespace yrui {
namespace diplomacy {
    void Diplomacy_mgr::load(Relation_table rel)
    {
        // BOOST_ASSERT_MSG(!rel.empty(), "bad relation table format");
        BOOST_ASSERT_MSG(std::all_of(std::begin(rel), std::end(rel),
                             [](decltype(*rel.cbegin()) x) { return x.first.first < x.first.second; }),
            "bad relation table format");

        table_ = std::move(rel);
    }

    namespace {
        Relation& get_impl(Relation_table& table_, id_t x, id_t y)
        {
            BOOST_ASSERT_MSG(x != 0 && y != 0 && x != y, "bad argument");

            auto p = std::minmax(x, y);
            // BOOST_ASSERT_MSG(table_.count(p), "bad argument");

            return table_[p];
        }
    } // of namespace unnamed

    Relation Diplomacy_mgr::get_relation(id_t x, id_t y) const
    {
        return get_impl(const_cast<Relation_table&>(table_), x, y);
    }

    Relation Diplomacy_mgr::set_relation(id_t x, id_t y, Relation rel) { return get_impl(table_, x, y) = rel; }

    void Diplomacy_mgr::update() { }

    bool Diplomacy_mgr::init_impl() { return true; }

    void Diplomacy_mgr::finalize_impl() { }

    void Diplomacy_mgr::on_month_begin(Timeline& t) { }

    void Diplomacy_mgr::on_month_end(Timeline& t) { }

    void Diplomacy_mgr::on_year_begin(Timeline& t) { }

    void Diplomacy_mgr::on_year_end(Timeline& t) { }

    bool Diplomacy_mgr::self_check() const { return {}; }
} // of namespace diplomacy
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
