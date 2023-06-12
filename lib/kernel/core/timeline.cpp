#include "kernel/core/timeline.h"

namespace yrui {
namespace {
    inline bool is_new_year(int month) { return month == 1; }

    inline bool is_end_year(int month) { return month == 12; }

    inline bool is_new_season(int month) { return month == 1 || month == 4 || month == 7 || month == 10; }

    inline bool is_end_season(int month) { return month == 3 || month == 6 || month == 9 || month == 12; }
}

Timeline::Timeline(int year, int month)
    : year_(year)
    , month_(month)
{
}

void Timeline::update()
{
    BOOST_ASSERT_MSG(1 <= month_ && month_ <= 12, "bad month");

    if (is_new_year(month_)) {
        BOOST_LOG_NAMED_SCOPE("YearBegin");
        year_begin(*this);
    }
    if (is_new_season(month_)) {
        BOOST_LOG_NAMED_SCOPE("SeasonBegin");
        season_begin(*this);
    }

    {
        BOOST_LOG_NAMED_SCOPE("MonthBegin");
        month_begin(*this);
    }

    {
        BOOST_LOG_NAMED_SCOPE("MonthEnd");
        month_end(*this);
    }

    if (is_end_season(month_)) {
        BOOST_LOG_NAMED_SCOPE("SeasonEnd");
        season_end(*this);
    }

    if (is_end_year(month_)) {
        BOOST_LOG_NAMED_SCOPE("YearEnd");
        year_end(*this);
    }

    //! update date time
    month_pass_();
}

bool Timeline::init_impl() { return true; }

void Timeline::finalize_impl() { }

void Timeline::month_pass_() { month_ == 12 ? (++year_, month_ = 1) : ++month_; }
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
