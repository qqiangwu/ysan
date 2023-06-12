#ifndef YRUI_COMPONENT_FWD_H_
#define YRUI_COMPONENT_FWD_H_

#include "kernel/config.h"

namespace yrui {
class Core;
class Game_obj;
class Isubsys;
class Timeline;
class Ifraction_ai;
class Iarchitecture_ai;

namespace person {
    class Person;
    class Person_mgr;
} // of namespace person

namespace arch {
    class Architecture;
    class Architecture_mgr;
} // of namespace arch

namespace fraction {
    class Fraction;
    class Fraction_mgr;
    class Fraction_controller;
} // of namespace fraction

namespace military {
    class Intruded_arch;
    class Legion;
    class Military_mgr;
    class Military_support;
    class Strategy;
}

// use it or not?
namespace job {
    class Job;
} // of namespace job

namespace report {
    class Report_mgr;
} // of namespace report

namespace effect {
    class Effect;
    class Effect_mgr;
    class Condition;
}

using arch::Architecture;
using arch::Architecture_mgr;
using effect::Condition;
using effect::Effect;
using fraction::Fraction;
using fraction::Fraction_controller;
using fraction::Fraction_mgr;
using military::Intruded_arch;
using military::Legion;
using military::Strategy;
using person::Person;
using person::Person_mgr;
} // of namespace yrui
#endif // !YRUI_COMPONENT_FWD_H_
