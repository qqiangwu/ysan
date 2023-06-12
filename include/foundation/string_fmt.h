#ifndef YRUI_DETAIL_STRING_FMT_H_
#define YRUI_DETAIL_STRING_FMT_H_

#include <boost/format.hpp>

namespace yrui {
namespace detail {
    //! base condition
    inline std::string strfmt_imp(boost::format&& fmt) { return fmt.str(); }

    //! iteration condition
    template <class T, class... Args> inline std::string strfmt_imp(boost::format&& fmt, T&& arg, Args&&... rest)
    {
        return strfmt_imp(std::move(fmt % std::forward<T>(arg)), std::forward<Args>(rest)...);
    }
}

using boost::format;

template <class... Args> inline std::string fmt(const std::string& fmt, Args&&... args)
{
    return detail::strfmt_imp(boost::format(fmt), std::forward<Args>(args)...);
}
} // of namespace yrui

#endif // YRUI_DETAIL_STRING_FMT_H_
