#include <boost/range.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

#include "foundation/contract.h"
#include "kernel/arch/architecture.h"
#include "kernel/common/person_filter.h"
#include "kernel/core/ai_fraction.h"
#include "kernel/fraction/fraction_controller.h"
#include "kernel/military/legion.h"
#include "kernel/person/person.h"

namespace YR = yrui::fraction;

void YR::AI_controller::emit_decision(const Timeline& tl)
{
    BOOST_ASSERT(ai_);

    ai_->think(tl);
}

yrui::uptr<yrui::Legion> YR::AI_controller::defend(Legion* attacker)
{
    YPRE_CONDITION(attacker->to());

    auto defensor = attacker->to();

    if (defensor->in_city().empty()) {
        YLOG() << "No persons in city";
        return nullptr;
    }
    if (defensor->troop() < attacker->troop() / 2) {
        YLOG() << format("Not enough troop %1% vs %2%") % defensor->troop() % attacker->troop();
        return nullptr;
    }

    auto candidates = to_vec(defensor->in_city() | filtered(common::Is_combatable {}));

    std::sort(candidates.begin(), candidates.end(), [](Person* x, Person* y) { return x->troop() > y->troop(); });

    if (candidates.size() > troop_num_limit)
        candidates.resize(troop_num_limit);

    while (!defensor->can_supply(candidates)) {
        candidates.pop_back();
    }

    if (candidates.empty()) {
        YLOG() << "Insufficient supply";
        return nullptr;
    }

    defensor->abort_jobs();

    return defensor->create_troop(nullptr, candidates);
}

void YR::AI_controller::command_troop(Legion* self, Legion* enemy)
{
    //! compare troops of self and enemy

    //! command troop
    self->prepare_attack();
}

void YR::AI_controller::command_troop(Legion* self, Intruded_arch* enemy)
{
    //! command troop
    self->prepare_attack();
}

bool YR::AI_controller::permit_strategy(Person* p) { return true; }

void YR::AI_controller::install_ai(uptr<Ifraction_ai> ai)
{
    ai_ = std::move(ai);
    ai_->take_control_of(self());
}

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
