#ifndef YRUI_CORE_AI_ARCHITECTURE_H_
#define YRUI_CORE_AI_ARCHITECTURE_H_

#include "kernel/component_fwd.h"

namespace yrui {
class Iarchitecture_ai {
public:
    virtual ~Iarchitecture_ai() = default;

public:
    virtual void take_control_of(Architecture* arch) = 0;
    virtual void think(const Timeline& tl) = 0;
    virtual const std::string& id() const = 0;
    virtual uptr<Iarchitecture_ai> clone() const = 0;
};
} // of namespace yrui

#endif // YRUI_CORE_AI_ARCHITECTURE_H_
