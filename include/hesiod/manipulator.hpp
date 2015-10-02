#ifndef HESIOD_MANIPULATOR_HPP
#define HESIOD_MANIPULATOR_HPP

#include <type_traits>

#include "hesiod/logger.hpp"
#include "hesiod/formatter_dispatcher.hpp"
#include "hesiod/utils.hpp"

namespace hesiod {

template <typename S, typename T, typename = void>
struct maybe_stream_impl {
    static void call(S&, T&&) {
    }
};

template <typename S, typename T>
struct maybe_stream_impl<
    S,
    T,
    void_t<decltype(std::declval<S&>() << std::declval<T>()())>
> {
    static void call(S &s, T &&t) {
        s << t();
    }
};

template <typename S, typename T>
void maybe_stream(S& s, T &&t) {
    maybe_stream_impl<S, T>::call(s, std::forward<T>(t));
}

struct endl_tag {
    template <typename StreamT>
    void operator()(StreamT &stream) {
        maybe_stream(stream, formatter_dispatcher::line<StreamT>::call);
    }
} endl;

}


#endif
