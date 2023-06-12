#include "kernel/impl/archai//ai_architecture_common.h"

///////////////////////////////////////////////////////////////////////////////
namespace AI = yrui::ai;

///////////////////////////////////////////////////////////////////////////////
void AI::detail::Arch_ai_common::take_control_of(Architecture* arch) { arch_ = arch; }

void AI::detail::Arch_ai_common::think(const Timeline& tl)
{
    BOOST_ASSERT(arch_);
    BOOST_ASSERT(arch_->fraction());

    mayor_ = arch_->mayor();
    if (mayor_ && !arch_->has_worked(mayor_)) {
        wen_.clear();
        wu_.clear();

        for (auto p : arch_->unworked()) {
            if (p->has_troop()) {
                wu_.push_back(p);
            } else {
                wen_.push_back(p);
            }
        }

        work_imp(tl);
    }
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
