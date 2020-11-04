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
    static thread_local std::mt19937_64 mt;
};

template<class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t>>
inline T random_generator::random() {
    return random_uniform<T>(std::numeric_limits<T>::lowest(), std::numeric_limits<T>::max());
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
inline T random_generator::random() {
    return random_uniform<T>(0.0, 1.0);
}

template<class T, std::enable_if_t<std::is_integral_v<T>, std::nullptr_t>>
inline T random_generator::random_uniform(T min, T max) {
    std::uniform_int_distribution<T> dist(min, max);
    return dist(mt);
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
inline T random_generator::random_uniform(T min, T max) {
    std::uniform_real_distribution<T> dist(min, max);
    return dist(mt);
}

template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>, std::nullptr_t>>
inline T random_generator::random_normal(T mean, T max) {
    if(auto&& m = std::numeric_limits<T>::max() / 2; max >= m) max = m;
    std::binomial_distribution<T> dist(max * 2, 0.5);
    return dist(mt) - max + mean;
}

template<class T, std::enable_if_t<std::is_integral_v<T> && std::is_unsigned_v<T>, std::nullptr_t>>
inline T random_generator::random_normal(T mean, T max) {
    if(auto&& m = std::numeric_limits<T>::max() / 2; max >= m) max = m;
    std::binomial_distribution<T> dist(max * 2, 0.5);
    return dist(mt) - max + mean;
}

template<class T, std::enable_if_t<std::is_floating_point_v<T>, std::nullptr_t>>
inline T random_generator::random_normal(T mean, T stddev) {
    std::normal_distribution<T> dist(mean, stddev);
    return dist(mt);
}

#endif //GENETIC_RANDOM_GENERATOR_H
