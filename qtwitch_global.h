/*
 * Copyright © 2019 Andrew Penkrat <contact.aldrog@gmail.com>
 *
 * This file is part of QTwitch.
 *
 * QTwitch is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QTwitch is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with QTwitch.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef QTWITCH_GLOBAL_H
#define QTWITCH_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QMetaType>
#include <memory>

#if defined(QTWITCH_LIBRARY)
#  define QTWITCHSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTWITCHSHARED_EXPORT Q_DECL_IMPORT
#endif

Q_DECLARE_SMART_POINTER_METATYPE(std::shared_ptr)
Q_DECLARE_SMART_POINTER_METATYPE(std::unique_ptr)

#if __cplusplus < 201402L
namespace std {
template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
}
#endif

#if __cplusplus < 201703L
namespace std {
// Partial inefficient implementation of std::optional
template<typename T>
class optional
{
    T val;
    bool hasVal;
public:
    optional() //-V730
        : hasVal(false)
    {}
    optional(T val_)
        : val(std::move(val_)), hasVal(true)
    {}
    optional(const optional &) = default;
    optional(optional &&) noexcept = default;

    optional &operator= (const optional &) = default;
    optional &operator= (optional &&) noexcept = default;

    const T& operator* () const {
        return val;
    }
    T& operator* () {
        return val;
    }
    operator bool() const {
        return hasVal;
    }

    void reset() {
        hasVal = false;
    }
};

// The same goes about string_view
class string_view
{
    const char *viewStart;
    size_t viewSize;
public:
    constexpr string_view() noexcept
        : viewStart(nullptr), viewSize(0)
    {}
    constexpr string_view(const string_view&) noexcept = default;

    constexpr string_view(const char *start_, size_t size_)
        : viewStart(start_), viewSize(size_)
    {}
    template<size_t N>
    constexpr string_view(const char (&s)[N])
        : viewStart(&s[0]), viewSize(N-1)
    {}

    size_t size() const { return viewSize; }
    const char *data() const { return viewStart; }
    bool empty() const { return viewSize == 0; }

    string_view
    substr(size_t __pos, size_t __n = -1) const
    {
        const size_t __rlen = std::min(__n, viewSize - __pos);
        return string_view{viewStart + __pos, __rlen};
    }

    int
    compare(string_view __str) const noexcept
    {
        const size_t __rlen = std::min(this->viewSize, __str.viewSize);
        int __ret = char_traits<char>::compare(this->viewStart, __str.viewStart, __rlen);
        if (__ret == 0) {
            const ptrdiff_t __diff = this->viewSize - __str.viewSize;
            if (__diff > std::numeric_limits<int>::max())
              return std::numeric_limits<int>::max();
            if (__diff < std::numeric_limits<int>::min())
              return std::numeric_limits<int>::min();
            __ret = static_cast<int>(__diff);
        }
        return __ret;
    }

    int
    compare(size_t __pos1, size_t __n1, string_view __str) const
    { return this->substr(__pos1, __n1).compare(__str); }

    int
    compare(size_t __pos1, size_t __n1,
            string_view __str, size_t __pos2, size_t __n2) const
    {
        return this->substr(__pos1, __n1).compare(__str.substr(__pos2, __n2));
    }

    int
    compare(size_t __pos1, size_t __n1,
            const char* __str, size_t __n2) const noexcept(false)
    {
    return this->substr(__pos1, __n1)
           .compare(string_view(__str, __n2));
    }

    size_t
    find(char __c, size_t __pos = 0) const noexcept
    {
        return std::find(viewStart + __pos, viewStart + viewSize, __c) - viewStart;
    }

    void
    remove_prefix(size_t __n) noexcept
    {
        __glibcxx_assert(this->_M_len >= __n);
        viewStart += __n;
        viewSize -= __n;
    }

    void
    remove_suffix(size_t __n) noexcept
    { viewSize -= __n; }
};
inline bool
operator==(string_view __x,
           string_view __y) noexcept
{ return __x.size() == __y.size() && __x.compare(__y) == 0; }
}
#endif

#endif // QTWITCH_GLOBAL_H
