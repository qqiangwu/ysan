#ifndef YRUI_PERSON_MGR_H_
#define YRUI_PERSON_MGR_H_

/*!
 *
 * \file     Person/person_mgr.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    person_mgr which owns and manages all person objects.
 * \since
 *
 */

#include <unordered_map>

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include "kernel/core/subsys.h"
#include "kernel/detail/range_view.h"

namespace yrui {
namespace person {
    class Person_mgr : public Isubsys {
        //! \name   main timeline loop
        //! slot
        //  these functions are called in the main timeline loop.
        //! @{
    public:
        void on_month_begin(Timeline& t);
        void on_month_end(Timeline& t);
        void on_year_begin(Timeline& t);
        void on_year_end(Timeline& t);
        //! @}

        //! \name   signals
        //! @{
    public:
        signal<void(Person*)> person_die;
        signal<void(Person*)> person_appear;

        signal<void(Person*, Fraction*)> person_join_fraction;
        signal<void(Person*, Fraction*)> person_leave_fraction;

        signal<void(Person*, Architecture*)> person_reside_arch;
        signal<void(Person*, Architecture*)> person_leave_arch;

        signal<void(Person*, Fraction*)> person_captured;
        signal<void(Person*, Fraction*)> person_capitulated;
        signal<void(Person*, Fraction*)> person_killed;
        signal<void(Person*, Fraction*)> person_released;

        void emit_person_die(Person* p);
        void emit_person_appear(Person* p);

        void emit_person_be_official(Person* p, Fraction* f);
        void emit_person_leave_offical(Person* p, Fraction* f);

        void emit_person_reside_arch(Person* p, Architecture* a);
        void emit_person_leave_arch(Person* p, Architecture* a);

        void emit_person_captured(Person* p, Fraction* f);
        void emit_person_capitulated(Person* p, Fraction* f);
        void emit_person_killed(Person* p, Fraction* f);
        void emit_person_released(Person* p, Fraction* f);
        //! @}

    public:
        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    public:
        Person* get(id_t id) const;

        void load(std::vector<uptr<Person>> p);

    public:
        using category_container = std::vector<Person*>;

        auto as_range() const { return make_range(all_); }

        //! 所有人物
        const category_container& all() const { return all_; }

        const category_container& appeared() const { return appeared_; }

        const category_container& not_appeared() const { return not_appeared_; }

        const category_container& official() const { return official_; }

        const category_container& out_of_office() const { return out_of_office_; }

        //! load persons from \a db_name.
        Person_mgr() = default;

    private:
        void install_signals_();

    private:
        using container_type = std::unordered_map<id_t, Person*>;

        std::vector<uptr<Person>> person_vec_;
        container_type persons_;

        category_container all_;

        category_container appeared_;
        category_container not_appeared_;

        category_container official_;
        category_container out_of_office_;

        DECLARE_MEMBER_LOGGER("PersonMgr");
    };
} // of namespace person
} // of namespace yrui
#endif // !YRUI_PERSON_MGR_H_
