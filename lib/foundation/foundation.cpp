#include <boost/property_tree/json_parser.hpp>

#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions/message.hpp>
#include <boost/log/keywords/auto_flush.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/open_mode.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>
#include <boost/stacktrace.hpp>

#include <algorithm>
#include <iterator>

#include "foundation/foundation.h"
#include "foundation/string_fmt.h"

namespace Impl = yrui;

namespace {
using boost::format;

std::string backtrace() noexcept
try {
    std::ostringstream out;

    boost::stacktrace::stacktrace stk;

    auto beg = stk.begin();

    //! fix a curious bug
    if (beg != stk.end()) {
        ++beg;
    }
    if (beg != stk.end()) {
        ++beg;
    }

    std::copy(beg, stk.end(), std::ostream_iterator<boost::stacktrace::frame>(out, "\n"));

    return out.str();
} catch (...) {
    return "Unknown crash";
}

void core_sig_handler(int signum) noexcept
{
    // associate each signal with a signal name string.
    const char* name = nullptr;

    switch (signum) {
    case SIGABRT:
        name = "SIGABRT";
        break;
    case SIGSEGV:
        name = "SIGSEGV";
        break;
    case SIGILL:
        name = "SIGILL";
        break;
    case SIGFPE:
        name = "SIGFPE";
        break;
    }

    if (name != nullptr) {
        YLOG_SEV(fatal) << format("Caught signal %1% (%2%)") % signum % name;
    } else {
        YLOG_SEV(fatal) << format("Caught signal %1%\n") % signum;
    }

    YLOG_SEV(fatal) << backtrace();

    // flush the log in that system may be not close the log file.
    boost::log::core::get()->flush();

    std::_Exit(EXIT_FAILURE);
}

void core_terminate_handler() noexcept
{
    YLOG_SEV(fatal) << "Program has abnormally terminated";

    if (std::uncaught_exceptions() > 0) {
        try {
            std::rethrow_exception(std::current_exception());
        } catch (std::exception& e) {
            YLOG_SEV(fatal) << "Exception: " << e.what();
        }
    }

    YLOG_SEV(fatal) << backtrace();

    // flush the log in that system may be not close the log file.
    boost::log::core::get()->flush();

    // exit without any cleanup
    std::_Exit(EXIT_FAILURE);
}
}

namespace {
BOOST_LOG_ATTRIBUTE_KEYWORD(channel_, "Channel", std::string)
// BOOST_LOG_ATTRIBUTE_KEYWORD(severity_, "Severity", ::yrui::detail::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(scope_, "Scope", boost::log::attributes::named_scope::value_type)

/*!
 * We use:
 *      Channel to logically separate the application into several parts.
 *      Scope to address the execution flow.
 *      Severity to identify its importance.
 *      Tag to attach extra information.
 */
void log_fmt(const boost::log::record_view& rec, boost::log::formatting_ostream& out)
{
    using namespace boost::log;

    out << "{" << rec[channel_] << "} "
        << "[" << rec[scope_] << "] ";

    //! other tags
    for (const auto& attr : rec.attribute_values()) {
        const auto& name = attr.first.string();
        if (name.front() == '!') {
            out << "<" << &name[1] << ":" << attr.second.extract<std::string>() << "> ";
        }
    }

    out << "- " << rec[expressions::smessage];
}

void clear_old_log(const boost::filesystem::path& p)
{
    using namespace boost::filesystem;

    auto log_folder = p.parent_path();
    if (!log_folder.empty()) {
        remove_all(log_folder);
    }
}

void setup_log(const yrui::Config_dict& config)
{
    using namespace yrui;

    //! int log
    auto enabled = config.get("enabled", true);
    if (enabled) {
        auto log_file = config.get<std::string>("file", "monitor");

        clear_old_log(log_file);

        //! add attributes
        boost::log::core::get()->add_global_attribute("Scope", boost::log::attributes::named_scope());

        constexpr auto kMB = 1024 * 1024;
        auto sink = boost::log::add_file_log(keywords::file_name = fmt("%1%-%%N.log", log_file),
            keywords::open_mode = std::ios::out, keywords::auto_flush = true, keywords::rotation_size = kMB);

        sink->set_formatter(&log_fmt);
    }
}
}

Impl::Foundation::Foundation(const int argc, const char** argv)
    : argc_ { argc }
    , argv_ { argv }
{
    set_handler(core_terminate_handler);
    set_sig_handler(core_sig_handler);

    boost::property_tree::read_json("config/app.conf", app_conf_);
    boost::property_tree::read_json("config/gui.conf", gui_conf_);

    setup_log(app_conf_.get_child("log"));
}

std::terminate_handler Impl::Foundation::current_handler() noexcept { return th_; }

void Impl::Foundation::set_handler(std::terminate_handler h) noexcept
{
    std::set_terminate(h);
    th_ = h;
}

void Impl::Foundation::set_sig_handler(SignalHandler h) noexcept
{
    std::signal(SIGABRT, h);
    std::signal(SIGSEGV, h);
    std::signal(SIGILL, h);
    std::signal(SIGFPE, h);

    sh_ = h;
}

Impl::Foundation::SignalHandler Impl::Foundation::current_sig_handler() noexcept { return sh_; }
