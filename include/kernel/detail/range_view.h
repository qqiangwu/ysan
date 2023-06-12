#ifndef YRUI_DETAIL_RANGE_VIEW_H_
#define YRUI_DETAIL_RANGE_VIEW_H_

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/iterator_range_core.hpp>
#include <vector>

namespace yrui {
using boost::adaptors::filtered;

template <class ForwardIterator> class Range_view : public boost::iterator_range<ForwardIterator> {
public:
    using boost::iterator_range<ForwardIterator>::iterator_range;

    std::vector<typename ForwardIterator::value_type> copy() const { return { this->begin(), this->end() }; }
};

template <class ForwardIterator>
inline Range_view<ForwardIterator> make_range(ForwardIterator begin, ForwardIterator end)
{
    return Range_view<ForwardIterator>(begin, end);
}

template <class SinglePassRange> inline Range_view<typename SinglePassRange::iterator> make_range(SinglePassRange& rng)
{
    return make_range(rng.begin(), rng.end());
}

template <class SinglePassRange>
inline Range_view<typename SinglePassRange::const_iterator> make_range(const SinglePassRange& rng)
{
    return make_range(rng.begin(), rng.end());
}

template <class Container, class Iterator> inline Container copy(Range_view<Iterator> rng)
{
    return Container(rng.begin(), rng.end());
}

template <class Container, class Range> inline Container copy(const Range& rng)
{
    return Container(rng.begin(), rng.end());
}

template <class Range> inline std::vector<typename Range::value_type> to_vec(const Range& rng)
{
    return std::vector<typename Range::value_type>(rng.begin(), rng.end());
}
} // of namespace yrui

#endif //! YRUI_DETAIL_RANGE_VIEW_H_
