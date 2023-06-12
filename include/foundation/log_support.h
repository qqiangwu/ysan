#ifndef YRUI_LOG_SUPPORT_H_
#define YRUI_LOG_SUPPORT_H_

#include <boost/log/attributes/function.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/keywords/channel.hpp>
#include <boost/log/keywords/severity.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#include <typeindex>

namespace yrui {
namespace detail {
    enum severity_level { trace, debug, info, warning, error, fatal };

    //! Returns stringized enumeration value or \c NULL, if the value is not valid
    const char* to_string(severity_level lvl);

    //! Outputs stringized representation of the severity level to the stream
    template <typename CharT, typename TraitsT>
    inline std::basic_ostream<CharT, TraitsT>& operator<<(std::basic_ostream<CharT, TraitsT>& strm, severity_level lvl)
    {
        const char* str = detail::to_string(lvl);
        if (str)
            strm << str;
        else
            strm << static_cast<int>(lvl);
        return strm;
    }
}

namespace keywords = boost::log::keywords;
} // of namespace yrui

BOOST_LOG_GLOBAL_LOGGER(ylogger, boost::log::sources::severity_channel_logger_mt<>)

#define NLOG(lg) BOOST_LOG(lg)

//! use global logger with default severity and channel
#define YLOG() BOOST_LOG(ylogger::get())

//! use global logger with severity provided
#define YLOG_SEV(level) BOOST_LOG_SEV(ylogger::get(), ::yrui::detail::level)

#define YLOG_CH(chan) BOOST_LOG_CHANNEL(ylogger::get(), chan)

//! use member logger with default severity
#define MLOG() BOOST_LOG(logger())

//! use member logger with severity provieded
#define MLOG_SEV(level) BOOST_LOG_SEV(logger(), ::yrui::detail::level)

#define DECLARE_MEMBER_LOGGER(channel_)                                                                                \
public:                                                                                                                \
    ylogger::logger_type& logger() { return logger_; }                                                                 \
                                                                                                                       \
private:                                                                                                               \
    ylogger::logger_type logger_ { yrui::keywords::channel = (channel_) }

/*!
 *
 * Note that one can also use both
 *
 *  BOOST_LOG_SCOPED_LOGGER_TAG(logger, tag_name, tag_value);
 *  BOOST_LOG_SCOPED_THREAD_TAG(tag_name, tag_value);
 *
 * to leverage scoped attributes.
 *
 * Also note that do disinguish %named_scope% and %scoped attributes%
 */

#define LOG_ATTACH_TAG(tag_name, tag_value) BOOST_LOG_SCOPED_THREAD_TAG("!" #tag_name, tag_value);

#define LOG_ATTACH_ATTR(tag_name, tag_attr)                                                                            \
    BOOST_LOG_SCOPED_THREAD_ATTR("!" #tag_name, ::boost::log::attributes::make_function(tag_attr));

#endif // YRUI_LOG_SUPPORT_H_
