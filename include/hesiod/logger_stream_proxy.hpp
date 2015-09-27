#ifndef HESIOD_LOGGER_STREAM_PROXY_HPP
#define HESIOD_LOGGER_STREAM_PROXY_HPP

#include <sstream>
#include <functional>

#include "hesiod/formatter_dispatcher.hpp"
#include "hesiod/manipulator.hpp"
#include "hesiod/utils.hpp"

namespace hesiod {

template <class LoggerT>
class logger_stream_proxy {
public:
    using stream_t = typename LoggerT::stream_t;
    using buffer_t = typename LoggerT::buffer_t;
    using formatter_t = typename LoggerT::formatter_t;

    template <class T>
    logger_stream_proxy(stream_t *pstream, T&& value)
        : pstream_(pstream)
    {
        forward_value<T>::call(buffer_, std::forward<T>(value));
    }

    logger_stream_proxy(const logger_stream_proxy &other)
        : pstream_(other.pstream_)
    {}

    ~logger_stream_proxy() {
        *pstream_ << buffer_.str();
    }

    template <class T>
    logger_stream_proxy &operator<<(T&& value) {
        // either streams value in to buffer, or if value is a buffer
        // manipulator, calls the manipulator with buffer as the argument
        forward_value<T>::call(buffer_, std::forward<T>(value));
        return *this;
    }

private:

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

    stream_t *pstream_;
    buffer_t buffer_;
};


}

#endif
