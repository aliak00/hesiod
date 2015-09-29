#ifndef HESIOD_STREAMS_WRITER_HPP
#define HESIOD_STREAMS_WRITER_HPP

#include <tuple>

namespace hesiod {

template <class StringT, class Tuple, std::size_t N>
struct streams_writer_impl {
    static void write(const StringT &str, const Tuple &tuple) {
        typedef decltype(std::get<N - 1>(tuple)) stream_t;
        stream_t &stream = std::get<N - 1>(tuple);
        stream << str;
        streams_writer_impl<StringT, Tuple, N - 1>::write(str, tuple);
    }
};

template <class StringT, class Tuple>
struct streams_writer_impl<StringT, Tuple, 0> {
    static void write(const StringT &, const Tuple &) {}
};

template <class StringT, class... Args>
struct streams_writer {
    void write(const StringT &str, const std::tuple<Args...> &tuple) {
        streams_writer_impl<StringT, decltype(tuple), sizeof...(Args)>::write(str, tuple);
    }
};


}

#endif
