#ifndef FOUNDATION_FOUNDATION_H_
#define FOUNDATION_FOUNDATION_H_

#include <csignal>
#include <exception>

#include "foundation/config_dict.h"
#include "foundation/contract.h"
#include "foundation/log_support.h"

namespace yrui {
/*!
 * \brief The Foundation class
 *
 * Log and configuration support
 */
class Foundation {
public:
    Foundation(int argc, const char** argv);

public:
    int argc() const { return argc_; }

    const char** argv() const { return argv_; }

    const Config_dict& gui_conf() const { return gui_conf_; }

    const Config_dict& app_conf() const { return app_conf_; }

public:
    using SignalHandler = void (*)(int);

    SignalHandler current_sig_handler() noexcept;
    std::terminate_handler current_handler() noexcept;

    void set_sig_handler(SignalHandler h) noexcept;
    void set_handler(std::terminate_handler h) noexcept;

private:
    std::terminate_handler th_ = nullptr;
    SignalHandler sh_ = nullptr;

private:
    int argc_;
    const char** argv_;

    Config_dict gui_conf_;
    Config_dict app_conf_;
};
} // of namespace yrui

#endif //! FOUNDATION_FOUNDATION_H_
