#ifndef YRUI_CONFIG_H_
#define YRUI_CONFIG_H_

/*!
 *
 * \file     config.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    global configuration of Bygone.
 * \ingroup  Core
 *
 */

#include <boost/signals2.hpp> // signal support

#include <algorithm>
#include <cstddef> // for std::size_t
#include <memory>
#include <string> // for std::string

#include "kernel/config/dll_support.h"

#include "foundation/contract.h"
#include "foundation/log_support.h"
#include "foundation/string_fmt.h"

/*! \brief  the main namespace of Bygone. */
namespace yrui {
//! \name   pointer semantic support
//! @{
template <class T> using ptr = std::shared_ptr<T>;

template <class T> using uptr = std::unique_ptr<T>;

template <class T, class... Args> inline ptr<T> make_ptr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template <class T, class... Args> inline uptr<T> make_uptr(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
//! @}

//! signals
//! @{
using boost::signals2::signal;

namespace sig_level {
    enum {
        internal, // used by items in the same subsystem
        mutual, // used by items in different subsystem
        observer, // used by the core.
        external, // used by the peripherals.
    };
} // of namespace sig_level
//! @}

//! \brief  game object id support
//! @{
using id_t = unsigned int;
constexpr id_t null_id = id_t(0);
constexpr id_t bad_id = id_t(-1);
//! @}
} // of namespace yrui

#define YRUI_UNUSED_VAR(var) ((void)var)
#define YRUI_NOT_IMPLEMENTED (BOOST_ASSERT(!"not implementation now"))
#define YRUI_DEFINE_IMP()                                                                                              \
    class Imp;                                                                                                         \
    friend class Imp;                                                                                                  \
    std::unique_ptr<Imp> imp_

#endif // !YRUI_CONFIG_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
