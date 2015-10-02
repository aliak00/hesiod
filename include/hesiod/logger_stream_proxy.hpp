#ifndef HESIOD_LOGGER_STREAM_PROXY_HPP
#define HESIOD_LOGGER_STREAM_PROXY_HPP

#include <sstream>
#include <functional>
#include <tuple>
#include <array>

#include "hesiod/formatter_dispatcher.hpp"
#include "hesiod/manipulator.hpp"
#include "hesiod/utils.hpp"

namespace hesiod {

template <typename LoggerT>
class logger_stream_proxy {
public:
    using streams_t = typename LoggerT::streams_t;
    using buffer_t = typename LoggerT::buffer_t;

    template <typename T>
    logger_stream_proxy(streams_t *pstreams, T &&value)
        : pstreams_(pstreams)
    {
        write_to_streams(std::forward<T>(value));
    }

    logger_stream_proxy(const logger_stream_proxy &other)
        : pstreams_(other.pstreams_)
    {}

    template <typename T>
    logger_stream_proxy &operator<<(T &&value) {
        write_to_streams(std::forward<T>(value));
        return *this;
    }

private:

    template <typename T, std::size_t N>
    void write_to_streams(T &&value, size_c<N>) {
        constexpr size_t I = N - 1;
        auto &stream = std::get<I>(*pstreams_);
        forward_value<decltype(stream), T>::call(stream, std::forward<T>(value));
        write_to_streams(std::forward<T>(value), size_c<I>());
    }

    template <typename T>
    void write_to_streams(T &&value, size_c<0>) {}

    template <typename T>
    void write_to_streams(T &&value) {
        write_to_streams(std::forward<T>(value), size_c<std::tuple_size<streams_t>::value>());
    }

    template <typename StreamT, typename T, typename = void>
    struct forward_value {
        static void call(StreamT &stream, T &&value) {
            stream << value;
        }
    };

    template <typename StreamT, typename T>
    struct forward_value<
        StreamT,
        T,
        void_t<
            decltype(std::declval<T>()(std::declval<StreamT>()))
        >
    > {
        static void call(StreamT &stream, T&& callable) {
            callable(stream);
        }
    };

    streams_t *pstreams_;
};


}

#endif
