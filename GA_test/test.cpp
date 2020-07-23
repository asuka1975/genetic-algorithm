//
// Created by hungr on 2020/07/23.
//
#include <algorithm>
#include <numeric>
#include <string>
#include <gtest/gtest.h>
#include "random_generator.h"

namespace genetic {
    std::string crossover(const std::string& d1, const std::string& d2) {
        std::string d = d1;
        for(auto i = 0; i < d1.size(); i++) {
            if(i < d1.size() / 2) d[i] = d2[i];
        }
        return d;
    }

    int crossover(const int& d1, const int& d2) {
        return d1 < d2 ? d2 : d1;
    }
}

#include "genetic.h"

namespace {
    TEST(COMPILE_TEST, GA_CONFIG) {
        genetic::ga_config<std::string> config;
        using individual_t = genetic::ga_config<std::string>::individual_t;
        config.population = 20;
        config.epoch = 100;
        config.fitness_max = 1.0f;
        config.fitness_min = 0.0f;
        config.callback = [](const std::vector<float>& f) {
            return;
        };
        config.select = genetic::roulet<std::string>{};
        config.step = [](const std::vector<individual_t>& d) {
            std::vector<float> f;
            std::transform(d.begin(), d.end(), std::back_inserter(f), [](const individual_t& x) {
                float t = 0.0f;
                for(auto c : std::get<0>(x)) t += std::abs(c - 'a');
                return (25 - t) / 25;
            });
            return f;
        };
        config.scale = [](float x) { return x * x; };
        config.initializer = []() {
            std::string s = "aaaa";
            for(auto& c : s) c += random_generator::random_uniform<int>(0, 25);
            return s;
        };

    }

    TEST(RUNTIME_TEST, CROSSOVER_TEST1) {
        std::tuple<std::string> d1 = std::make_tuple("aaaa");
        std::tuple<std::string> d2 = std::make_tuple("bbbb");
        auto d3 = genetic::crossover(d1, d2);
        ASSERT_EQ("bbaa", std::get<0>(d3));
        auto d4 = genetic::crossover(d2, d1);
        ASSERT_EQ("aabb", std::get<0>(d4));
    }

    TEST(RUNTIME_TEST, CROSSOVER_TEST2) {
        std::tuple<std::string, int> d1 = std::make_tuple("aaaa", 4);
        std::tuple<std::string, int> d2 = std::make_tuple("bbbb", 3);
        auto d3 = genetic::crossover(d1, d2);
        ASSERT_EQ("bbaa", std::get<0>(d3));
        ASSERT_EQ(4, std::get<1>(d3));
        auto d4 = genetic::crossover(d2, d1);
        ASSERT_EQ("aabb", std::get<0>(d4));
        ASSERT_EQ(4, std::get<1>(d4));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}