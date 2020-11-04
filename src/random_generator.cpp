//
// Created by hungr on 2020/07/21.
//
#include "random_generator.h"

std::random_device random_generator::seed_gen;
thread_local std::mt19937_64 random_generator::mt(random_generator::seed_gen());
