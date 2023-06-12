#include "kernel/common/common_util.h"
#include "kernel/core.h"
#include "kernel/core/subsys.h"
#include "kernel/effect/effect_mgr.h"
#include "kernel/fraction/fraction.h"
#include "kernel/person/person.h"

#include "kernel/impl/person_impl.h"

namespace yrui {
namespace aux {
    Pimpl<Person>::implementation::implementation(Person& interf, const Raw_person& raw) noexcept
        : self(interf)
        , raw_(raw)
        , fight_(raw_.fight)
        , leadership_(raw_.leadership)
        , wisdom_(raw_.wisdom)
        , politic_(raw_.politics)
    {
        YPRE_CONDITION(self.id());

        set_position(raw_.position);
    }

    void Pimpl<Person>::implementation::init_self()
    {
        if (raw_.stayed_city) {
            stayed_city_ = common::find_arch(raw_.stayed_city);
        }

        if (raw_.showed_city) {
            showed_city_ = common::find_arch(raw_.showed_city);
        }

        if (raw_.relative) {
            relative_ = common::find_person(raw_.relative);
        }

        if (raw_.fraction) {
            fraction_ = common::find_fraction(raw_.fraction);
        }

        auto& mgr = use_subsys<effect::Effect_mgr>();
        for (auto id : raw_.fixed_skill) {
            auto eff = mgr.get(id);
            if (!eff) {
                MLOG() << "Bad fixed skill id: " << id;
            } else {
                fixed_skill_.push_back(eff);
            }
        }

        calculate_abilities();

        if (fraction_) {
            yrui::core()->add_post_initializer([this] {
                if (fraction_->leader() == &self) {
                    set_position(position_const::leader);
                }
            });
        }
    }

    bool Pimpl<Person>::implementation::is_valid() noexcept
    {
        //! \todo
        return true;
    }

    void Pimpl<Person>::implementation::calculate_abilities() noexcept
    {
        farm_ability_ = Act_ability(politic_.val());
        trade_ability_ = Act_ability(politic_.val());
        security_ability_ = Act_ability((politic_.val() + leadership_.val()) / 2);
        steady_ability_ = Act_ability((politic_.val() + leadership_.val()) / 2);

        train_ability_ = Act_ability(leadership_.val());
        attack_ = Act_ability((leadership_.val() * 2 + fight_.val() * 8) / 10);
        defence_ = Act_ability((leadership_.val() * 8 + fight_.val() * 2) / 10);

        strategy_rate_ = Act_ability(wisdom_.val());
        strategy_success_rate_ = Act_ability(wisdom_.val());
        strategy_detect_rate_ = Act_ability(wisdom_.val());
        strategy_attack_ = Act_ability(wisdom_.val());
        strategy_defence_ = Act_ability((wisdom_.val() + leadership_.val()) / 2);

        //! exert fix skills
        for (auto eff : fixed_skill_) {
            eff->exert(&self);
        }
    }

    void Pimpl<Person>::implementation::set_position(Position pos) noexcept
    {
        raw_.position = pos;
        raw_.max_troop = base_troop + int(pos) * 500;

        //! 降低官职时兵力减少
        raw_.troop = std::min(raw_.max_troop, raw_.troop);
    }

    void Pimpl<Person>::implementation::leave_office() noexcept
    {
        BOOST_LOG_NAMED_SCOPE("LeaveFraction");

        YPRE_CONDITION(self.is_in_office());

        lose_troop();
        set_position(position_const::nothing);
        set_fraction(nullptr);

        YPOST_CONDITION(!self.is_in_office());
    }

    void Pimpl<Person>::implementation::set_stayed_city(Architecture* arch) noexcept { stayed_city_ = arch; }

    void Pimpl<Person>::implementation::set_fraction(Fraction* frac) noexcept { fraction_ = frac; }

    void Pimpl<Person>::implementation::lose_troop() noexcept
    {
        MLOG() << self.name() << " lose troop";

        raw_.troop = 0;
        raw_.morale = 0;
        raw_.drill = 0;
    }

    inline int pseduo_morale_loss(const person::Person& p, int attack) noexcept
    {
        return (1000 * attack) / (1 + p.morale() * p.controlled_troop());
    }

    inline int pseduo_drill_up(const person::Person& p, int attack) noexcept
    {
        return (1000 * attack) / (1 + p.drill() * p.controlled_troop());
    }

    //! 军队战损，会降低士气值，提高训练值，士气下降到一定地步，军队crash
    void Pimpl<Person>::implementation::troop_loss(int val) noexcept
    {
        YPRE_CONDITION(val >= min_loss);

        raw_.morale -= pseduo_morale_loss(self, val);
        raw_.drill += pseduo_drill_up(self, val);

        raw_.troop = val > int(raw_.troop) ? 0 : raw_.troop - val;

        if (raw_.morale < 0)
            raw_.morale = 0;
        if (raw_.troop == 0)
            lose_troop();
    }

    void Pimpl<Person>::implementation::set_loyalty(int val) noexcept
    {
        raw_.loyalty = std::min<int>(raw_.loyalty + val, raw_.max_loyalty);
    }

    ylogger::logger_type& Pimpl<Person>::implementation::logger() const { return self.logger(); }
}
} // of namespace yrui::aux
