#ifndef YRUI_CORE_MILITARY_LEGION_H_
#define YRUI_CORE_MILITARY_LEGION_H_

#include <vector>

#include "kernel/component_fwd.h"
#include "kernel/config/characteristic_config.h"

namespace yrui {
namespace military {
    class Legion : public std::enable_shared_from_this<Legion>, private boost::noncopyable {
    public:
        enum { troop_num = troop_num_limit };

        typedef std::vector<Person*> Troop_container;

        using iterator = Troop_container::iterator;
        using const_iterator = Troop_container::const_iterator;

    public:
        /* 粮食会在创建部队时由创建者扣除
         *  人物会由创建者移出城市，由Legion负责移回城市
         */
        Legion(const Troop_container& troops, Architecture* from, Architecture* target);

        ~Legion();

    public:
        const std::string& name() const;

        Person* leader() const;

        Architecture* from() { return from_; }
        Architecture* to() { return to_; }

        Fraction* fraction() const;

        bool is_alive() const;

        //! 返回一支在最前的军队
        Person* frontier() const;
        std::pair<Person*, Person*> strategy_troops() const;

        int troop() const;

        //! 返回多支在最前的军队
        Troop_container frontiers(int size) const;
        Troop_container all() const;
        const Troop_container& all_persons() const noexcept;

        Troop_container::size_type num_available() const;

    public:
        //! \name   troop commands gived by controller
        //! @{
        Fraction_controller* controller() const;

        void prepare_attack();
        void prepare_defense();
        void retreat();
        void reside_or_return();

        void take_control_of(Intruded_arch* arch);
        void deal_captive(Person* p);
        //! @}

    public:
        //! \name   combat procedure
        //! @{
        void on_battle_start(Military_mgr& mgr);
        void on_battle_end(Military_mgr& mgr);

        void on_day_begin();
        void on_day_end();

        //! 出战军队
        bool is_waging() const;

        void attack(Military_mgr& mgr, Legion* enemy);
        void attack(Military_mgr& mgr, Intruded_arch* enemy);
        //! @}

    public:
        Military_mgr& mgr() const noexcept;
        ylogger::logger_type& logger() const noexcept;

    private:
        Troop_container troops_;
        Architecture* from_;
        Architecture* to_;
        Fraction* fraction_;

    private:
        enum class State { attack, defence };

        State state_ = State::attack;

        Person* leader_;
        Person* strategy_1_;
        Person* strategy_2_;

        Military_mgr* mgr_ = nullptr;
        ylogger::logger_type* logger_ = nullptr;
    };
} // of namespace military
} // of namespace yrui

#endif // YRUI_CORE_MILITARY_LEGION_H_
