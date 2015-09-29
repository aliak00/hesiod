#ifndef HESIOD_STANDARD_FORMATTER_HPP
#define HESIOD_STANDARD_FORMATTER_HPP

#include <ostream>

#include "hesiod/formatter.hpp"

namespace hesiod {

template <class CharT>
class standard_formatter : public formatter<CharT> {
public:
    static auto line() {
        return std::basic_string<CharT>("\n");
    }
};

}
#endif
