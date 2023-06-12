#include "kernel/arch/architecture.h"
#include "kernel/fraction/fraction.h"
#include "kernel/fraction/fraction_mgr.h"
#include "kernel/person/person.h"

#include "kernel/aggr_ai/aggressive_maker.h"
#include "kernel/aggr_ai/aggressiveai.h"

namespace Fai {
void Aggressive_ai::take_control_of(Fraction* f)
{
    YPRE_CONDITION(f);

    unregister_slots();

    self_ = f;

    auto& mgr = f->mgr();

    auto c1 = mgr.arch_obtained.connect(sig_level::observer, [this, f](Fraction* pf, Architecture* a) {
        BOOST_ASSERT(pf);
        BOOST_ASSERT(a->fraction() == pf);
        if (pf == f)
            on_get_city(a);
    });

    auto c2 = mgr.arch_lost.connect(sig_level::observer, [this, f](Fraction* pf, Architecture* a) {
        if (pf == f)
            on_lose_city(a);
    });

    auto c3 = mgr.person_obtained.connect(sig_level::observer, [this, f](Fraction* pf, Person* p) {
        BOOST_ASSERT(pf);
        BOOST_ASSERT(pf == p->fraction());
        if (pf == f)
            on_get_person(p);
    });

    auto c4 = mgr.person_lost.connect(sig_level::observer, [this, f](Fraction* pf, Person* p) {
        if (pf == f)
            on_lose_person(p);
    });

    unregister_ = [c1, c2, c3, c4]() {
        c1.disconnect();
        c2.disconnect();
        c3.disconnect();
        c4.disconnect();
    };

    add_delayed([this] { init_fraction(); });
}

void Aggressive_ai::think(const Timeline& tl)
{
    pre_work(tl);
    do_work(tl);
    post_work(tl);
}

const std::string& Aggressive_ai::id() const
{
    static std::string id_ { "wu_Aggressive_ai" };

    return id_;
}

yrui::uptr<Ifraction_ai> Aggressive_ai::clone() const { return yrui::make_uptr<Aggressive_ai>(); }
} // of namespace Fai

namespace yrui {
uptr<Ifraction_ai> make_fraction_ai() { return make_uptr<Fai::Aggressive_ai>(); }
} // of namespace yrui
