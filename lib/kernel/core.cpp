#include <memory>

#include "foundation/string_fmt.h"

#include "kernel/core.h"
#include "kernel/impl/core_impl.h"

namespace yrui {
namespace {
    std::unique_ptr<Core> __instance__ = nullptr;
} // of namespace unnamed

Core::Core(const std::string& config)
    : imp_(new Imp(config))
{
}

Core::~Core() { }

Core* core() { return __instance__.get(); }

Core* Core::create(const std::string& config)
{
    if (!__instance__) {
        __instance__.reset(new Core(config));

        __instance__->imp_->initialize_();
    }

    return __instance__.get();
}

void Core::destroy()
{
    if (__instance__) {
        __instance__.reset();
    }
}

Isubsys& Core::get_subsys(std::type_index ti)
{
    auto iter = imp_->sys_.find(ti);
    BOOST_ASSERT_MSG(iter != imp_->sys_.end(), "Bad subsystem");

    return *iter->second;
}

const Config_dict& Core::config() const { return imp_->config_; }

const Config_dict& Core::config(const std::string& subconfig) const
{
    return imp_->config_.get_child(fmt("config.%1%", subconfig));
}

uptr<Ifraction_ai> Core::make_fraction_ai() const
{
    BOOST_ASSERT(imp_->frac_ai_maker_);
    return imp_->frac_ai_maker_();
}

uptr<Iarchitecture_ai> Core::make_arch_ai(const std::string& name) const
{
    auto iter = imp_->arch_ai_.find(name);
    return iter != imp_->arch_ai_.end() ? iter->second()->clone() : nullptr;
}

bool Core::self_check() const { return {}; }

void Core::run() { imp_->run_(); }

void Core::stop() { imp_->stop_(); }

bool Core::is_running() const { return imp_->is_run_; }

void Core::add_post_initializer(std::function<void()> func) { imp_->post_initializer_.push_back(func); }
} // of namespace yrui

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
