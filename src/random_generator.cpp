//
// Created by hungr on 2020/07/21.
//
#include "random_generator.h"

std::random_device random_generator::seed_gen;
std::map<std::thread::id, std::mt19937_64> random_generator::thread_own_gens;
std::mutex random_generator::mtx;
