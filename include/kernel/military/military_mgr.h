#ifndef YRUI_CORE_MILITARY_MGR_H_
#define YRUI_CORE_MILITARY_MGR_H_

#include <map>
#include <vector>

#include "kernel/component_fwd.h"
#include "kernel/core/subsys.h"
#include "kernel/detail/pimpl.h"
#include "kernel/detail/range_view.h"
#include "kernel/military/legion.h"
#include "kernel/military/strategy.h"
#include "kernel/resource/map.h"

namespace yrui {
namespace military {
    class Military_mgr : public Isubsys {
        //! \name   main timeline loop
        //! slot
        //  these functions are called in the main timeline loop.
        //! @{
    public:
        void on_month_begin(Timeline& t);
        void on_month_end(Timeline& t);
        //! @}

        //! \name   signals
        //! @{
    public:
        //! 野战开始与结束
        signal<void(Legion*, Legion*)> battle_start;
        signal<void(Legion*, Legion*)> battle_end;

        signal<void(Architecture*, Legion*)> arch_intruded;

        //! 攻城战开始与结束
        signal<void(Legion*, Intruded_arch*)> siege_start;
        signal<void(Legion*, Intruded_arch*)> siege_end;

        //! 互斥，且不一定有siege_start或者siege_end触发，因此直接攻陷
        //! @{
        signal<void(Legion*, Intruded_arch*)> siege_success;
        signal<void(Legion*, Intruded_arch*)> siege_failed;
        //! @}

        //! 肉搏战
        signal<void(Person*, Person*)> combat;
        signal<void(Person*, Person*)> defeat;

        //! 围城
        signal<void(Legion*, Intruded_arch*)> siege_wait;
        //! 武将攻城
        signal<void(Person*, Intruded_arch*)> siege_attack;
        //! 武将攻陷城池
        signal<void(Person*, Intruded_arch*)> siege_attack_success;
        //! 武将败退
        signal<void(Person*, Intruded_arch*)> siege_attack_failed;

        //! 成为计谋部队
        signal<void(Person*)> become_strategy_troop;
        //! 思考计谋
        signal<void(Person*)> strategy_prepare;
        //! 用计
        signal<void(Person*, Strategy*)> strategy_attack;
        //! 用计被识破
        signal<void(Person*, Person*)> strategy_detected;
        //! 用计击破敌军
        signal<void(Person*, Person*)> strategy_conquer;

        void emit_arch_intruded(Architecture* arch, Legion* l);
        //! @}

    public:
        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    public:
        void load(resource::Map::graph_type&& map);

    public:
        //! map related
        //! @{
        resource::Map& map();
        //! @}

    public:
        void go_battle(uptr<Legion> legion);
        bool has_troop(Architecture* from, Architecture* to) const;

    public:
        //! strategy related
        //! @{
        using TStrategy_container = std::vector<uptr<TStrategy>>;
        using AStrategy_container = std::vector<uptr<AStrategy>>;
        using DStrategy_container = std::vector<uptr<DStrategy>>;

        TStrategy* find_troop_strategy(id_t id) const;
        DStrategy* find_defence_strategy(id_t id) const;
        AStrategy* find_arch_strategy(id_t id) const;

        auto troop_strategies() const { return make_range(tst_); }

        auto arch_strategies() const { return make_range(ast_); }

        auto defence_strategies() const { return make_range(dst_); }
        //! @}

    public:
        auto as_range() const { return make_range(legions_); }

    public:
        Military_mgr() = default;

    private:
        void schedule_battle_();
        bool start_combat_(Legion* attacker, Legion* defensor);
        bool start_seige_(Legion* attacker, Intruded_arch* defensor);

    private:
        using container_type = std::map<std::pair<Architecture*, Architecture*>, Legion*>;

        uptr<resource::Map> map_;

        container_type legions_;
        std::vector<uptr<Legion>> legion_cache_;

        TStrategy_container tst_;
        AStrategy_container ast_;
        DStrategy_container dst_;

        DECLARE_MEMBER_LOGGER("MilitaryMgr");
    };
} // of namespace military
} // of namespace yrui

#endif // YRUI_CORE_MILITARY_MGR_H_
