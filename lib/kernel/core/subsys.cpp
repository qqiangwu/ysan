#include "kernel/core/subsys.h"
#include "kernel/core.h"

namespace yrui {
namespace detail {

    Isubsys& get_subsys(std::type_index ti) { return core()->get_subsys(ti); }
}

bool Isubsys::self_check() const { return {}; }

bool Isubsys::init()
{
    BOOST_ASSERT(!is_initialized());

    return init_ = init_impl();
}

bool Isubsys::is_initialized() const { return init_; }

void Isubsys::finalize()
{
    if (is_initialized()) {
        finalize_impl();
    }
}
}
