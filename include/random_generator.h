//
// Created by hungr on 2020/07/21.
//

#ifndef GENETIC_RANDOM_GENERATOR_H
#define GENETIC_RANDOM_GENERATOR_H

#include <limits>
#include <map>
#include <mutex>
#include <random>
#include <thread>
#include <type_traits>

class random_generator {
public:
    template <class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
    static T random();
    template <class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t> = nullptr>
    static T random();

    template <class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t> = nullptr>
    static T random_uniform(T min, T max);
    template <class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t> = nullptr>
    static T random_uniform(T min, T max);

    template <class T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, std::nullptr_t> = nullptr>
    static T random_normal(T mean, T max);
    template <class T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, std::nullptr_t> = nullptr>
    static T random_normal(T mean, T max);
    template <class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t> = nullptr>
    static T random_normal(T mean, T stddev);
private:
    static std::random_device seed_gen;
    static std::map<std::thread::id, std::mt19937_64> thread_own_gens;
    static std::mutex mtx;
};

template<class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t>>
T random_generator::random() {
    return random_uniform<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
T random_generator::random() {
    return random_uniform<T>(0.0, 1.0);
}

template<class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t>>
T random_generator::random_uniform(T min, T max) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    std::uniform_int_distribution<T> dist(min, max);
    auto&& id = std::this_thread::get_id();
    if(thread_own_gens.find(id) == thread_own_gens.end()) {
        thread_own_gens.emplace(id, std::mt19937_64{seed_gen()});
    }
    return dist(thread_own_gens[id]);
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
T random_generator::random_uniform(T min, T max) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    std::uniform_real_distribution<T> dist(min, max);
    auto&& id = std::this_thread::get_id();
    if(thread_own_gens.find(id) == thread_own_gens.end()) {
        thread_own_gens.emplace(id, std::mt19937_64{seed_gen()});
    }
    return dist(thread_own_gens[id]);
}

template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, std::nullptr_t>>
T random_generator::random_normal(T mean, T max) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    if(auto&& m = std::numeric_limits<T>::max() / 2; max >= m) max = m;
    std::binomial_distribution<T> dist(max * 2, 0.5);
    auto&& id = std::this_thread::get_id();
    if(thread_own_gens.find(id) == thread_own_gens.end()) {
        thread_own_gens.emplace(id, std::mt19937_64{seed_gen()});
    }
    return dist(thread_own_gens[id]) - max + mean;
}

template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, std::nullptr_t>>
T random_generator::random_normal(T mean, T max) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    if(auto&& m = std::numeric_limits<T>::max() / 2; max >= m) max = m;
    std::binomial_distribution<T> dist(max * 2, 0.5);
    auto&& id = std::this_thread::get_id();
    if(thread_own_gens.find(id) == thread_own_gens.end()) {
        thread_own_gens.emplace(id, std::mt19937_64{seed_gen()});
    }
    return dist(thread_own_gens[id]) - max + mean;
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
T random_generator::random_normal(T mean, T stddev) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    std::normal_distribution<T> dist(mean, stddev);
    auto&& id = std::this_thread::get_id();
    if(thread_own_gens.find(id) == thread_own_gens.end()) {
        thread_own_gens.emplace(id, std::mt19937_64{seed_gen()});
    }
    return dist(thread_own_gens[id]);
}

#endif //GENETIC_RANDOM_GENERATOR_H
