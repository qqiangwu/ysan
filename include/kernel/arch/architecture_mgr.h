#ifndef YRUI_ARCHITECTURE_MGR_H_
#define YRUI_ARCHITECTURE_MGR_H_

/*!
 *
 * \file     Architecture/architecure_mgr.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    architecture manager.
 * \since
 *
 */

#include <unordered_map>

#include "kernel/component_fwd.h"
#include "kernel/core/subsys.h"
#include "kernel/detail/range_view.h"

namespace yrui {
namespace arch {
    class Architecture_mgr : public Isubsys {
        //! \name   signals
        //! @{
    public:
        signal<void(Architecture*, Fraction*, Fraction*)> ownership_transfer;

        void emit_ownership_transfer(Architecture* arch, Fraction* from, Fraction* to);
        //! @}

        //! \name   main timeline loop
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
        Architecture* get(id_t id) const;

        void load(std::vector<uptr<Architecture>> archs);

    public:
        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

        void install_signals_();

    public:
        auto as_range() const { return make_range(all_); }

    public:
        Architecture_mgr() = default;

    private:
        std::unordered_map<id_t, Architecture*> archs_;
        std::vector<uptr<Architecture>> arch_vec_;
        std::vector<Architecture*> all_;

        DECLARE_MEMBER_LOGGER("ArchMgr");
    };
} // of namespace arch
} // of namespace yrui

#endif // !YRUI_ARCHITECTURE_MGR_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
