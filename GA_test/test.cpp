//
// Created by hungr on 2020/07/23.
//
#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <gtest/gtest.h>
#include "random_generator.h"
#include "genetic.h"

class string_genome {
public:
    static string_genome crossover(const string_genome &d1, const string_genome &d2) {
        string_genome d = d1;
        for (auto i = 0; i < d1.data.size(); i++) {
            if (i < d1.data.size() / 2) d.data[i] = d2.data[i];
        }
        return d;
    }
    std::string data;
};

class int_genome {
public:
    static int_genome crossover(const int_genome& d1, const int_genome& d2) {
        int_genome d { d1.data < d2.data ? d2.data : d1.data };
        return d;
    }
    int data;
};


namespace {
    TEST(COMPILE_TEST, GA) {
        genetic::ga_config<string_genome> config;
        using individual_t = genetic::ga_config<string_genome>::individual_t;
        config.population = 20;
        config.epoch = 200;
        config.fitness_max = 1.0f;
        config.fitness_min = 0.0f;
        config.callback = [](const std::vector<individual_t>& d, const std::vector<float>& f) {
            auto iter = std::max_element(f.begin(), f.end());
            std::cout << std::get<0>(d[iter - f.begin()]).data << ":" <<
                    *iter << " average:" << std::accumulate(f.begin(), f.end(), 0.0f) / f.size() << std::endl;
        };
        config.select = genetic::roulet<string_genome>{};
        config.step = [](const std::vector<individual_t>& d) {
            std::vector<float> f;
            std::transform(d.begin(), d.end(), std::back_inserter(f), [](const individual_t& x) {
                float t = 0.0f;
                for(auto c : std::get<0>(x).data) t += std::abs(c - 'a');
                return (100 - t) / 100;
            });
            return f;
        };
        config.scale = [](float x) { return x * x; };
        config.initializer = []() {
            std::string s = "aaaa";
            for(auto& c : s) c += random_generator::random_uniform<int>(0, 25);
            return string_genome{ s };
        };
        config.node_mutates.emplace_back(0.01f, [](float p, individual_t& d) {
            for(auto&& c : std::get<0>(d).data) {
                if(random_generator::random<float>() <= p) {
                    c = static_cast<char>(random_generator::random_uniform<int>(0, 25) + 97);
                }
            }
        });
        genetic::ga g(config);
        g.run();
    }

    TEST(RUNTIME_TEST, CROSSOVER_TEST1) {
        std::tuple<string_genome> d1 = std::make_tuple(string_genome{"aaaa"});
        std::tuple<string_genome> d2 = std::make_tuple(string_genome{"bbbb"});
        auto d3 = genetic::crossover(d1, d2);
        ASSERT_EQ("bbaa", std::get<0>(d3).data);
        auto d4 = genetic::crossover(d2, d1);
        ASSERT_EQ("aabb", std::get<0>(d4).data);
    }

    TEST(RUNTIME_TEST, CROSSOVER_TEST2) {
        std::tuple<string_genome, int_genome> d1 = std::make_tuple(string_genome{"aaaa"}, int_genome{4});
        std::tuple<string_genome, int_genome> d2 = std::make_tuple(string_genome{"bbbb"}, int_genome{3});
        auto d3 = genetic::crossover(d1, d2);
        ASSERT_EQ("bbaa", std::get<0>(d3).data);
        ASSERT_EQ(4, std::get<1>(d3).data);
        auto d4 = genetic::crossover(d2, d1);
        ASSERT_EQ("aabb", std::get<0>(d4).data);
        ASSERT_EQ(4, std::get<1>(d4).data);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
