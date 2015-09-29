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

template <class LoggerT>
class logger_stream_proxy {
public:
    using streams_t = typename LoggerT::streams_t;
    using buffer_t = typename LoggerT::buffer_t;
    using array_t = std::array<buffer_t, std::tuple_size<streams_t>::value>;

    template <class T>
    logger_stream_proxy(streams_t *pstreams, T&& value)
        : pstreams_(pstreams)
    {
        write_value(std::forward<T>(value));
    }

    logger_stream_proxy(const logger_stream_proxy &other)
        : pstreams_(other.pstreams_)
    {}

    ~logger_stream_proxy() {
        write_buffers_to_streams();
    }

    template <class T>
    logger_stream_proxy &operator<<(T&& value) {
        write_value(std::forward<T>(value));
        return *this;
    }

private:

    template <std::size_t N>
    void write_buffers_to_streams(size_c<N>) {
        auto &stream = std::get<N - 1>(*pstreams_);
        stream << buffers_[N - 1].str();
        write_buffers_to_streams(size_c<N - 1>());
    };

    void write_buffers_to_streams(size_c<0>) {}

    void write_buffers_to_streams() {
        write_buffers_to_streams(size_c<std::tuple_size<streams_t>::value>());
    }

    template <class T>
    void write_value(T&& value) {
        for (auto & buffer: buffers_) {
            forward_value<T>::call(buffer, std::forward<T>(value));
        }
    }

    template <class T, class = void>
    struct forward_value {
        static void call(buffer_t &buffer, const T &value) {
            buffer << value;
        }
    };

    template <class T>
    struct forward_value<T,
        void_t<
            decltype(std::declval<T>().operator()(std::declval<buffer_t&>()))
        >> {
        static void call(buffer_t &buffer, T callable) {
            callable(buffer);
        }
    };

    streams_t *pstreams_;
    array_t buffers_;
};


}

#endif
