#ifndef YRUI_DETAIL_PIMPL_H_
#define YRUI_DETAIL_PIMPL_H_

#include <memory> // for std::shared_ptr
#include <utility> // for std::hash

#include "foundation/contract.h"

namespace yrui {
namespace aux {
    namespace detail {
        template <class Impl> struct value_semantics_ptr {
            using this_type = value_semantics_ptr;

            ~value_semantics_ptr() noexcept { reset(nullptr); }

            constexpr value_semantics_ptr() noexcept = default;

            constexpr value_semantics_ptr(Impl* p) noexcept
                : impl_(p)
            {
            }

            value_semantics_ptr(const this_type& that)
                : impl_(that.impl_ ? new Impl(*that.impl_) : nullptr)
            {
            }

            value_semantics_ptr(this_type&& that) noexcept
                : impl_(that.impl_)
            {
                that.impl_ = nullptr;
            }

            this_type& operator=(const this_type& that)
            {
                auto tmp = that;

                swap(tmp);

                return *this;
            }

            this_type&& operator=(this_type&& that) noexcept
            {
                reset(that.impl_);
                that.impl_ = nullptr;
            }

            void reset(Impl* p = nullptr) noexcept
            {
                if (impl_) {
                    delete impl_;
                }

                impl_ = p;
            }

            void swap(value_semantics_ptr& that) noexcept { std::swap(impl_, that.impl_); }

            Impl* get() noexcept { return impl_; }
            const Impl* get() const noexcept { return impl_; }

            Impl& operator*() noexcept { return *impl_; }

            const Impl& operator*() const noexcept { return *impl_; }

            explicit operator bool() const { return !!impl_; }

        private:
            Impl* impl_ = nullptr;
        };
    } // of namespace yrui::aux::detail

    template <class Interface> struct Pimpl {
        struct implementation;

        template <template <class> class> struct Pimpl_base;

        using pointer_semantics = Pimpl_base<std::shared_ptr>;
        using value_sementics = Pimpl_base<detail::value_semantics_ptr>;
    };

    template <class Interface> template <template <class> class Manager> struct Pimpl<Interface>::Pimpl_base {
        using pimpl_base_type = Pimpl_base;
        using this_type = Pimpl_base;
        using impl_type = Pimpl<Interface>::implementation;
        using managed_type = Manager<impl_type>;

    public:
        explicit operator bool() const noexcept { return !!impl_; }

        friend bool operator==(const this_type& x, const this_type& y) { return x.impl_ == y.impl_; }

        friend bool operator!=(const this_type& x, const this_type& y) { return !(x == y); }

    public:
        void swap(const this_type& that) noexcept(noexcept(that.swap(that.impl_))) { impl_.swap(that.impl_); }

    protected:
        constexpr Pimpl_base() noexcept = default;

        constexpr explicit Pimpl_base(std::nullptr_t) noexcept
            : Pimpl_base()
        {
        }

        template <class... Args>
        explicit Pimpl_base(Args&&... args)
            : impl_(new impl_type(std::forward<Args>(args)...))
        {
        }

    protected:
        impl_type& impl() noexcept
        {
            YPRE_CONDITION(impl_);

            return *impl_;
        }

        const impl_type& impl() const noexcept
        {
            YPRE_CONDITION(impl_);

            return *impl_;
        }

    private:
        friend struct std::hash<this_type>;

    private:
        managed_type impl_;
    };
} // of namespace aux
} // of namespace yrui

#endif // YRUI_DETAIL_PIMPL_H_
