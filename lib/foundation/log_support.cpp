#include "foundation/log_support.h"

BOOST_LOG_GLOBAL_LOGGER_INIT(ylogger, boost::log::sources::severity_channel_logger_mt<>)
{
    static boost::log::sources::severity_channel_logger_mt<> logger(
        yrui::keywords::severity = ::yrui::detail::info, yrui::keywords::channel = "Core");
    return logger;
}

namespace yrui {
namespace detail {
    namespace {
        constexpr unsigned int names_count = 6;

        template <typename CharT> struct severity_level_names {
            static const CharT names[names_count][8];
        };

        template <typename CharT>
        const CharT severity_level_names<CharT>::names[names_count][8] = { { 't', 'r', 'a', 'c', 'e', 0 },
            { 'd', 'e', 'b', 'u', 'g', 0 }, { 'i', 'n', 'f', 'o', 0 }, { 'w', 'a', 'r', 'n', 'i', 'n', 'g', 0 },
            { 'e', 'r', 'r', 'o', 'r', 0 }, { 'f', 'a', 't', 'a', 'l', 0 } };
    }

    const char* to_string(severity_level lvl)
    {
        typedef severity_level_names<char> level_names;
        if (static_cast<unsigned int>(lvl) < names_count)
            return level_names::names[static_cast<unsigned int>(lvl)];
        else
            return nullptr;
    }
} // of namespace Detail
} // of namespace Bygone
