#ifndef HESIOD_MANIPULATOR_HPP
#define HESIOD_MANIPULATOR_HPP

#include "hesiod/logger.hpp"
#include "hesiod/formatter_dispatcher.hpp"

namespace hesiod {


template <class LoggerT>
struct endl_m {
    void operator()(typename LoggerT::buffer_t &buffer) {
        formatter_dispatcher::line<LoggerT>::call(buffer);
    }
};

}


#endif
