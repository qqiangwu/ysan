#ifndef YRUI_FRACTION_MGR_H_
#define YRUI_FRACTION_MGR_H_

/*!
 *
 * \file     Fraction/fraction_mgr.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    fraction manager.
 * \ingroup  Fraction
 *
 */

#include <boost/range/adaptor/map.hpp>

#include <queue>
#include <unordered_map>
#include <vector>

#include "kernel/component_fwd.h"
#include "kernel/core/subsys.h"
#include "kernel/detail/range_view.h"

namespace yrui {
namespace fraction {
    class Fraction_mgr : public Isubsys {
        //! slot
        //  these functions are called in the main timeline loop.
        //! @{
    public:
        void on_month_begin(Timeline& t);
        void on_month_end(Timeline& t);
        void on_season_begin(Timeline& t);
        void on_season_end(Timeline& t);
        void on_year_begin(Timeline& t);
        void on_year_end(Timeline& t);
        //! @}

    public:
        //! \todo   add fraction_established;
        signal<void(Fraction*)> fraction_died;
        signal<void(Fraction*, Architecture*)> arch_obtained;
        signal<void(Fraction*, Architecture*)> arch_lost;
        signal<void(Fraction*, Person*)> person_obtained;
        signal<void(Fraction*, Person*)> person_lost;
        signal<void(Fraction*, Person*)> succession;
        signal<void(Fraction*)> nation_unified;

        void emit_fraction_died(Fraction*);
        void emit_succession(Fraction*, Person*);
        void emit_arch_obtained(Fraction*, Architecture*);
        void emit_arch_lost(Fraction*, Architecture*);
        void emit_person_obtained(Fraction*, Person*);
        void emit_person_lost(Fraction*, Person*);
        void emit_nation_unified(Fraction*);

    private:
        void install_signals_();

    public:
        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    public:
        Fraction* get(id_t id) const;

        void load(std::vector<uptr<Fraction>> frac);

        int size() const;

    public:
        auto as_range() const { return make_range(fractions_ | boost::adaptors::map_values); }

    public:
        Fraction_mgr() = default;

    private:
        std::unordered_map<id_t, Fraction*> fractions_;
        std::vector<uptr<Fraction>> frac_vec_;
        std::queue<Fraction*> died_;

        DECLARE_MEMBER_LOGGER("FractionMgr");
    };
} // of namespace fraction
} // of namespace yrui

namespace yrui {
namespace fraction {
    inline int Fraction_mgr::size() const { return fractions_.size(); }
} // of namespace fraction
} // of namespace yrui

#endif // !YRUI_FRACTION_MGR_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
