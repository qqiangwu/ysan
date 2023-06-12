#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/fraction/fraction.h"
#include "kernel/impl/arch_impl.h"
#include "kernel/military/legion.h"
#include "kernel/military/troop.h"
#include "kernel/person/person.h"

namespace yrui {
namespace arch {
    uptr<Legion> Architecture::create_troop(Architecture* target, const std::vector<Person*>& troops)
    {
        // BOOST_ASSERT(target); // may be defend
        BOOST_ASSERT(!troops.empty());
        BOOST_ASSERT(troops.size() <= military::Legion::troop_num);
        BOOST_LOG_NAMED_SCOPE("ArchCreateTroop");

        using namespace military;

        Legion::Troop_container cont;

        //! let all persons go out
        for (auto p : troops) {
            BOOST_ASSERT(p->stayed_city() == this);
            BOOST_ASSERT(!p->is_troop_crashed());
            BOOST_ASSERT(!has_worked(p));

            on_person_go_out_(p);

            cont.push_back(p);
        }

        auto legion = make_uptr<Legion>(std::ref(cont), this, target);

        BOOST_ASSERT(fraction_can_supply(legion->troop()));

        ask_for_supply(legion->troop());

        return legion;
    }

    void Architecture::supply(int ntroops)
    {
        BOOST_ASSERT(can_supply(ntroops));

        consume_food(military::required_food(ntroops));
        consume_money(military::required_money(ntroops));
    }

    bool Architecture::ask_for_supply(int num_of_troops)
    {
        if (!is_capital() && capital_connected()) {
            auto frac = fraction();
            if (frac->can_supply(num_of_troops)) {
                frac->supply_troop(num_of_troops);
                return true;
            }
        }

        if (can_supply(num_of_troops)) {
            supply(num_of_troops);
            return true;
        }

        return false;
    }

    void Architecture::dismiss_troop(Person* p)
    {
        BOOST_ASSERT(p->has_troop());

        MLOG() << format("%1% @ %2% dev dismiss") % p->name() % name();

        auto troop = p->troop();

        p->dismiss_troop();
        impl().raw_.population += troop;
        impl().calculate_troop();
    }

    void Architecture::set_mayor(Person* person)
    {
        BOOST_LOG_NAMED_SCOPE("SetMayor");

        //! 清除旧的太守
        if (mayor()) {
            // 假设无效果
        }

        //! 设置新的太守
        if (person) {
            BOOST_ASSERT(person->fraction() == fraction());
            BOOST_ASSERT(person->stayed_city() == this);

            impl().mayor_ = person;

            // person->loyalty(person->max_loyalty() / 3);
            //! \todo   emit set mayor signal

            MLOG() << format("%2% @ %1% be mayor [Loyal: %3%]") % name() % person->name() % person->loyalty();
        } else {
            MLOG() << format("%1% remove mayor") % name();

            impl().mayor_ = nullptr;
        }
    }

    void Architecture::harvest()
    {
        BOOST_ASSERT_MSG(false, "I don't use harvest now");

        auto del = population() * farming() / 10;

        if (fraction() && capital_connected()) {
            int food_cont = del * 0.6;
            int money_cont = food() * 0.6;

            fraction()->contribute_food(food_cont);
            fraction()->contribute_money(money_cont);

            del -= food_cont;

            consume_money(money_cont);
        }

        set_food(food() + del);

        MLOG() << format("%1% harvest %2%") % name() % del;
    }

    void Architecture::beleaguered() { abort_jobs(); }

    void Architecture::abort_jobs() { impl().abort_jobs(); }

    void Architecture::transfer_to(Fraction* f)
    {
        BOOST_LOG_NAMED_SCOPE("TransferTo");
        YPRE_CONDITION(fraction() != f);

        MLOG() << format("%1% transfer from %2% to %3%") % name() % (fraction() ? fraction()->name() : "NULL")
                % (f ? f->name() : "NULL");

        auto old = fraction();

        //! \todo   cluster these logic into a class
        BOOST_ASSERT(in_city().empty());

        impl().fraction_ = f;
        set_mayor(nullptr);

        mgr().emit_ownership_transfer(this, old, f);

        impl().clear_state();

        BOOST_ASSERT(!troop());
        BOOST_ASSERT(!mayor());
        BOOST_ASSERT(all().empty());
        BOOST_ASSERT(in_city().empty());
        BOOST_ASSERT(out_of_city().empty());
        BOOST_ASSERT(impl().has_worked_.empty());
        BOOST_ASSERT(impl().jobs_.empty());
    }

    void Architecture::add_job(uptr<job::Job> j)
    {
        BOOST_ASSERT(detail::contains(all(), j->person()));
        BOOST_ASSERT(!has_worked(j->person()));

        auto p = j->person();

        impl().jobs_.insert(std::move(j));
        impl().has_worked_[p] = true;

        BOOST_ASSERT(has_worked(p));
    }
}
} // of namespace yrui::arch
