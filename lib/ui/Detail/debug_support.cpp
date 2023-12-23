#include "kernel/config.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

namespace boost {
void assertion_failed_msg(char const* expr, char const* msg, char const* function, char const* file, long line)
{
    std::ostringstream out;
    out << "\n"
        << "***** Internal Program Error - assertion (" << expr << ") failed in " << function << ":\n"
        << file << '(' << line << "): " << msg << std::endl;

    const auto m = out.str();

    YLOG_SEV(fatal) << m;
    std::cerr << m;

    std::abort();
}

void assertion_failed(char const* expr, char const* function, char const* file, long line)
{
    std::ostringstream out;
    out << "\n"
        << "***** Internal Program Error - assertion (" << expr << ") failed in " << function << ":\n"
        << file << '(' << line << ")";

    const auto msg = out.str();

    YLOG_SEV(fatal) << msg;
    std::cerr << msg;

    std::abort();
}
}
