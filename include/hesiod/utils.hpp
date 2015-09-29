#ifndef HESIOD_UTILS_HPP
#define HESIOD_UTILS_HPP

#include <type_traits>

namespace hesiod {

template <class...>
using void_t = void;

template <std::size_t N>
using size_c = std::integral_constant<std::size_t, N>;

}


#endif
