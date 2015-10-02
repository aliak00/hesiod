#ifndef HESIOD_LOGGER_HPP
#define HESIOD_LOGGER_HPP

#include <string>
#include <sstream>
#include <iterator>
#include <iostream>
#include <tuple>

#include "hesiod/logger_stream_proxy.hpp"
#include "hesiod/manipulator.hpp"

namespace hesiod {

template <typename CharT, typename ...StreamTs>
class logger {
public:

    static const CharT format_specifier = static_cast<CharT>('%');

    using streams_t = std::tuple<StreamTs...>;
    using self_t = logger<CharT, StreamTs...>;

    using buffer_t = std::basic_stringstream<CharT>;
    using string_t = std::basic_string<CharT>;

    logger(StreamTs &&...streams)
        : streams_(std::make_tuple(streams...))
    {}

    template <typename ...Args>
    void write(const string_t &str, Args ...args) {
        buffer_t buffer;
        expand_args(buffer, 0, str, std::forward<Args>(args)...);
        write_to_streams(buffer.str());
    }

    template <typename ...Args>
    void writeln(const string_t &str, Args... args) {
        buffer_t buffer;
        expand_args(buffer, 0, str, std::forward<Args>(args)...);
        write_to_streams(buffer.str());
        logger_stream_proxy<self_t>(&streams_, endl);
    }

    template <typename T>
    auto operator<<(T &&value) {
        return logger_stream_proxy<self_t>(&streams_, std::forward<T>(value));
    }

private:

    streams_t streams_;

    template <std::size_t N>
    void write_to_streams(const string_t &str, size_c<N>) {
        auto &stream = std::get<N - 1>(streams_);
        stream << str;
        write_to_streams(str, size_c<N - 1>());
    }

    void write_to_streams(const string_t &, size_c<0>) {}

    void write_to_streams(const string_t &str) {
        write_to_streams(str, size_c<std::tuple_size<streams_t>::value>());
    }

    // no specifiers or no more specifiers left
    buffer_t& expand_args(buffer_t &buffer, std::size_t start_index, const string_t &str) const {
        auto found_index = find_format_specifier(str, start_index);
        if (found_index == std::string::npos) {
            std::copy(std::cbegin(str) + start_index, std::cend(str), std::ostream_iterator<CharT>(buffer));
            return buffer;
        }
        throw std::invalid_argument("Found more specifiers than arguments.");
    }

    template <typename Arg, typename ...Args>
    buffer_t& expand_args(buffer_t &buffer, std::size_t start_index, const string_t &str, const Arg &val, const Args&... args) const {
        auto found_index = find_format_specifier(str, start_index);
        if (found_index == std::string::npos) {
            throw std::invalid_argument("Found more arguments than specifiers.");
        }
        auto cit = std::cbegin(str);
        std::copy(cit + start_index, cit + found_index, std::ostream_iterator<CharT>(buffer));
        buffer << val;
        return expand_args(buffer, found_index + 1, str, std::forward<Args>(args)...);
    }

    auto find_format_specifier(const string_t &str, std::size_t start_index) const {
        auto found_index = str.find(format_specifier, start_index);
        if (found_index == std::string::npos) {
            return std::string::npos;
        }
        // counts if previous char is not an escape
        if (found_index == 0 || str[found_index - 1] != static_cast<CharT>('\\')) {
            return found_index;
        }
        return find_format_specifier(str, found_index + 1);
    }
};

template <typename CharT, typename ...StreamTs>
auto make_logger(StreamTs &&...streams) {
    return logger<CharT, StreamTs...>(std::forward<StreamTs>(streams)...);
}


}

#endif
