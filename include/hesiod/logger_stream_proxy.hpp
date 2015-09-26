#ifndef HESIOD_LOGGER_STREAM_PROXY_HPP
#define HESIOD_LOGGER_STREAM_PROXY_HPP

#include <sstream>

#include "hesiod/formatter_dispatcher.hpp"

namespace hesiod {

template <class StreamT, class BufferT, class FormatterT>
class logger_stream_proxy {
public:
    template <class T>
    logger_stream_proxy(StreamT *pstream, T&& value)
        : pstream_(pstream)
    {
        buffer_ << value;
    }

    logger_stream_proxy(logger_stream_proxy &&rval)
        : pstream_(rval.pstream_) 
    {
        rval.pstream_ = nullptr;
    }

    ~logger_stream_proxy() {
        if (pstream_) {
            formatter_dispatcher::line<FormatterT, BufferT>::call(buffer_);
            *pstream_ << buffer_.str();
        }
    }

    template <class T>
    logger_stream_proxy &operator<<(const T &value) {
        buffer_ << value;
        return *this;
    }

private:
    StreamT *pstream_;
    BufferT buffer_;
};


}

#endif
