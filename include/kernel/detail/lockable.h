#ifndef YRUI_DETAIL_LOCKABLE_H_
#define YRUI_DETAIL_LOCKABLE_H_

#include <mutex>

namespace yrui {
namespace detail {
    struct Lockable {
    protected:
        using mutex_type = std::mutex;
        using lock_type = std::unique_lock<mutex_type>;

        lock_type lock_self() const { return lock_type { mutex_ }; }

    private:
        mutable mutex_type mutex_;
    };
} // of namespace detail
} // of namespace name

#endif // YRUI_DETAIL_LOCKABLE_H_
