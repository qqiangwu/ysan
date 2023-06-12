#ifndef YRUI_AI_AGGRESSIVEAI_H_
#define YRUI_AI_AGGRESSIVEAI_H_

#include <deque>
#include <functional>
#include <set>

#include "kernel/core/ai_architecture.h"
#include "kernel/core/ai_fraction.h"

namespace Fai {
using namespace yrui;

class Aggressive_ai : public Ifraction_ai {
public:
    virtual ~Aggressive_ai();

public:
    virtual void take_control_of(Fraction* f) override;

    virtual void think(const Timeline& tl) override;

    virtual const std::string& id() const override;

    virtual uptr<Ifraction_ai> clone() const override;

private:
    uptr<Iarchitecture_ai> make_attack_ai();
    uptr<Iarchitecture_ai> make_develop_ai();

    void on_get_city(Architecture* arch);
    void on_lose_city(Architecture* arch);
    void on_get_person(Person* p);
    void on_lose_person(Person* p);

    void init_fraction();
    void unregister_slots();

private:
    void pre_work(const Timeline& tl);
    void do_work(const Timeline& tl);
    void post_work(const Timeline& tl);

private:
    Fraction* self_;

    std::set<Architecture*> cities_losed_;
    std::set<Architecture*> cities_obtained_;
    std::set<Person*> persons_losed_;
    std::set<Person*> persons_obtained_;

    uptr<Iarchitecture_ai> cache_attack_;
    uptr<Iarchitecture_ai> cache_defense_;
    uptr<Iarchitecture_ai> cache_develop_;

    std::function<void()> unregister_;

private:
    using Task = std::function<void()>;
    using Task_queue = std::deque<Task>;

    void add_delayed(Task t);
    void handle_delayed(const Timeline& tl);

    Task_queue delayed_tasks_;
};
} // of namespace name

#endif // YRUI_AI_AGGRESSIVEAI_H_
