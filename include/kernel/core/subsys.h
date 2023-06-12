#ifndef YRUI_CORE_SUBSYS_H_
#define YRUI_CORE_SUBSYS_H_

#include <boost/core/noncopyable.hpp>
#include <typeindex>

#include "kernel/config.h"

namespace yrui {
class Core;

class Isubsys : private boost::noncopyable {
public:
    virtual ~Isubsys() = default;

public:
    bool init();
    bool is_initialized() const;
    void finalize();

    virtual bool self_check() const;
    virtual void update() = 0;

private:
    virtual bool init_impl() = 0;
    virtual void finalize_impl() = 0;

private:
    bool init_ {};
};

namespace detail {
    Isubsys& get_subsys(std::type_index ti);
}

template <class Subsys> inline Subsys& use_subsys()
{
    auto& raw = detail::get_subsys(std::type_index(typeid(Subsys)));
    return static_cast<Subsys&>(raw);
}
} // of namespace yrui

#endif // !YRUI_CORE_SUBSYS_H_
