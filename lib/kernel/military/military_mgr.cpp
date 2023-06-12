#include <boost/range/algorithm/find_if.hpp>

#include "kernel/military/military_mgr.h"
#include "kernel/military/strategy.h"

namespace yrui {
namespace military {
    namespace {
        Military_mgr::TStrategy_container load_troop_strategies()
        {
            using namespace strategy;

            Military_mgr::TStrategy_container ret;

            ret.push_back(make_uptr<Disturb>());
            ret.push_back(make_uptr<Raid>());
            ret.push_back(make_uptr<Ambush>());
            ret.push_back(make_uptr<Fire>());
            ret.push_back(make_uptr<Strike>());

            return ret;
        }

        Military_mgr::DStrategy_container load_defence_strategies()
        {
            using namespace strategy;
            Military_mgr::DStrategy_container ret;

            ret.push_back(make_uptr<Bluff>());
            ret.push_back(make_uptr<Sneak>());

            return ret;
        }

        Military_mgr::AStrategy_container load_arch_strategies()
        {
            using namespace strategy;

            Military_mgr::AStrategy_container ret;

            ret.push_back(make_uptr<Burn>());
            ret.push_back(make_uptr<Wreck>());
            ret.push_back(make_uptr<Traitor>());

            return ret;
        }
    }

    void Military_mgr::on_month_begin(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("MilitaryMgr");
        MLOG() << "on month begin";
        /* empty now */
    }

    /* start the war */
    void Military_mgr::on_month_end(Timeline& t)
    {
        BOOST_LOG_NAMED_SCOPE("MilitaryMgr");
        MLOG() << "on month end";
        schedule_battle_();

        legion_cache_.clear();
        legions_.clear();
    }

    void Military_mgr::update()
    { /* empty now */
    }

    bool Military_mgr::init_impl()
    {
        //! load all strategies
        tst_ = load_troop_strategies();
        dst_ = load_defence_strategies();
        ast_ = load_arch_strategies();

        return true;
    }

    void Military_mgr::finalize_impl()
    { /* empty now */
    }

    //////////////////////////////////////////////////////////////////////
    void Military_mgr::load(resource::Map::graph_type&& map) { map_.reset(new resource::Map { std::move(map) }); }

    resource::Map& Military_mgr::map()
    {
        BOOST_ASSERT(map_);
        return *map_;
    }

    //////////////////////////////////////////////////////////////////////
    void Military_mgr::go_battle(uptr<Legion> legion)
    {
        BOOST_ASSERT(legion && legion->is_alive());
        BOOST_ASSERT(legion->from());
        BOOST_ASSERT(legion->to());

        legions_.insert({ { legion->from(), legion->to() }, legion.get() });
        legion_cache_.push_back(std::move(legion));
    }

    bool Military_mgr::has_troop(Architecture* from, Architecture* to) const
    {
        auto iter = legions_.find({ from, to });
        return iter != legions_.end();
    }

    //////////////////////////////////////////////////////////////////////
    bool Military_mgr::self_check() const { return {}; }

    TStrategy* Military_mgr::find_troop_strategy(id_t id) const
    {
        auto iter = boost::range::find_if(tst_, [id](auto& s) { return s->id() == id; });

        return iter == tst_.end() ? nullptr : iter->get();
    }

    DStrategy* Military_mgr::find_defence_strategy(id_t id) const
    {
        auto iter = boost::range::find_if(dst_, [id](auto& s) { return s->id() == id; });

        return iter == dst_.end() ? nullptr : iter->get();
    }

    AStrategy* Military_mgr::find_arch_strategy(id_t id) const
    {
        auto iter = boost::range::find_if(ast_, [id](auto& s) { return s->id() == id; });

        return iter == ast_.end() ? nullptr : iter->get();
    }

    void Military_mgr::emit_arch_intruded(Architecture* arch, Legion* l)
    {
        BOOST_LOG_NAMED_SCOPE("EmitArchIntruded");

        arch_intruded(arch, l);
    }
}
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
