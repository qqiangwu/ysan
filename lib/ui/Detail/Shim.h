#ifndef YRUI_FRONTEND_SHIM_H_
#define YRUI_FRONTEND_SHIM_H_

#include <QString>
#include <memory>

namespace Bygone {
namespace Frontend {
    template <class T> inline QString to_name(const std::unique_ptr<T>& obj) { return obj->name().c_str(); }

    template <class T> inline QString to_name(std::shared_ptr<T> obj) { return obj->name().c_str(); }

    template <class T> inline QString to_name(const T& obj) { return obj->name().c_str(); }
} // of namespace Frontend
} // of namespace Bygone

#endif // YRUI_FRONTEND_SHIM_H_
