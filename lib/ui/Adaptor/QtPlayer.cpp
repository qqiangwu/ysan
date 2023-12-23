#include "QtPlayer.h"
#include "Debugger.h"
#include "kernel/arch/architecture.h"
#include "kernel/common/person_filter.h"
#include "kernel/fraction/fraction.h"
#include "kernel/military/legion.h"
#include "kernel/person/person.h"

namespace Bygone {
namespace Frontend {
    void QtPlayer::prepare(const yrui::Timeline& tl)
    {
        Player_controller::prepare(tl);
        qDebug() << "It's your turn";
        emit playerThinking();
    }

    void QtPlayer::emit_decision(const yrui::Timeline& tl)
    {
        std::unique_lock<std::mutex> lock(mu_);
        decision_ends_.wait(lock);
    }

    yrui::uptr<yrui::Legion> QtPlayer::defend(yrui::Legion* attacker)
    {
        YPRE_CONDITION(attacker->to());

        using namespace yrui;

        auto defensor = attacker->to();

        if (defensor->in_city().empty())
            return nullptr;
        if (defensor->troop() < attacker->troop() / 2)
            return nullptr;

        auto candidates = to_vec(defensor->in_city() | filtered(common::Is_combatable {}));

        std::sort(
            candidates.begin(), candidates.end(), [](Person* x, Person* y) { return x->troop() > y->troop(); });

        if (candidates.size() > troop_num_limit)
            candidates.resize(troop_num_limit);

        while (!defensor->can_supply(candidates)) {
            candidates.pop_back();
        }

        if (candidates.empty())
            return nullptr;

        defensor->abort_jobs();
        return defensor->create_troop(nullptr, candidates);
    }

    void QtPlayer::command_troop(yrui::Legion* self, yrui::Legion*)
    {
        qDebug() << "Player decision in war";

        switch (emit actionRequired()) {
        case 0:
            return self->prepare_attack();
        case 1:
            return self->prepare_defense();
        case 2:
            return self->retreat();
        default:
            BOOST_ASSERT(!"Not here");
            break;
        }
    }

    void QtPlayer::command_troop(yrui::Legion* self, yrui::Intruded_arch*)
    {
        qDebug() << "Player decision in siege war";

        switch (emit actionRequired()) {
        case 0:
            return self->prepare_attack();
        case 1:
            return self->prepare_defense();
        case 2:
            return self->retreat();
        default:
            BOOST_ASSERT(!"Not here");
            break;
        }
    }

    bool QtPlayer::permit_strategy(yrui::Person* p)
    {
        qDebug() << "Ask for strategy";
        return emit yesOrNo(QString("允许 %1 担任谋士？").arg(p->name().c_str()));
    }

    void QtPlayer::notify_self() { decision_ends_.notify_one(); }
}
}
