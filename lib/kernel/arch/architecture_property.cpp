#include "kernel/arch/architecture.h"
#include "kernel/common/common_util.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/arch_impl.h"
#include "kernel/military/troop.h"
#include "kernel/person/person.h"

#include <numeric>

namespace yrui {
namespace arch {
    //! \name   global properties
    //! @{
    const std::string& Architecture::name() const noexcept { return impl().raw_.name; }

    Fraction* Architecture::fraction() const noexcept { return impl().fraction_; }

    Person* Architecture::mayor() const noexcept { return impl().mayor_; }

    Archtype Architecture::type() const noexcept { return impl().raw_.type; }
    //! @}

    //! \name   state queries
    //! @{
    bool Architecture::is_capital() const noexcept
    {
        BOOST_ASSERT(fraction());
        return fraction()->capital() == this;
    }

    bool Architecture::is_frontier() const noexcept { return common::map().is_frontier(this); }

    bool Architecture::capital_connected() const noexcept
    {
        BOOST_ASSERT(fraction());

        return impl().connected_to_capital_;
    }

    //! has the arch beleaguered last month?
    bool Architecture::has_beleaguered() const noexcept { return impl().beleaguered_; }

    bool Architecture::can_escauge(int ntroop) const noexcept { return escuage() >= ntroop && can_supply(ntroop); }

    bool Architecture::can_supply(int ntroops) const noexcept
    {
        return (money() >= military::required_money(ntroops)) && (food() >= military::required_food(ntroops));
    }

    bool Architecture::can_supply(const std::vector<Person*>& ps) const noexcept
    {
        auto troops = std::accumulate(ps.begin(), ps.end(), 0U, [](int acc, Person* p) { return acc + p->troop(); });

        return can_supply(troops);
    }

    //! query to the fraction
    bool Architecture::fraction_can_supply(int ntroops) const noexcept
    {
        if (is_capital()) {
            return can_supply(ntroops);
        } else {
            auto f_sup = capital_connected() && fraction()->can_supply(ntroops);

            auto a_sup = can_supply(ntroops);

            return f_sup || a_sup;
        }
    }

    //! \name   城池工作相关
    //! 这里面的工作都会被加入工作队列，留到月末完成。
    //! @{
    bool Architecture::has_done_farm() const noexcept { return impl().has_done_farm_; }

    bool Architecture::has_done_trade() const noexcept { return impl().has_done_trade_; }

    bool Architecture::has_done_steady() const noexcept { return impl().has_done_steady_; }

    bool Architecture::has_done_security() const noexcept { return impl().has_done_security_; }

    bool Architecture::has_worked(Person* p) const noexcept
    {
        BOOST_ASSERT(detail::contains(impl().all_, p));

        return impl().has_worked_.at(p);
    }
    //! @}

    //! \name   architecture info
    //! @{
    int Architecture::farming() const noexcept { return impl().raw_.farming; }

    int Architecture::max_farming() const noexcept { return impl().raw_.max_farming; }

    int Architecture::farming_growth() const noexcept { return impl().raw_.farming_growth; }

    int Architecture::trade() const noexcept { return impl().raw_.trade; }

    int Architecture::max_trade() const noexcept { return impl().raw_.max_trade; }

    int Architecture::trade_growth() const noexcept { return impl().raw_.trade_growth; }

    int Architecture::steady() const noexcept { return impl().raw_.steady; }

    int Architecture::max_steady() const noexcept { return impl().raw_.max_steady; }

    int Architecture::security() const noexcept { return impl().raw_.security; }

    int Architecture::security_growth() const noexcept { return impl().raw_.security_growth; }

    int Architecture::population() const noexcept { return impl().raw_.population; }

    int Architecture::max_population() const noexcept { return impl().raw_.max_population; }

    int Architecture::population_growth() const noexcept { return impl().raw_.population_growth; }

    int Architecture::corruption() const noexcept { return impl().raw_.corruption; }

    int Architecture::efficiency() const noexcept { return impl().raw_.efficiency; }

    int Architecture::aristocrat() const noexcept { return impl().raw_.aristocrat; }

    int Architecture::money() const noexcept { return impl().raw_.money; }

    int Architecture::food() const noexcept { return impl().raw_.food; }
    //! @}

    //! military properties
    //! @{
    int Architecture::escuage() const noexcept { return impl().escuage_; }

    int Architecture::troop() const noexcept { return impl().troop_num_; }
    //! @}

    //! person related
    //! @{
    const std::set<Person*>& Architecture::all() const noexcept { return impl().all_; }

    const std::set<Person*>& Architecture::in_city() const noexcept { return impl().in_city_; }

    const std::set<Person*>& Architecture::out_of_city() const noexcept { return impl().out_city_; }

    const std::set<uptr<job::Job>>& Architecture::jobs() const noexcept { return impl().jobs_; }
    //! @}
}
} // of namespace yrui::arch
