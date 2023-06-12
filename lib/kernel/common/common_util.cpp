#include "kernel/common/common_util.h"
#include "kernel/arch/architecture.h"
#include "kernel/arch/architecture_mgr.h"
#include "kernel/core.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/military/legion.h"
#include "kernel/military/military_mgr.h"
#include "kernel/person/person_mgr.h"

namespace yrui {
namespace common {
    const resource::Map& map() { return use_subsys<military::Military_mgr>().map(); }

    Person* find_person(id_t id) { return use_subsys<person::Person_mgr>().get(id); }

    Architecture* find_arch(id_t id) { return use_subsys<arch::Architecture_mgr>().get(id); }

    Fraction* find_fraction(id_t id) { return use_subsys<fraction::Fraction_mgr>().get(id); }

    Fraction_controller* get_controller(Architecture* arch)
    {
        return arch->fraction() ? arch->fraction()->controller() : nullptr;
    }

    Fraction_controller* get_controller(Legion* lg)
    {
        BOOST_ASSERT(lg->fraction());
        return lg->fraction()->controller();
    }

    std::vector<Architecture*> adjacent(Architecture* arch) { return map().adjacent_city(arch); }
}
}
