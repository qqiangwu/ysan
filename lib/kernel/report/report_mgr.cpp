#include "kernel/report/report_mgr.h"
#include "kernel/core.h"

namespace yrui {
namespace report {
    void Report_mgr::update() { }

    bool Report_mgr::init_impl() { return true; }

    void Report_mgr::finalize_impl() { }

    bool Report_mgr::self_check() const { return {}; }
}
}
