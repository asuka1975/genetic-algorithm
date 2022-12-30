//
// Created by  on 2021/06/24.
//

#include <selector.h>

std::size_t genetic::roulette_function(const std::vector<std::size_t> &idx, const std::vector<float> &f, float r) {
    float s = 0;
    for(auto i : idx) {
        if(r <= (s += f[i])) {
            return i;
        }
    }
    return 0;
}

