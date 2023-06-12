#ifndef YRUI_CORE_IMP_H_
#define YRUI_CORE_IMP_H_

#include <atomic>
#include <functional>
#include <unordered_map>

#include "foundation/config_dict.h"

#include "kernel/core.h"
#include "kernel/core/subsys.h"

namespace yrui {
class Core::Imp {
public:
    friend class Core;

    Imp(const std::string& config);

    uptr<Ifraction_ai> make_fraction_ai();
    uptr<Iarchitecture_ai> make_arch_ai(const std::string& name);

private:
    void run_();
    void stop_() { is_run_ = false; }

private:
    template <class Subsys> Subsys& use_subsys_()
    {
        auto elem = sys_.find(std::type_index(typeid(Subsys)));
        assert(elem != sys_.end());
        return static_cast<Subsys&>(*elem->second);
    }

private:
    //! load db
    //! @{
    //! load a scenario from a config file with the format xml
    void load_scenario_(const boost::property_tree::ptree& config);

    void load_person_(const std::string& db);
    void load_architecture_(const std::string& db);
    void load_fraction_(const std::string& db);
    void load_map_(const std::string& db);
    void load_ai_(const std::string& dll);
    void self_check_();

    void initialize_();

private:
    template <class Subsys> void install_subsys_(Subsys* sys)
    {
        sys_.emplace(std::type_index(typeid(Subsys)), uptr<Isubsys>(sys));
    }

private:
    Config_dict config_;

    std::vector<std::function<void()>> post_initializer_;

    std::function<uptr<Ifraction_ai>()> frac_ai_maker_;
    std::unordered_map<std::string, std::function<uptr<Iarchitecture_ai>()>> arch_ai_;
    std::unordered_map<std::type_index, uptr<Isubsys>> sys_;

    std::atomic_bool is_run_ {};
};
} // of namespace yrui

#endif //! YRUI_CORE_IMP_H_
