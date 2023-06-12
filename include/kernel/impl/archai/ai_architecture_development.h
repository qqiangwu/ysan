#ifndef YRUI_CORE_AI_ARCHITECTURE_DEVELOPMENT_H
#define YRUI_CORE_AI_ARCHITECTURE_DEVELOPMENT_H

#include "kernel/impl/archai/ai_architecture_common.h"

namespace yrui {
namespace ai {
    //! 内政AI
    class Arch_development_ai : public detail::Arch_ai_common {
    protected:
        virtual void work_imp(const Timeline& tl) override;

        virtual const std::string& id() const override;

        virtual uptr<Iarchitecture_ai> clone() const override { return make_uptr<Arch_development_ai>(); }

    protected:
        virtual void work_wen() override;
        virtual void work_wu() override;
    };
}
}

#endif // YRUI_CORE_AI_ARCHITECTURE_DEVELOPMENT_H
