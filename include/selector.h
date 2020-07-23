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
    apply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2) {
        return crossover(std::get<I>(d1), std::get<I>(d2));
    }

    template <std::size_t... I, class... TArgs>
    inline std::tuple<TArgs...> mapply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2) {
        return std::make_tuple(apply<I, TArgs...>(d1, d2)...);
    }

    template <class... TArgs>
    struct mapply_wrapper {
        template <class T>
        struct mapply_impl;

        template <class T, T... I>
        struct mapply_impl<std::integer_sequence<T, I...>> {
            static std::tuple<TArgs...> mapply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2) {
                return std::make_tuple(apply<I, TArgs...>(d1, d2)...);
            }
        };

        static std::tuple<TArgs...> mapply(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2) {
            return mapply_impl<std::index_sequence_for<TArgs...>>::mapply(d1, d2);
        }
    };

    template <class... TArgs>
    inline std::tuple<TArgs...> crossover(const std::tuple<TArgs...>& d1, const std::tuple<TArgs...>& d2) {
        return mapply_wrapper<TArgs...>::mapply(d1, d2);
    }

    template <class... TArgs>
    struct roulet {
        using individual_t = typename ga<TArgs...>::individual_t;
        std::vector<individual_t> operator()(const std::vector<individual_t>& pop, const std::vector<float>& fitness);
    };

    template <class... TArgs>
    struct elite {
        using individual_t = typename ga<TArgs...>::individual_t;
        elite(std::uint64_t elitism = 1);
        std::vector<individual_t> operator()(const std::vector<individual_t>& pop, const std::vector<float>& fitness);
        std::uint64_t elitism;
    };

    template<class... TArgs>
    std::vector<typename ga<TArgs...>::individual_t>
    roulet<TArgs...>::operator()(const std::vector<typename ga<TArgs...>::individual_t> &pop,
                                 const std::vector<float> &fitness) {
        std::vector<individual_t> p(pop.size());

        //Instead of `pop` and `fitness`, vector of index and fitness pair is sorted.
        std::vector<std::pair<std::size_t, float>> f(pop.size());
        for(auto i = 0; i < f.size(); i++) {
            f[i] = std::make_pair(i, fitness[i]);
        }
        std::sort(f.begin(), f.end(), [](auto const& p1, auto const& p2) { return p1.second > p2.second; });

        //return a selected index
        auto rlt = [&f](float r) {
            float id = 0.0f;
            for(const auto& t : f) {
                if(id <= r && r < (id += t.second)) {
                    return t.first;
                }
            }
            return 0lu;
        };

        for(auto i = 0; i < p.size(); i++) {
            auto k = rlt(random_generator::random<float>());
            auto l = rlt(random_generator::random<float>());
            p[i] = genetic::crossover(pop[k], pop[l]);
        }
        return p;
    }

    template <class... TArgs>
    elite<TArgs...>::elite(std::uint64_t elitism) : elitism(elitism) { }

    template <class... TArgs>
    std::vector<typename ga<TArgs...>::individual_t>
    elite<TArgs...>::operator()(const std::vector<typename ga<TArgs...>::individual_t>& pop,
                                const std::vector<float>& fitness) {
        std::vector<individual_t> p(pop.size());

        //Instead of `pop` and `fitness`, vector of index and fitness pair is sorted.
        std::vector<std::pair<std::size_t, float>> f(pop.size());
        for(auto i = 0; i < f.size(); i++) {
            f[i] = std::make_pair(i, fitness[i]);
        }
        std::sort(f.begin(), f.end(), [](auto const& p1, auto const& p2) { return p1.second > p2.second; });

        auto rlt = [&f](float r) {
            float id = 0.0f;
            for(const auto& t : f) {
                if(id <= r && r < (id += t.second)) {
                    return t.first;
                }
            }
            return 0lu;
        };

        for(auto i = 0; i < elitism && i < p.size(); i++) p[i] = pop[f[i].first];
        for(auto i = elitism; i < p.size(); i++) {
            auto k = rlt(random_generator::random<float>());
            auto l = rlt(random_generator::random<float>());
            p[i] = genetic::crossover(pop[k], pop[l]);
        }
        return p;
    }
}

#endif //GENETIC_SELECTOR_H
