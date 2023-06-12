#include "kernel/arch/architecture.h"
#include "kernel/impl/arch_impl.h"

namespace yrui {
namespace arch {
    //! \name   architecture info
    //! 考虑到这些值都有增有降，因此使用这种低级接口
    //! @{
    void Architecture::set_farming(int n)
    {
        impl().raw_.farming = std::min(n, max_farming());

        BOOST_ASSERT(farming() <= max_farming());
    }

    void Architecture::set_max_farming(int n)
    {
        impl().raw_.max_farming = n;
        impl().raw_.farming = std::min(farming(), max_farming());

        BOOST_ASSERT(farming() <= max_farming());
    }

    void Architecture::set_farming_growth(int n) { impl().raw_.farming_growth = n; }

    void Architecture::set_trade(int n)
    {
        impl().raw_.trade = std::min(n, max_trade());

        BOOST_ASSERT(trade() <= max_trade());
    }

    void Architecture::set_max_trade(int n)
    {
        impl().raw_.max_trade = n;
        impl().raw_.trade = std::min(trade(), max_trade());

        BOOST_ASSERT(trade() <= max_trade());
    }

    void Architecture::set_trade_growth(int n) { impl().raw_.trade_growth = n; }

    void Architecture::set_steady(int n)
    {
        impl().raw_.steady = std::min(n, max_steady());

        BOOST_ASSERT(steady() <= max_steady());
    }

    void Architecture::set_max_steady(int n)
    {
        impl().raw_.max_steady = n;
        impl().raw_.steady = std::min(steady(), max_steady());

        BOOST_ASSERT(steady() <= max_steady());
    }

    void Architecture::set_security(int n)
    {
        impl().raw_.security = std::min(n, max_security);

        BOOST_ASSERT(security() <= max_security);
    }

    void Architecture::set_security_growth(int n) { impl().raw_.security_growth = n; }

    void Architecture::set_population(int n)
    {
        impl().raw_.population = std::min(n, max_population());

        BOOST_ASSERT(population() <= max_population());
    }

    void Architecture::set_max_population(int n)
    {
        impl().raw_.max_population = n;
        impl().raw_.population = std::min(population(), max_population());

        BOOST_ASSERT(population() <= max_population());
    }

    void Architecture::set_population_growth(int n) { impl().raw_.population_growth = n; }

    /*! \note this can be negative to denote debt.
     */
    void Architecture::set_money(int n) { impl().raw_.money = n; }

    /*! \note this can be negative to denote debt.
     */
    void Architecture::set_food(int n) { impl().raw_.food = n; }
    //! @}

    //! \name
    //! @{
    //! 推荐使用高级接口
    void Architecture::consume_money(int s) { impl().raw_.money = std::max<int>(money() - s, 0); }

    void Architecture::consume_food(int s) { impl().raw_.food = std::max<int>(food() - s, 0); }

    void Architecture::consume_escuage(int s)
    {
        BOOST_ASSERT(can_escauge(s));

        impl().escuage_ -= s;

        supply(s);
    }
    //! @}
}
} // of namespace yrui::arch
