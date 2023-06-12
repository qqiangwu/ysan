#include "kernel/arch/architecture_mgr.h"

namespace Impl = yrui::arch;

void Impl::Architecture_mgr::emit_ownership_transfer(Architecture* arch, Fraction* from, Fraction* to)
{
    BOOST_LOG_NAMED_SCOPE("EmitOwnerShipTransfer");

    ownership_transfer(arch, from, to);
}
