#ifndef HESIOD_STANDARD_FORMATTER_HPP
#define HESIOD_STANDARD_FORMATTER_HPP

#include <ostream>

namespace hesiod {

template <class CharT>
class standard_formatter {
public:
    static auto line() {
        return std::basic_string<CharT>("\n");
    }
};

}
#endif
