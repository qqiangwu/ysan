#ifndef YRUI_ARCHITECTURE_H_
#define YRUI_ARCHITECTURE_H_

/*!
 *
 * \file     Architecture/architecture.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    define architecture objects.
 * \since
 *
 */

#include <set>
#include <vector>

#include "kernel/arch/architecture_type.h"
#include "kernel/detail/characteristic_type.h"
#include "kernel/detail/game_obj_common.h"
#include "kernel/detail/range_view.h"
#include "kernel/job/job.h"

namespace yrui {
namespace detail {
    struct Raw_architecture;
} // of namespace detail

namespace arch {
    class Architecture : public yrui::detail::Game_obj_common<Architecture, Architecture_mgr> {
    public:
        friend class Architecture_mgr;

        //! \name   structors
        //! @{
    public:
        explicit Architecture(const yrui::detail::Raw_architecture& raw);

        virtual ~Architecture();
        //! @}

        //! \name   main logical callbacks
        //! @{
    public:
        virtual void on_month_begin(const Timeline& t) override;
        virtual void on_month_end(const Timeline& t) override;
        //! @}

        //! \name   observers
        //! @{
    public:
        //! \name   global properties
        //! @{
        const std::string& name() const noexcept;

        Fraction* fraction() const noexcept;
        Person* mayor() const noexcept;
        Archtype type() const noexcept;
        //! @}

        //! \name   state queries
        //! @{
        bool is_capital() const noexcept;
        bool is_frontier() const noexcept;
        bool capital_connected() const noexcept;

        //! has the arch beleaguered last month?
        bool has_beleaguered() const noexcept;

        bool can_escauge(int ntroop) const noexcept;
        bool can_supply(int ntroops) const noexcept;
        bool can_supply(const std::vector<Person*>& p) const noexcept;

        //! query to the fraction
        bool fraction_can_supply(int ntroops) const noexcept;

        //! \name   城池工作相关
        //! 这里面的工作都会被加入工作队列，留到月末完成。
        //! @{
        bool has_done_farm() const noexcept;
        bool has_done_trade() const noexcept;
        bool has_done_steady() const noexcept;
        bool has_done_security() const noexcept;

        bool has_worked(Person* p) const noexcept;
        //! @}

        //! \name   architecture info
        //! @{
        int farming() const noexcept;
        int max_farming() const noexcept;
        int farming_growth() const noexcept;

        int trade() const noexcept;
        int max_trade() const noexcept;
        int trade_growth() const noexcept;

        int steady() const noexcept;
        int max_steady() const noexcept;

        int security() const noexcept;
        int security_growth() const noexcept;

        int population() const noexcept;
        int max_population() const noexcept;
        int population_growth() const noexcept;

        int corruption() const noexcept;
        int efficiency() const noexcept;
        int aristocrat() const noexcept;

        int money() const noexcept;
        int food() const noexcept;
        //! @}

        //! military properties
        //! @{
        int escuage() const noexcept;
        int troop() const noexcept;
        //! @}

        //! person related
        //! @{
        int num_all() const noexcept;
        int num_in_city() const noexcept;
        int num_out_of_city() const noexcept;
        int num_worked() const noexcept;
        int num_unworked() const noexcept;

        const std::set<Person*>& all() const noexcept;
        const std::set<Person*>& in_city() const noexcept;
        const std::set<Person*>& out_of_city() const noexcept;

        auto unworked() const noexcept;
        auto worked() const noexcept;

        const std::set<uptr<job::Job>>& jobs() const noexcept;
        //! @}
        //! @}

        //! \name   modifiers
        //! @{
    public:
        //! \name   architecture info
        //! 考虑到这些值都有增有降，因此使用这种低级接口
        //! @{
        void set_farming(int n);
        void set_max_farming(int n);
        void set_farming_growth(int n);

        void set_trade(int n);
        void set_max_trade(int n);
        void set_trade_growth(int n);

        void set_steady(int n);
        void set_max_steady(int n);

        void set_security(int n);
        void set_security_growth(int n);

        void set_population(int n);
        void set_max_population(int n);
        void set_population_growth(int n);

        void set_money(int n);
        void set_food(int n);
        //! @}

        //! \name
        //! @{
        //! 推荐使用高级接口
        void consume_money(int s);
        void consume_food(int s);
        void consume_escuage(int s);
        //! @}
        //! @}

        //! \name   actions
        //! may emit signals
        //! @{
    public:
        // harvest in September
        void harvest();

        void set_mayor(Person* p);
        void dismiss_troop(Person* p);

        //! \name queued jobs
        //! @{
        void develop_farming(Person* p);
        void develop_trade(Person* p);
        void develop_security(Person* p);
        void develop_steady(Person* p);
        void search_person(Person* p);

        /*!
         * \brief transport some money or food to another city
         */
        void transport(Person* p, Architecture* target, int money, int food);

        /*! 为人物新建一只军队 */
        void conscript(Person* p);

        /*!
         * \brief 进行征兵，以大量恢复军队数目。
         *
         * 如果兵役充足，每月被会自动恢复兵力，但显式地征兵恢复速度会更加快一些。
         */
        void replenish(Person* p);

        /*!
         * \brief 训练军队 -> 加入 job
         */
        void train(Person* p);

        void add_job(uptr<job::Job> j);

        //! general wrapper to simplify the work logic
        template <class JobKind, class... Args> void do_job(Person* p, Args&&... args);
        //! @}

        //! 停止所有工作队列中还没有完成的任务
        void abort_jobs();
        //! @}

        void supply(int ntroops);

        bool ask_for_supply(int num_of_troops);

        //! 城池处于被围攻状态
        void beleaguered();

        //! 仅仅创建一支军队，即，调用人物，取用粮草，但需要将Legion加入到Military_mgr
        //! 才可正式出兵。请使用Fraction::intrude方法。
        uptr<Legion> create_troop(Architecture* target, const std::vector<Person*>& troops);
        //! @}

        //! \name   war related
        //! remove this!
        //! @{
    public:
        //! 城池被围攻
        int suffer_siege();

        //! 城池中了坑道之计
        int suffer_wreck(Strategy_power power);

        /*!
         * \brief 城池被强攻
         * \param 进攻强度
         * \return 反击强度
         */
        Attack_power suffer_attack(Attack_power attack_strength);

        bool is_conquered() const noexcept;
        //! @}

        //! \name   ai related
        //! @{
    public:
        void install_ai(uptr<Iarchitecture_ai> ai);
        void think(const Timeline& tl);
        //! @}

        //! 城池人员管理
        //! @{
        //! \fixme
    public:
        //! \name   城池内部状态
        //! 现在的问题是，当人物加入城池，我希望只调用p->garrison(arch);
        //! 但是，城池的属性必须要相应地更新，可我又不希望让arch本身显露像上面这样的接口，
        //! 因为到后来，我自己都可能用错。怎么办？一种方法是为Person提供一些独有的接口，
        //! 让它来调用，从而改变Arch的状态。比如，提供一个内部类Notifier。
        //! 或者，使用一个中立的函数，让它负责同时改变相关对象的状态。
        //! 或者，使用Observers。
        //! @{

        /*!
         * 设置城池归属新势力
         * 当城池被一个势力占领时传动调用此方法，城池所有权由一个势力被转交给另外一个势力。
         *
         * 会触发arch_transfer信号，Fraction应该监测此信号，以更新自己所维护的arch列表。
         *
         * 如果我为Fraction类添加一个take_control_of(Architecture*)方法，那么，我们很容易
         * 弄不清它和此方法的区别，毕竟，这两个方法都可以改变Arch的归属，这是我所不希望看
         * 到的。但如果不这样，我就需要在此函数中处理俘虏逻辑。
         *
         * 还有一种方法，就是将take_control_of的逻辑放到Legion中。将军队接管某座城池，
         * 因为这可能是被隔断城池出兵的结果，首都无法干涉。
         *
         * 旧势力的所有状态会被清空。所有人员将会成为俘虏。（？）
         *
         * 现在的问题是：
         *      Person如何转移？
         */
        void transfer_to(Fraction* f);
        //! @}

        //! \name   slots
        //! @{
    private:
        /*!
         * 通知城池人物被加入，城池不能更改人物的内部属性
         */
        void on_obtain_person(Person* p);

        /*!
         *  通知城池人物被移除，城池不能更改人物的内部属性
         */
        void on_remove_person(Person* p);

        void on_fraction_die(Fraction* f);

    public:
        //! \todo   fix this
        /*!
         *  人物暂时离开（出征）
         */
        void on_person_go_out_(Person* p);

        /*!
         * \brief person_back
         * 人物归来
         */
        void on_person_back_(Person* p);
        //! @}

    private:
        void init_self();
    };
} // of namespace arch
} // of namespace yrui

//! implementation
namespace yrui {
namespace arch {
    inline auto Architecture::unworked() const noexcept
    {
        auto not_worked = [this](Person* p) { return !has_worked(p); };

        return make_range(in_city() | filtered(not_worked));
    }

    inline auto Architecture::worked() const noexcept
    {
        auto is_worked = [this](Person* p) { return has_worked(p); };

        return make_range(in_city() | filtered(is_worked));
    }

    inline int Architecture::num_all() const noexcept { return all().size(); }

    inline int Architecture::num_in_city() const noexcept { return in_city().size(); }

    inline int Architecture::num_out_of_city() const noexcept { return out_of_city().size(); }

    inline int Architecture::num_worked() const noexcept { return size(worked()); }

    inline int Architecture::num_unworked() const noexcept { return size(unworked()); }

    namespace detail {
        inline bool contains(const std::set<Person*>& s, Person* p) { return s.find(p) != s.end(); }
    } // of namespace detail

    template <class JobKind, class... Args> inline void Architecture::do_job(Person* p, Args&&... args)
    {
        add_job(job::make_job<JobKind>(p, std::forward<Args>(args)...));

        BOOST_ASSERT(has_worked(p));
    }
} // of namespace arch
} // of namespace yrui

#endif // !YRUI_ARCHITECTURE_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
