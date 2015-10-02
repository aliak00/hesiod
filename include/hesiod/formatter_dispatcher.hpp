#ifndef HESIOD_FORMATTER_DISPATCHER_HPP
#define HESIOD_FORMATTER_DISPATCHER_HPP

#include "hesiod/utils.hpp"

namespace hesiod {

// checks if formatter has implemented the required functions and
// then calls them, else does nothing
struct formatter_dispatcher {
    template <typename StreamT, typename = void>
    struct line {
        static void call() {}
    };

    template <typename StreamT>
    struct line<StreamT, void_t<decltype(StreamT::formatter_t::line())>> {
        static auto call() {
            return StreamT::formatter_t::line();
        }
    };
};

}

#endif
