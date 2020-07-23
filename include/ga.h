//
// Created by hungr on 2020/07/22.
//

#ifndef GENETIC_GA_H
#define GENETIC_GA_H

#include <cstdint>
#include <functional>
#include <type_traits>
#include "genome.h"

namespace genetic {
    template <bool...> struct bool_pack;
    template <bool... b>
    using all_true = std::is_same<bool_pack<b..., true>, bool_pack<true, b...>>;
    template <bool... b>
    inline constexpr bool all_true_v = all_true<b...>::value;

    template <class... TArgs>
    struct ga_config {
        using individual_t = std::tuple<TArgs...>;
        std::uint64_t population;
        std::uint64_t epoch;
        float fitness_max;
        float fitness_min;
        std::function<void(const std::vector<float>&)> callback;
        std::function<std::vector<individual_t>(const std::vector<individual_t>&, const std::vector<float>&)> select;
        std::function<std::vector<float>(const std::vector<individual_t>&)> step;
        std::function<float(float)> scale;
        std::function<individual_t()> initializer;
        std::vector<std::pair<float, std::function<void(individual_t&)>>> mutates;
        std::vector<std::pair<float, std::function<void(float, individual_t&)>>> node_mutates;
        ga_config();
    };

    template <class... TArgs>
    struct ga {
        using individual_t = std::tuple<TArgs...>;
        ga();
    };

    template <class... TArgs>
    inline ga<TArgs...>::ga() {
        using individual_t = std::tuple<TArgs...>;
        static_assert(all_true_v<is_genome_v<TArgs>...>, "These genomes include genomes which can't crossover.");
    }


    template <class... TArgs>
    inline ga_config<TArgs...>::ga_config() {
        static_assert(all_true_v<is_genome_v<TArgs>...>, "These genomes include genomes which can't crossover.");
        population = 0;
        epoch = 0;
        fitness_max = 1.0f;
        fitness_min = 0.0f;
        callback = [](const std::vector<float>&) -> void {};
        select = [](const std::vector<typename ga_config<TArgs...>::individual_t>&,
                    const std::vector<float>&) {
            return std::vector<typename ga_config<TArgs...>::individual_t>{};
        };
        step = [](const std::vector<typename ga_config<TArgs...>::individual_t>&) {
            return std::vector<float>{};
        };
        scale = [](float x) { return x; };
        initializer = []() {
            return typename ga_config<TArgs...>::individual_t{};
        };
    }
}

#endif //GENETIC_GA_H
