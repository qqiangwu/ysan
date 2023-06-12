#ifndef YRUI_CORE_MAP_LOCATION_H_
#define YRUI_CORE_MAP_LOCATION_H_

namespace yrui {
namespace resource {
    class Location {
    public:
        constexpr Location() noexcept = default;

        constexpr Location(int x, int y) noexcept
            : x_(x)
            , y_(y)
        {
        }

    public:
        constexpr int x() const noexcept;
        constexpr int y() const noexcept;

    private:
        int x_ = 0;
        int y_ = 0;
    };
} // of namespace resource
} // of namespace yrui

//! implementation
namespace yrui {
namespace resource {
    inline constexpr int Location::x() const noexcept { return x_; }
    inline constexpr int Location::y() const noexcept { return y_; }
} // of namespace resource
} // of namespace yrui

#endif // !YRUI_CORE_MAP_LOCATION_H_

/*!------------------------------------------------------------
 * End of File
 *
 * Created By Wu QQ AT lv.yrui@foxmail.com
 *
 *------------------------------------------------------------*/
