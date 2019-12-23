/*
 * std::make_unique is an upcoming C++14 feature and thus might not be
 * available on your compiler, even if it is C++11 compliant.
 *
 * You can however easily roll your own implementation, look at:
 * http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3656.htm
 * https://isocpp.org/files/papers/N3656.txt
 *
 * Document number: N3656
 * Date: 2013-04-18
 * Project: Programming Language C++, Library Working Group
 * Reply-to: Stephan T. Lavavej <stl@microsoft.com>
 *
 */
#ifndef __BACKPORT_STD_UNIQUE_PTR_HPP
#define __BACKPORT_STD_UNIQUE_PTR_HPP

// make_unique<T>(args...)
// make_unique<T[]>(n)
// make_unique<T[N]>(args...) = delete

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

#if __cplusplus >= 201402L

// use standard implementation for C++14
using std::make_unique;

#elif __cplusplus >= 201103L

// use open-std implementation for C++11
namespace std {

    template<class T> struct _Unique_if {
        typedef unique_ptr<T> _Single_object;
    };

    template<class T> struct _Unique_if<T[]> {
        typedef unique_ptr<T[]> _Unknown_bound;
    };

    template<class T, size_t N> struct _Unique_if<T[N]> {
        typedef void _Known_bound;
    };

    template<class T, class... Args>
        typename _Unique_if<T>::_Single_object
        make_unique(Args&&... args) {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }

    template<class T>
        typename _Unique_if<T>::_Unknown_bound
        make_unique(size_t n) {
            typedef typename remove_extent<T>::type U;
            return unique_ptr<T>(new U[n]());
    }

    template<class T, class... Args>
        typename _Unique_if<T>::_Known_bound
        make_unique(Args&&...) = delete;

}

#elif __cplusplus >= 199711L

#error "std::make_unique re-implementation not supported for historical C++"

#else

#error "std::make_unique re-implementation not supported for historical C"

#endif
                 
#endif // __BACKPORT_STD_UNIQUE_PTR_HPP
