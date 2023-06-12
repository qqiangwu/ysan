#ifndef YRUI_CORE_AI_FRACTION_H_
#define YRUI_CORE_AI_FRACTION_H_

#include "kernel/component_fwd.h"

namespace yrui {
class Ifraction_ai {
public:
    virtual ~Ifraction_ai() = default;

public:
    //! take control of a fraction.
    virtual void take_control_of(Fraction* f) = 0;

    //! called every turn to do decisions
    virtual void think(const Timeline& tl) = 0;

    //! identifier of the AI
    virtual const std::string& id() const = 0;

    //! create a new AI object
    virtual uptr<Ifraction_ai> clone() const = 0;
};
} // of namespace yrui::ai

#endif // YRUI_CORE_AI_FRACTION_H_
