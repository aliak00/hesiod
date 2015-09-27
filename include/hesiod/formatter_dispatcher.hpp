#ifndef HESIOD_FORMATTER_DISPATCHER_HPP
#define HESIOD_FORMATTER_DISPATCHER_HPP

#include "hesiod/utils.hpp"

namespace hesiod {

// checks if formatter has implemented the required functions and
// then calls them, else does nothing
struct formatter_dispatcher {
    template <class LoggerT, class = void>
    struct line {
        static void call(typename LoggerT::buffer_t &) {}
    };

    template <class LoggerT>
    struct line<LoggerT, void_t<decltype(LoggerT::formatter_t::line())>> {
        static void call(typename LoggerT::buffer_t &buffer) {
            buffer << LoggerT::formatter_t::line();
        }
    };
};

}

#endif
