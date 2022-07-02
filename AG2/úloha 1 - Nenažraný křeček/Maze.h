#ifndef TASK1_MAZE_H
#define TASK1_MAZE_H


#include <cassert>
#include <cctype>
#include <cmath>
#include <unistd.h>
#include <cinttypes>

#include <memory>
#include <string>
#include <random>
#include <algorithm>

#include <iostream>
#include <iomanip>

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

// junctions are numbered consecutively starting with zero
using Junction = int32_t;
using Corridor = std::pair<Junction, Junction>;

struct Maze {
    Junction hamster, exit;
    std::vector<int> junctions;
    std::vector<Corridor> corridors;

    Maze() {}
    Maze(Junction h, Junction e, std::vector<int> j, std::vector<Corridor> c)
            : hamster(h), exit(e), junctions(std::move(j)), corridors(std::move(c)) {}
};

std::ostream& operator << (std::ostream& out, const Maze& M) {
    constexpr size_t j_wrap = 12;
    constexpr size_t c_wrap = 6;
    constexpr bool cpp = false;

    out << "Maze{ " << (cpp ? "" : "H: ") << M.hamster
        << (cpp ? ", " : ", E: ") << M.exit
        << ",\n  " << (cpp ? "" : "Junctions") << "{";

    for (size_t i = 0; i < M.junctions.size(); i++) {
        if (i % j_wrap == 0) out << "\n    ";
        if (!cpp) out << std::setw(2) << i << ": ";
        out << std::setw(2) << M.junctions[i] << ", ";
    }

    out << "\n  }, " << (cpp ? "" : "Corridors") << "{";
    size_t i = 0;
    for (auto [u, v] : M.corridors) {
        if (i++ % c_wrap == 0) out << "\n    ";
        out << "{" << std::setw(2) << u << ", " << std::setw(2) << v << "}, ";
    }

    return out << "\n  }\n}";
}


#endif //TASK1_MAZE_H
