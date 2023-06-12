#ifndef YRUI_CORE_H_
#define YRUI_CORE_H_

/*!
 *
 * \file     core.h
 * \author   Wu QQ AT lv.yrui@foxmail.com
 * \brief    the main game object of Bygone.
 * \since
 *
 */

#include <boost/core/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>

#include <string>
#include <typeindex>

#include "kernel/component_fwd.h"
#include "kernel/config.h"
#include "kernel/core/ai_architecture.h"
#include "kernel/core/ai_fraction.h"

namespace yrui {
using Ptree = boost::property_tree::ptree;

/*!
 * \brief The Core class
 *
 * 这个类用于提供游戏的所有后台数据，供UI使用。
 *
 * 它运行在一个单独的逻辑线程中，UI仅读取数据。
 *
 * 当AI思考时，后台线程会有数据的改变。
 * 当玩家思考时，后台线程会暂时挂起，之后，玩家只可以通过UI修改后台数据。
 *
 * 当后台发生游戏事件时，会调用signal handler给前台发消息，消息是同步处理的，处理时，
 * 后台会挂起，以使得前台可以修改数据。
 *
 * Core仅仅有几个接口，被设计为线程安全的，以供UI修改Core的状态，如，开始游戏，停止游戏。
 *
 * UI可以通过Core访问游戏的各个子系统，从而，完成一系列的任务，故而，Core不应该提供过多的
 * 接口。
 *
 * \todo    add load_scenoria() support for reuse
 */
class Core : private boost::noncopyable {
public:
    static Core* create(const std::string& config);
    static void destroy();

public:
    ~Core();

public:
    bool self_check() const;

    //! \name   thread-safe interface for the frontend
    //! @{
    //! run the game
    void run();
    void stop();
    bool is_running() const;
    //! @}

public:
    //! \name   ai maker
    //! @{
    uptr<Ifraction_ai> make_fraction_ai() const;

    uptr<Iarchitecture_ai> make_arch_ai(const std::string& name) const;
    //! @}

public:
    //! \name   configuration service
    //! @{
    const Ptree& config() const;
    const Ptree& config(const std::string& subconfig) const;
    //! @}

public:
    //! \name   subsystem service
    //! @{
    template <class Subsys> Subsys& use_subsys()
    {
        return static_cast<Subsys&>(get_subsys(std::type_index(typeid(Subsys))));
    }

    Isubsys& get_subsys(std::type_index ti);

    void add_post_initializer(std::function<void()> func);
    //! @}

private:
    //! load a scenario from a config file with the format xml
    Core(const std::string& config);

private:
    YRUI_DEFINE_IMP();
};

CORE_DECL Core* core();
} // of namespace yrui

#endif // !YRUI_CORE_H_
