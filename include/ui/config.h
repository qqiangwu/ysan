#ifndef FRONTEND_GLOBAL_H
#define FRONTEND_GLOBAL_H

#include <boost/config.hpp>

#if defined(FRONTEND_LIBRARY)
#define FRONTEND_API BOOST_SYMBOL_EXPORT
#else
#define FRONTEND_API BOOST_SYMBOL_IMPORT
#endif

#include <cassert>
#include <string>

#endif // FRONTEND_GLOBAL_H
