#include "kernel/impl/archai/ai_architecture_defensive.h"

namespace AI = yrui::ai;

const std::string& AI::Arch_defensive_ai::id() const
{
    static std::string id_ = "core::defense";
    return id_;
}

void AI::Arch_defensive_ai::work_imp(const Timeline& tl)
{
    //! prepare jobs and queue jobs
    work_wu();
    work_wen();
}

void AI::Arch_defensive_ai::work_wen()
{
    using Task = std::function<void(Person*)>;
    using Task_queue = std::vector<Task>;

    Task_queue q;

    //! prepare task queue
    if (!arch_->has_done_farm())
        q.push_back(detail::Do_farm { arch_ });
    if (!arch_->has_done_security())
        q.push_back(detail::Do_security { arch_ });
    if (!arch_->has_done_trade())
        q.push_back(detail::Do_trade { arch_ });

    while (!wen_.empty() && !q.empty()) {
        q.back()(wen_.back());

        q.pop_back();
        wen_.pop_back();
    }

    if (!wen_.empty()) {
        BOOST_ASSERT(q.empty());

        for (auto p : wen_) {
            arch_->search_person(p);
        }
    }

    wen_.clear();
}

void AI::Arch_defensive_ai::work_wu()
{
    if (!arch_->has_done_steady()) {
        const auto steady = arch_->steady();

        auto iter = std::find_if(
            wu_.begin(), wu_.end(), [steady](Person* p) { return p->morale() > steady && p->drill() > steady; });

        if (iter != wu_.end()) {
            arch_->develop_steady(*iter);
            wu_.erase(iter);
        }
    }

    const auto can_supply = arch_->fraction_can_supply(arch_->troop());

    for (auto p : wu_) {
        if (can_supply && p->troop() < p->max_troop() / 2) {
            arch_->replenish(p);
        } else if (p->is_supplied()) {
            arch_->train(p);
        }
    }

    wu_.clear();
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
