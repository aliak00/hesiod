#ifndef HESIOD_LOGGER_HPP
#define HESIOD_LOGGER_HPP

#include <string>
#include <sstream>
#include <iterator>
#include <iostream>

#include "hesiod/logger_stream_proxy.hpp"
#include "hesiod/standard_formatter.hpp"
#include "hesiod/formatter_dispatcher.hpp"

namespace hesiod {

template <class CharT, class FormatterT = standard_formatter<CharT>>
class logger {
public:

    using stream_t = std::basic_ostream<CharT>;
    using str_t = std::basic_string<CharT>;
    using buffer_t = std::basic_stringstream<CharT>;
    using formatter_t = FormatterT;
    using self_t = logger<CharT, FormatterT>;

    static const CharT format_specifier = static_cast<CharT>('%');

    logger(stream_t &stream)
        : stream_(stream)
    {}

    template <class... Args>
    void write(const str_t &str, Args... args) {
        buffer_t buffer;
        expand_impl(buffer, 0, str, std::forward<Args>(args)...);
        stream_ << buffer.str();
    }

    template <class... Args>
    void writeln(const str_t &str, Args... args) {
        buffer_t buffer;
        expand_impl(buffer, 0, str, std::forward<Args>(args)...);
        formatter_dispatcher::line<self_t>::call(buffer);
        stream_ << buffer.str();
    }

    template <class T>
    auto operator<<(T &&value) {
        return logger_stream_proxy<self_t>(&stream_, std::forward<T>(value));
    }

private:

    std::basic_ostream<CharT> &stream_;

    buffer_t& expand_impl(buffer_t &buffer, std::size_t start_index, const str_t &str) const {
        auto found_index = find_format_specifier(str, start_index);
        if (found_index == std::string::npos) {
            std::copy(std::cbegin(str) + start_index, std::cend(str), std::ostream_iterator<CharT>(buffer));
            return buffer;
        }
        throw std::invalid_argument("Found more specifiers then arguments.");
    }

    template <class Arg, class ...Args>
    buffer_t& expand_impl(buffer_t &buffer, std::size_t start_index, const str_t &str, const Arg &val, const Args&... args) const {
        auto found_index = find_format_specifier(str, start_index);
        if (found_index == std::string::npos) {
            throw std::invalid_argument("Found more arguments then specifiers.");
        }
        auto cit = std::cbegin(str);
        std::copy(cit + start_index, cit + found_index, std::ostream_iterator<CharT>(buffer));
        buffer << val;
        return expand_impl(buffer, found_index + 1, str, std::forward<Args>(args)...);
    }

    auto find_format_specifier(const str_t &str, std::size_t start_index) const {
        auto found_index = str.find(format_specifier, start_index);
        if (found_index == std::string::npos) {
            return std::string::npos;
        }
        if (found_index == 0 || str[found_index - 1] != static_cast<CharT>('\\')) {
            return found_index;
        }
        return find_format_specifier(str, found_index + 1);
    }
};

}

#endif
