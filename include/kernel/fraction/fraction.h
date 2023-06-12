#ifndef YRUI_FRACTION_H_
#define YRUI_FRACTION_H_

/*!
 *
 * \file     Fraction/fraction.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    fraction object.
 * \ingroup  Fraction
 *
 */

#include <set>

#include "kernel/detail/game_obj_common.h"
#include "kernel/detail/range_view.h"

namespace yrui {
namespace detail {
    struct Raw_fraction;
} // of namespace detail

namespace fraction {
    class Fraction : public yrui::detail::Game_obj_common<Fraction, Fraction_mgr> {
    public:
        friend class Fraction_mgr;

        //! \name   structors
        //! @{
    public:
        //! \note   Simply load data, and the invariant is not
        //!         established untill init_self() is called!
        explicit Fraction(const yrui::detail::Raw_fraction& raw) noexcept;

        virtual ~Fraction();
        //! @}

        //! \name   observers
        //! @{
    public:
        const std::string& name() const noexcept;

        Person* leader() const noexcept;
        Person* heir() const noexcept;
        Architecture* capital() const noexcept;

        bool has_seal() const noexcept;
        bool has_emperor() const noexcept;

        int politics() const noexcept;
        int action_power() const noexcept;

        bool is_alive() const noexcept;

        int arch_count() const noexcept;
        int person_count() const noexcept;

        auto heir_candidate() const noexcept;

        //! \warning
        //! These two are initialized on constructor.
        //! They rely on that Person_mgr and Architecture_mgr must be initialized
        //! before Fraction_mgr.
        //! Can I use another way which does not impose the initialization
        //! dependencies?
        //! Or should I use two phrase initialization?
        //! @{
        const std::set<Architecture*>& archs() const noexcept;
        const std::set<Person*>& persons() const noexcept;
        //! @}

        Fraction_controller* controller() const noexcept;

        int food() const noexcept;
        int money() const noexcept;

        bool can_supply(const std::vector<Person*>& p) const noexcept;
        bool can_supply(int num_of_troops) const noexcept;

        int troop() const noexcept;
        int max_supply() const noexcept;
        //! @}

        //! \name   modifiers
        //! @{
    public:
        void set_heir(Person* p);
        void set_leader(Person* person);
        void set_capital(Architecture* arch);

        void get_seal();
        void get_emperor();
        void lose_seal();
        void lose_emperor();

        void set_controller(uptr<Fraction_controller> ctrl);
        //! @}

        //! \name   fraction actions
        //! @{
    public:
        //! do decision here
        void think(const Timeline& tl);

        void contribute_food(int food);
        void contribute_money(int money);

        void die();

        void supply_troop(int num_of_troops);
        //! @}

        //! \name   slots
        //! They are not public accessed for the reason that fractions' internal
        //! state should not be managed by others.
        //! @{
    private:
        void on_obtain_person(Person* p);
        void on_lose_person(Person* p);

        void on_obtain_arch(Architecture* arch);
        void on_lose_arch(Architecture* arch);
        //! @}

        //! \name   fraction operations
        //! @{
    public:
        //! \name   diplomatic operations
        //! @{
        void cease_war(Person* p, Fraction* other);
        void ally(Person* p, Fraction* other);
        void cease_alliance(Person* p, Fraction* other);
        void develop_relation(Person* p, Fraction* other);
        void cease_war_by_emperor(Person* p, Fraction* other);
        void ally_by_emperor(Person* p, Fraction* other);
        void intrade_by_emperor(Person* p, Fraction* src, Fraction* dest);
        //! @}

        int conscript();

        /*!
         * intrade a architecture.
         * \pre persons' distances from \target is below a specific value.
         */
        Legion* intrade(const std::vector<Person*>& p, Architecture* targart);
        //! @}

    private:
        //! Fully setup invariants!
        void init_self();
    };
} // of namespace fraction
} // of namespace yrui

//! implementation
namespace yrui {
namespace fraction {
    namespace detail {
        bool is_heir(Person* p) noexcept;
    }

    inline auto Fraction::heir_candidate() const noexcept { return make_range(persons() | filtered(detail::is_heir)); }

    inline int Fraction::arch_count() const noexcept { return archs().size(); }

    inline int Fraction::person_count() const noexcept { return persons().size(); }
} // of namespace fraction
} // of namespace yrui
#endif // !YRUI_FRACTION_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
