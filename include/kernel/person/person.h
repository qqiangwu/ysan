#ifndef YRUI_PERSON_H_
#define YRUI_PERSON_H_

/*!
 *
 * \file     Person/person.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    person objects.
 * \ingroup  Person
 *
 */

#include "kernel/detail/characteristic_type.h"
#include "kernel/detail/game_obj_common.h"
#include "kernel/person/position.h"

namespace yrui {
namespace detail {
    struct Raw_person;
} // of namespace detail

namespace person {
    /*!
     * \brief  the person objects.
     *
     * 维护人物的各项属性以及各种能力。
     * 提供军队支持
     *
     * 所有属性尽量使用有意义的函数进行操作，如: be_attacked, gain_troop。
     * 对于其他可能升降的属性来说，提供通用接口，但通用接口无前置条件。
     *
     * Person维护了所有自己的状态信息，如，所在城池，这些信息的改变，都需要Person
     * 的动作来完成，而不是由其他对象来设置Person的字段。
     */
    class Person : public detail::Game_obj_common<Person, Person_mgr> {
    public:
        friend class military::Military_support;
        friend class Person_mgr;

        //! \name   structors
        //! @{
    public:
        /*! person object is default constructed, all its info is read by the
         *  database loader.
         *
         *  simply bit blit operation
         */
        explicit Person(const detail::Raw_person& raw) noexcept;

        virtual ~Person();
        //! @}

        //! interface
        //! @{
    public:
        virtual void on_month_begin(const Timeline& tl) override;
        virtual void on_month_end(const Timeline& tl) override;
        //! @}

        //! \name   observers
        //! @{
    public:
        const std::string& name() const noexcept;

        int born_year() const noexcept;
        int showed_year() const noexcept;
        int lifespan() const noexcept;
        int age() const noexcept;
        int feat() const noexcept;

        Position position() const noexcept;

        Architecture* stayed_city() const noexcept;
        Architecture* showed_city() const noexcept;
        Person* relative() const noexcept;
        Fraction* fraction() const noexcept;

        //! \name   state queries
        //! @{
        bool is_leader() const noexcept;
        bool is_in_office() const noexcept;
        bool is_alive() const noexcept;
        bool is_active() const noexcept;
        bool is_supplied() const noexcept;
        //! @}

        //! \name   person abilities(currently)
        //! @{
        Basic_ability fight() const noexcept;
        Basic_ability leadership() const noexcept;
        Basic_ability wisdom() const noexcept;
        Basic_ability politics() const noexcept;
        int potential() const noexcept;
        //! @}

        //! \name   exp
        //! @{
        int fight_exp() const noexcept;
        int leadership_exp() const noexcept;
        int wisdom_exp() const noexcept;
        int politics_exp() const noexcept;
        //! @}

        //! \name   loyalty
        //! @{
        int loyalty() const noexcept;
        int max_loyalty() const noexcept;
        //! @}

        //! \name   troop related
        //! @{
        int controlled_troop() const noexcept;

        bool has_troop() const noexcept;
        bool is_troop_combatable() const noexcept;
        bool is_troop_crashed() const noexcept;

        int troop() const noexcept;
        int max_troop() const noexcept;
        int morale() const noexcept;
        int drill() const noexcept;
        //! @}

        //! \name   Affairs and Military Ability
        //! @{
        Act_ability farm_ability() const noexcept;
        Act_ability trade_ability() const noexcept;
        Act_ability security_ability() const noexcept;
        Act_ability steady_ability() const noexcept;
        Act_ability train_ability() const noexcept;

        Act_ability attack() const noexcept;
        Act_ability defence() const noexcept;

        Act_ability strategy_rate() const noexcept;
        Act_ability strategy_success_rate() const noexcept;
        Act_ability strategy_detect_rate() const noexcept;
        Act_ability strategy_attack() const noexcept;
        Act_ability strategy_defence() const noexcept;
        //! @}
        //! @}

        //! \name   public modifiers
        //! @{
    public:
        //! \name   exp
        //! gain exp and convert potential power to main ability
        //! @{
        void gain_fight_exp(int n) noexcept;
        void gain_leadership_exp(int n) noexcept;
        void gain_wisdom_exp(int n) noexcept;
        void gain_politics_exp(int n) noexcept;

        void add_feat(int n) noexcept;
        //! @}

        //! \name   military
        //! @{
        void morale_up(int n) noexcept;
        void drill_up(int n) noexcept;
        void morale_loss(int n) noexcept;
        //! @}

        //! \name   Affairs and Military Ability
        //! @{
        void set_farm_ability(Act_ability n) noexcept;
        void set_trade_ability(Act_ability n) noexcept;
        void set_security_ability(Act_ability n) noexcept;
        void set_steady_ability(Act_ability n) noexcept;
        void set_train_ability(Act_ability n) noexcept;

        void set_attack(Act_ability n) noexcept;
        void set_defence(Act_ability n) noexcept;

        void set_strategy_rate(Act_ability n) noexcept;
        void set_strategy_success_rate(Act_ability n) noexcept;
        void set_strategy_detect_rate(Act_ability n) noexcept;
        void set_strategy_attack(Act_ability n) noexcept;
        void set_strategy_defence(Act_ability n) noexcept;
        //! @}

        void set_position(Position pos) noexcept;
        //! @}

        //! \name  public actions.
        //! @{
    public:
        /*!
         * \brief dismiss_troop
         * \pre   has_troop().
         * \post  !has_troop() && morale() == 0 && drill() == 0.
         */
        void dismiss_troop();

        /*!
         * \brief gain_troop
         *
         * Establish a new troop or replenish existing troop.
         *
         * \post    has_troop().
         */
        void gain_troop(int n);

        /*!
         * \brief is_persuaded_by
         *
         * \pre   fraction() != f.
         *
         * Try persuade the person to capitulate \a f.
         */
        bool is_persuaded_by(Fraction* f) const noexcept;

        /*!
         * killed by the fraction.
         *
         * \pre     is_alive().
         * \post    !is_alive().
         *
         * \emit    person_die.
         */
        void killed_by(Fraction* f);

        /*!
         * captured by the fraction.
         *
         * \pre fraction() != f.
         * \pre !is_captured().
         * \post    is_captured().
         *
         * \emit person_captured
         */
        void captured_by(Fraction* f);

        /*!
         * capitulate to the fraction.
         *
         * \pre     is_active().
         * \pre     fraction() != f.
         * \post    fraction() == f.
         */
        void capitulate(Fraction* f);

        /*!
         * released by the fraction after captured.
         *
         * \pre     is_captured().
         * \post    !is_captured().
         */
        void release(Fraction* f);

        /*!
         * \brief reside
         * \param other
         *
         * Reside \other arch.
         *
         * \pre     is_in_office().
         * \post    stayed_city() == other.
         *
         * \emit    (optional) person_leave_arch.
         * \emit    person_reside_arch.
         */
        void reside(Architecture* other);

        /*!
         * \brief go_back
         *
         * Go back to resided arch. If the arch has been transfered to another,
         * go back to captial.
         *
         */
        void go_back();

        /*!
         * \brief join
         * \param arch
         *
         * Join the fraction the arch belongs to.
         *
         * \pre     !is_in_office()
         * \post    is_in_office()
         * \post    fraction() == arch->fraction()
         * \post    stayed_city() == arch
         *
         * \emit    person_join_fraction
         * \emit    person_reside_arch
         */
        void join(Architecture* arch);
        //! @}

        //! \name  modifiers inside the person system.
        //! 这些方法由自身或者Person_mgr调用，会改变自身状态，同时会发出信号
        //! @{
    private:
        /*!
         * \brief appear
         *
         * Put person into active state thus it will reponse the timeline signals.
         *
         * \pre     !active() && is_alive().
         * \post    active().
         *
         * \emit    person_appear
         *
         */
        void appear();

        /*!
         * \brief die
         *
         * All personal references will be cleared.
         *
         * \pre     is_active().
         * \post    !is_alive().
         *
         * \emit    person_die
         */
        void die();

        /*!
         * \brief leave_from_office
         *
         * Leave the current fraction, clear up all fraction related states.
         *
         * \pre     is_in_office()
         * \post    !is_in_office()
         *
         * \emit    person_leave_fraction.
         * \emit    (optioinal) person_leave_arch.
         */
        void leave_from_office();

        /*!
         * \brief move_to
         * \param other
         *
         * Move from the current city to another.
         *
         * \pre !is_in_office()
         * \pre stayed_city() != other.
         * \post    stayed_city() == other.
         */
        void move_to(Architecture* other);
        //! @}

        //! \name   signal handlers
        //! @{
    private:
        void on_fraction_die(Fraction* f);

        /*!
         * \brief on_become_leader
         *
         * \todo
         * \emit    become_leader
         */
        void on_become_leader(Fraction* f);
        //! @}

        //! \name   Military related
        //! \deprecated
        //! \bug
        //! \todo   remove this to a single class object.
        //! @{
    public:
        Attack_power initiate_attack() const;
        Strategy_power initiate_strategy(const Strategy* str) const;

        bool will_survive(Attack_power power) const noexcept;
        int attack_loss_hint(Attack_power power) const noexcept;

        void suffer_attack(Attack_power power);
        void suffer_strategy(Strategy_power power);
        //! @}

    private:
        void init_self();
    };
} // of namespace person
} // of namespace yrui

#endif // !YRUI_PERSON_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
