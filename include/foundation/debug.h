#ifndef YRUI_FOUNDATION_DEBUG_H_
#define YRUI_FOUNDATION_DEBUG_H_

#ifndef NDEBUG
#define YDEBUG(stmt) stmt;
#define YDEBUG_INC(header) #include <##header>
#else
#define YDEBUG(stmt) /* empty */
#define YDEBUG_INC(header) /* empty */
#endif

#endif //! YRUI_FOUNDATION_DEBUG_H_
