#ifndef YRUI_CORE_MILITARY_STRATEGY_H_
#define YRUI_CORE_MILITARY_STRATEGY_H_

#include "kernel/component_fwd.h"
#include <functional>

namespace yrui {
namespace military {
    // 根据对象不同，计谋分三种：对部队计谋、对城池计谋、城池对部队计谋
    //! provide some common functionality
    class Strategy {
    protected:
        typedef Strategy base_type;

        Strategy(id_t id, const std::string& name, int power, int poss)
            : id_(id)
            , name_(name)
            , power_(power)
            , poss_(poss)
        {
        }

        virtual ~Strategy() = default;

    public:
        id_t id() const { return id_; }
        const std::string& name() const { return name_; }

    public:
        int power() const { return power_; }
        int probability() const { return poss_; }

    private:
        id_t id_;
        std::string name_; //! \fixme  先使用硬编码
        int power_;
        int poss_;
    };

    namespace detail {
        bool defensible_impl(Person* attacker, Person* defensor);
    }

    template <class Target> class Basic_strategy : public Strategy {
    public:
        using base_type::Strategy;

        typedef Person* initiator;
        typedef Target target;

    public:
        bool condition_suitable(target t) const
        {
            BOOST_ASSERT(t);
            return check_condition_impl_(t);
        }

        bool defensible(initiator attacker, initiator defensor) const
        {
            return detail::defensible_impl(attacker, defensor);
        }

        void occur(Military_mgr& mgr, initiator attacker, target t) const
        {
            BOOST_ASSERT(attacker);
            BOOST_ASSERT(t);
            return occur_impl_(mgr, attacker, t);
        }

    private:
        virtual bool check_condition_impl_(target t) const = 0;
        virtual void occur_impl_(Military_mgr& mgr, initiator attacker, target t) const = 0;
    };

    using TStrategy = Basic_strategy<Legion*>;
    using AStrategy = Basic_strategy<Intruded_arch*>;
    using DStrategy = Basic_strategy<Legion*>;

#define DEFINE_STRATEGY_COMMON(base, strategy_cls, id, name, power, poss)                                              \
    namespace strategy {                                                                                               \
        class strategy_cls : public base {                                                                             \
        public:                                                                                                        \
            strategy_cls()                                                                                             \
                : base(id, name, power, poss)                                                                          \
            {                                                                                                          \
            }                                                                                                          \
                                                                                                                       \
        private:                                                                                                       \
            virtual bool check_condition_impl_(target t) const override;                                               \
            virtual void occur_impl_(Military_mgr& mgr, initiator attacker, target t) const override;                  \
        };                                                                                                             \
    }
#define DEFINE_TSTRATEGY(strategy, id, name, power, poss)                                                              \
    DEFINE_STRATEGY_COMMON(TStrategy, strategy, id, name, power, poss)
#define DEFINE_ASTRATEGY(strategy, id, name, power, poss)                                                              \
    DEFINE_STRATEGY_COMMON(AStrategy, strategy, id, name, power, poss)
#define DEFINE_DSTRATEGY(strategy, id, name, power, poss)                                                              \
    DEFINE_STRATEGY_COMMON(DStrategy, strategy, id, name, power, poss)

    DEFINE_TSTRATEGY(Disturb, 1, "扰乱", 0, 30)
    DEFINE_TSTRATEGY(Raid, 2, "突袭", 20, 15)
    DEFINE_TSTRATEGY(Ambush, 3, "伏击", 20, 10)
    DEFINE_TSTRATEGY(Fire, 4, "火计", 40, 5)
    DEFINE_TSTRATEGY(Strike, 5, "奇袭", 20, 5)

    DEFINE_ASTRATEGY(Burn, 1, "火烧", 0, 10)
    DEFINE_ASTRATEGY(Wreck, 2, "坑道", 0, 10)
    DEFINE_ASTRATEGY(Traitor, 3, "内奸", 0, 5)

    DEFINE_DSTRATEGY(Bluff, 1, "空城", 0, 3)
    DEFINE_DSTRATEGY(Sneak, 2, "偷袭", 20, 10)

#undef DEFINE_DSTRATEGY
#undef DEFINE_ASTRATEGY
#undef DEFINE_TSTRATEGY
#undef DEFINE_STRATEGY_COMMON
}
}

#endif // YRUI_CORE_MILITARY_STRATEGY_H_
