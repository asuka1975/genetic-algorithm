//
// Created by hungr on 2020/07/23.
//

#ifndef GENETIC_SELECTOR_H
#define GENETIC_SELECTOR_H

#include "ga.h"
#include "random_generator.h"

namespace genetic {
    template <std::size_t I, class... TArgs>
    inline std::tuple_element_t<I, std::tuple<TArgs...>>
    apply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2, const std::tuple<typename TArgs::crossover_config_type...>& config) {
        return std::tuple_element_t<I, std::tuple<TArgs...>>::crossover(std::get<I>(d1), std::get<I>(d2), std::get<I>(config));
    }

    template <class... TArgs>
    struct mapply_wrapper {
        template <class T>
        struct mapply_impl;

        template <class T, T... I>
        struct mapply_impl<std::integer_sequence<T, I...>> {
            static std::tuple<TArgs...> mapply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2, const std::tuple<typename TArgs::crossover_config_type...>& config) {
                return std::make_tuple(apply<I, TArgs...>(d1, d2, config)...);
            }
        };

        static std::tuple<TArgs...> mapply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2, const std::tuple<typename TArgs::crossover_config_type...>& config) {
            return mapply_impl<std::index_sequence_for<TArgs...>>::mapply(d1, d2, config);
        }
    };

    std::size_t roulette_function(const std::vector<std::size_t>& idx, const std::vector<float>& f, float r);

    template <class... TArgs>
    inline std::tuple<TArgs...> crossover(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2, const std::tuple<typename TArgs::crossover_config_type...>& config) {
        return mapply_wrapper<TArgs...>::mapply(d1, d2, config);
    }

    template <class... TArgs>
    struct roulet {
        using individual_t = typename ga<TArgs...>::individual_t;
        std::vector<individual_t> operator()(const std::vector<individual_t>& pop, const std::vector<float>& fitness, const std::tuple<typename TArgs::crossover_config_type...>&);
    };

    template <class... TArgs>
    struct elite {
        using individual_t = typename ga<TArgs...>::individual_t;
        elite(std::uint64_t elitism = 1);
        std::vector<individual_t> operator()(const std::vector<individual_t>& pop, const std::vector<float>& fitness, const std::tuple<typename TArgs::crossover_config_type...>&);
        std::uint64_t elitism;
    };

    template<class... TArgs>
    std::vector<typename ga<TArgs...>::individual_t>
    roulet<TArgs...>::operator()(const std::vector<typename ga<TArgs...>::individual_t> &pop,
                                 const std::vector<float> &fitness,
                                 const std::tuple<typename TArgs::crossover_config_type...>& config) {
        std::vector<individual_t> p(pop.size());

        std::vector<std::size_t> idx(pop.size());
        std::sort(idx.begin(), idx.end(), [&fitness](auto i, auto j) { return fitness[i] > fitness[j]; });

        for(auto i = 0; i < p.size(); i++) {
            auto k = roulette_function(idx, fitness, random_generator::random<float>());
            auto l = roulette_function(idx, fitness, random_generator::random<float>());
            p[i] = genetic::crossover(pop[k], pop[l], config);
        }
        return p;
    }

    template <class... TArgs>
    elite<TArgs...>::elite(std::uint64_t elitism) : elitism(elitism) { }

    template <class... TArgs>
    std::vector<typename ga<TArgs...>::individual_t>
    elite<TArgs...>::operator()(const std::vector<typename ga<TArgs...>::individual_t>& pop,
                                const std::vector<float>& fitness,
                                const std::tuple<typename TArgs::crossover_config_type...>& config) {
        std::vector<individual_t> p(pop.size());

        std::vector<std::size_t> idx(pop.size());
        std::sort(idx.begin(), idx.end(), [&fitness](auto i, auto j) { return fitness[i] > fitness[j]; });

        for(std::size_t i = 0; i < elitism && i < p.size(); i++) p[i] = pop[idx[i]];
        for(std::size_t i = elitism; i < p.size(); i++) {
            auto k = roulette_function(idx, fitness, random_generator::random<float>());
            auto l = roulette_function(idx, fitness, random_generator::random<float>());
            p[i] = genetic::crossover(pop[k], pop[l], config);
        }
        return p;
    }
}

#endif //GENETIC_SELECTOR_H
