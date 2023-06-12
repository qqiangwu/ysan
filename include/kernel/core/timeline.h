#ifndef YRUI_COMMON_TIMELINE_H_
#define YRUI_COMMON_TIMELINE_H_

/*!
 *
 * \file     Common/timeline.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    timeline object controlling the date of Bygone.
 */
#include "kernel/core/subsys.h"

namespace yrui {
//! The main logic driver. All the logic events occur in *_begin/end.
class Timeline : public Isubsys {
public:
    typedef Timeline self_type;

public:
    int year() const;
    int month() const;

    Timeline(int year, int month);

public:
    //! not used now. Three turns comprise a month
    //! @{
    yrui::signal<void(self_type&)> turn_begin;
    yrui::signal<void(self_type&)> turn_end;
    //! @}

    yrui::signal<void(self_type&)> month_begin;
    yrui::signal<void(self_type&)> month_end;
    yrui::signal<void(self_type&)> season_begin;
    yrui::signal<void(self_type&)> season_end;
    yrui::signal<void(self_type&)> year_begin;
    yrui::signal<void(self_type&)> year_end;

public:
    virtual void update() override;

private:
    virtual bool init_impl() override;
    virtual void finalize_impl() override;

private:
    void month_pass_();

private:
    int year_;
    int month_;

    DECLARE_MEMBER_LOGGER("Timeline");
};
} // of namespace yrui

//! implementation
namespace yrui {
inline int Timeline::year() const
{
    BOOST_ASSERT_MSG(1 <= month_ && month_ <= 12, "bad month");
    return year_;
}

inline int Timeline::month() const
{
    BOOST_ASSERT_MSG(1 <= month_ && month_ <= 12, "bad month");
    return month_;
}
} // of namespace yrui

#endif // !YRUI_COMMON_TIMELINE_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
