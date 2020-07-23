//
// Created by hungr on 2020/07/22.
//

#ifndef GENETIC_GENOME_H
#define GENETIC_GENOME_H

#include <cstdint>
#include <type_traits>

namespace genetic {
    inline namespace genetic_internal {
        struct crossover_type {};

        crossover_type crossover(const crossover_type &, const crossover_type &) { return crossover_type{}; }
    }

    template<class T, class = void>
    struct is_callable_crossover : std::false_type {};

    template<class T>
    struct is_callable_crossover<T, std::void_t<decltype(crossover(std::declval<T>(), std::declval<T>()))>>
            : std::true_type {
    };

    template<class T, class = void>
    struct is_genome : public std::false_type {};

    template<class T>
    struct is_genome<T, std::enable_if_t<
            std::is_copy_assignable_v<T> &&
            std::is_copy_constructible_v<T> &&
            std::is_default_constructible_v<T>&&
            is_callable_crossover<T>::value>>
            : public std::true_type {
    };

    template<class T>
    inline constexpr bool is_genome_v = is_genome<T>::value;
}
#endif //GENETIC_GENOME_H
