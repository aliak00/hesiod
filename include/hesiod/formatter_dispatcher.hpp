#ifndef HESIOD_FORMATTER_DISPATCHER_HPP
#define HESIOD_FORMATTER_DISPATCHER_HPP

#include <type_traits>

namespace hesiod {

template <class...>
using void_t = void;


struct formatter_dispatcher {
    template <class F, class B, class = void>
    struct line {
        static void call(B &) {}
    };

    template <class F, class B>
    struct line<F, B, void_t<decltype(F::line())>> {
        static void call(B &buffer) {
            buffer << F::line();
        }
    };
};

}

#endif
