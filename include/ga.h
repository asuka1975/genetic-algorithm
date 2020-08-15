//
// Created by hungr on 2020/07/22.
//

#ifndef GENETIC_GA_H
#define GENETIC_GA_H

#include <algorithm>
#include <numeric>
#include <cstdint>
#include <functional>
#include <limits>
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
        using expression_t = std::tuple<typename TArgs::expression_t...>;
        std::uint64_t population;
        std::uint64_t epoch;
        float fitness_max;
        float fitness_min;
        std::uint32_t save = 0;
        std::function<void(const std::vector<expression_t>&, const std::vector<float>&)> callback;
        std::function<std::vector<individual_t>(const std::vector<individual_t>&, const std::vector<float>&)> select;
        std::tuple<std::function<typename TArgs::expression_t(const TArgs&)>...> express;
        std::function<std::vector<float>(const std::vector<expression_t>&)> step;
        std::function<float(float)> scale;
        std::function<individual_t()> initializer;
        std::vector<std::pair<float, std::function<void(individual_t&)>>> mutates;
        std::vector<std::pair<float, std::function<void(float, individual_t&)>>> node_mutates;
        ga_config();
    };

    template <class... TArgs>
    struct ga {
        using individual_t = std::tuple<TArgs...>;
        ga(const ga_config<TArgs...>& config);
        void run();
        const ga_config<TArgs...> config;
    private:
        std::vector<individual_t> population;
        template <class T> class expression_impl;
        template <std::size_t... I>
        struct expression_impl<std::index_sequence<I...>> {
            static typename ga_config<TArgs...>::expression_t express(const ga_config<TArgs...>& g, const typename ga_config<TArgs...>::individual_t& d);
        };
        typename ga_config<TArgs...>::expression_t express(const typename ga_config<TArgs...>::individual_t& d) const;
    };

    template <class... TArgs>
    inline ga<TArgs...>::ga(const ga_config<TArgs...>& config) : config(config) {
        static_assert(all_true_v<is_genome_v<TArgs>...>, "These genomes include genomes which can't crossover.");
        population.resize(config.population);
        for(auto& d : population) d = config.initializer();
    }

    template <class... TArgs>
    inline void ga<TArgs...>::run() {
        for(auto i = 0; i < config.epoch; i++) {
            std::vector<typename ga_config<TArgs...>::expression_t> e(population.size());
            std::transform(population.begin(), population.end(), e.begin(), [&g=*this](const auto& x) {
                return g.express(x);
            });
            auto f = config.step(e);
            std::for_each(f.begin(), f.end(), [
                    scale = this->config.scale,
                    fitness_max = this->config.fitness_max,
                    fitness_min = this->config.fitness_min]
                    (auto& m) {
                m -= fitness_min;
                m = scale(m / (fitness_max - fitness_min));
            });
            config.callback(e, f);
            auto sum = std::accumulate(f.begin(), f.end(), 0.0f);
            if(std::abs(sum) <= std::numeric_limits<float>::min()) sum = 1.0f;
            std::for_each(f.begin(), f.end(), [&sum](auto& m) { m /= sum; });

            population = config.select(population, f);
            std::uint32_t j = 0;
            for(auto& d : population) {
                if(j < config.save) {
                    j++;
                    continue;
                }
                for(auto [rate, mutate] : config.mutates) {
                    if(random_generator::random<float>() < rate) {
                        mutate(d);
                    }
                }
                for(auto [rate, mutate] : config.node_mutates) {
                    mutate(rate, d);
                }
            }
        }
    }

    template <class... TArgs>
    template <std::size_t... I>
    typename ga_config<TArgs...>::expression_t ga<TArgs...>::expression_impl<std::index_sequence<I...>>::
    express(const ga_config<TArgs...>& g, const typename ga_config<TArgs...>::individual_t& d) {
        return std::make_tuple(std::get<I>(g.express)(std::get<I>(d))...);
    }

    template <class... TArgs>
    inline typename ga_config<TArgs...>::expression_t ga<TArgs...>::express(const typename ga_config<TArgs...>::individual_t &d) const {
        return expression_impl<std::make_index_sequence<std::tuple_size_v<std::tuple<TArgs...>>>>::express(config, d);
    }

    template <class... TArgs>
    inline ga_config<TArgs...>::ga_config() {
        static_assert(all_true_v<is_genome_v<TArgs>...>, "These genomes include genomes which can't crossover.");
        population = 0;
        epoch = 0;
        fitness_max = 1.0f;
        fitness_min = 0.0f;
        callback = [](const std::vector<expression_t>&, const std::vector<float>&) -> void {};
        select = [](const std::vector<typename ga_config<TArgs...>::individual_t>&,
                    const std::vector<float>&) {
            return std::vector<typename ga_config<TArgs...>::individual_t>{};
        };
        step = [](const std::vector<typename ga_config<TArgs...>::expression_t>&) {
            return std::vector<float>{};
        };
        scale = [](float x) { return x; };
        initializer = []() {
            return typename ga_config<TArgs...>::individual_t{};
        };
    }
}

#endif //GENETIC_GA_H
