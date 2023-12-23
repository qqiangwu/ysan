#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <QDebug>
#include <boost/format.hpp>
#include <string>

inline QDebug&& operator<<(QDebug&& db, boost::basic_format<char>& fmt)
{
    return static_cast<QDebug&&>(db << fmt.str().c_str());
}

inline QDebug&& operator<<(QDebug&& db, const std::string& str) { return static_cast<QDebug&&>(db << str.c_str()); }

#endif // DEBUGGER_H
