//
// Created by hungr on 2020/07/22.
//

#ifndef GENETIC_GENOME_H
#define GENETIC_GENOME_H

#include <cstdint>
#include <type_traits>

namespace genetic {
    inline namespace genetic_internal {
        inline constexpr int crossover = 0;
    }
    template <class T, class = void>
    struct is_genome : std::false_type<T> { };

    template <class T, std::enable_if_t<std::is_copy_assignable_v<T> && std::is_invocable_r_v<T, decltype(crossover), T, T>>>
    struct is_genome : std::true_type<T> { };

    template <class T>
    using is_genome_v = is_genome<T>::value;
}

#endif //GENETIC_GENOME_H
