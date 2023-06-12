#ifndef YRUI_CORE_DETAIL_SEMANTIC_INT_H_
#define YRUI_CORE_DETAIL_SEMANTIC_INT_H_

/*
 * 提供有语义的int值，作为接口使用。
 *
 */

#include "kernel/config.h"

namespace yrui {
namespace detail {
    template <int min, int max, class tag> class Semantic_int {
    public:
        typedef Semantic_int self_type;

        enum { min_value = min };
        enum { max_value = max };

    public:
        Semantic_int() noexcept
            : rep_(min_value)
        {
        }

        explicit Semantic_int(int x) noexcept
            : rep_(x)
        {
            rep_ = min_value > x ? min_value : (max_value < x ? max_value : x);
        }

        int val() const noexcept
        {
            BOOST_ASSERT(min_value <= rep_);
            BOOST_ASSERT(rep_ <= max_value);
            return rep_;
        }

        self_type operator+(int delta) const noexcept
        {
            BOOST_ASSERT(min_value <= rep_);
            BOOST_ASSERT(rep_ <= max_value);
            return self_type(rep_ + delta);
        }

        self_type operator-(int delta) const noexcept
        {
            BOOST_ASSERT(min_value <= rep_);
            BOOST_ASSERT(rep_ <= max_value);
            return self_type(rep_ - delta);
        }

        self_type operator-(self_type s) const noexcept { return *this - s.val(); }

        explicit operator bool() const noexcept { return !!rep_; }

    private:
        int rep_;
    };

    template <int min, int max, class tag> inline bool operator<(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() < y;
    }

    template <int min, int max, class tag> inline bool operator>(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() > y;
    }

    template <int min, int max, class tag> inline bool operator<=(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() <= y;
    }

    template <int min, int max, class tag> inline bool operator>=(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() >= y;
    }

    template <int min, int max, class tag> inline bool operator==(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() == y;
    }

    template <int min, int max, class tag> inline bool operator!=(Semantic_int<min, max, tag> x, int y) noexcept
    {
        return x.val() != y;
    }

    template <int min, int max, class tag>
    inline bool operator<(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() < y.val();
    }

    template <int min, int max, class tag>
    inline bool operator<=(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() <= y.val();
    }

    template <int min, int max, class tag>
    inline bool operator>(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() > y.val();
    }

    template <int min, int max, class tag>
    inline bool operator>=(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() >= y.val();
    }

    template <int min, int max, class tag>
    inline bool operator==(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() == y.val();
    }

    template <int min, int max, class tag>
    inline bool operator!=(Semantic_int<min, max, tag> x, Semantic_int<min, max, tag> y) noexcept
    {
        return x.val() != y.val();
    }
}
}

#endif // YRUI_CORE_DETAIL_SEMANTIC_INT_H_
