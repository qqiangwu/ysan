#ifndef YRUI_DIPLOMACY_MGR_H_
#define YRUI_DIPLOMACY_MGR_H_

#include "kernel/component_fwd.h"
#include "kernel/core/subsys.h"
#include "kernel/diplomacy/fraction_relation.h"

namespace yrui {
namespace diplomacy {
    class Diplomacy_mgr : public Isubsys {
    public:
        //! in loading, the whole system is not created yet
        void load(Relation_table rel);

        Relation get_relation(id_t x, id_t y) const;
        Relation set_relation(id_t x, id_t y, Relation rel);

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

    public:
        Diplomacy_mgr() = default;

    public:
        virtual void update() override;
        virtual bool self_check() const override;

    private:
        virtual bool init_impl() override;
        virtual void finalize_impl() override;

    private:
        Relation_table table_;

        DECLARE_MEMBER_LOGGER("DiplomacyMgr");
    };
} // of namespace diplomacy
} // of namespace yrui

#endif // !YRUI_DIPLOMACY_MGR_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
