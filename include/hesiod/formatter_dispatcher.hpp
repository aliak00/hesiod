#ifndef HESIOD_FORMATTER_DISPATCHER_HPP
#define HESIOD_FORMATTER_DISPATCHER_HPP

#include <type_traits>

namespace hesiod {

template <class...>
using void_t = void;

template <class F, class B, class = void>
struct formatter_dispatcher {
    static void line(B &) {}
};

template <class F, class B>
struct formatter_dispatcher<F, B, void_t<decltype(F::line())>> {
    static void line(B &buffer) {
        buffer << F::line();
    }
};


}

#endif
