#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <unordered_map>
#include <fstream>
#include <chrono>
#include "SoftHeap/SoftHeap.h"
#include "utils/ApproxSortUtils.h"
#include "KaplanSoftHeap/TreeNode.h"
#include "SoftSequenceHeap/SequenceNode.h"

#define padding std::string(5, ' ')

template<class T>
std::vector<int> approxSorting(int N, double epsilon, const std::vector<int> &arr = {}) {
    std::vector<int> permutation(N);
    if (arr.empty()) {
        std::iota(permutation.begin(), permutation.end(), 0);
        std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    } else {
        permutation = arr;
    }

    auto P = std::make_unique<SoftHeap<T>>(permutation[0], epsilon);
    for (int i = 1; i < N; i++) {
        SoftHeap<T>::insert(P, permutation[i]);
    }

    std::vector<int> res(N);
    for (int i = 0; i < N; i++) {
        res[i] = SoftHeap<T>::extract_min(P);
    }
    return res;
}

template<class T>
void measureDeviation(int N, const std::string &heapType) {
    std::ofstream file(heapType + "_approx_sort_deviation.txt", std::ios_base::trunc);
    file << "# epsilon" << padding << "r" << padding << "avg_deviation" << std::endl;


    double epsilon = 0;
    double delta = 0.0001;
    std::unordered_map<int, double> cache;

    std::vector<int> permutation(N);
    std::iota(permutation.begin(), permutation.end(), 0);
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));

    while (epsilon + delta < 1) {
        epsilon += delta;
        int r = static_cast<int>(ceil(std::log2(1 / epsilon))) + 5;
        if (cache.find(r) != cache.end()) {
            file << epsilon << padding << r << padding << cache[r] << std::endl;
            continue;
        }

        auto res = approxSorting<T>(N, epsilon, permutation);

        long long misplaced_range = 0;
        for (int i = 0; i < N; i++) {
            misplaced_range += std::abs(res[i] - i);
        }

        cache[r] = static_cast<double>(misplaced_range) / N;

        file << epsilon << padding << r << padding << cache[r] << std::endl;
    }
}

inline std::string appendEpsilonToFileName(const std::string &prefix, double epsilon) {
    std::string epsilon_str = std::to_string(epsilon).substr(0, 4);
    epsilon_str[1] = '-';
    return prefix + "_" + epsilon_str + ".txt";
}

template<class T>
void measureInversions(int N, const std::string &heapType) {
    std::vector<double> epsilons = {0.1, 0.2, 0.4, 0.5};

    for (const double &epsilon: epsilons) {
        std::ofstream f_inversions(appendEpsilonToFileName(heapType + "_approx_sort_inversions", epsilon),
                                   std::ios_base::trunc);
        f_inversions << "# number of inversions in respect to N" << std::endl;
        f_inversions << "# N" << padding << "inversions" << std::endl;

        int delta;
        for (int n = 1; n <= N; n += delta) {
            delta = std::min(static_cast<int>(std::pow(10, std::floor(std::log10(n)))), 10000);

            std::vector<int> res = approxSorting<T>(n, epsilon);

            f_inversions << n << padding << ApproxSortUtils::countInversions(res) << std::endl;
        }
    }
}

template<class T>
void measureExecutionTime(int N, const std::string &heapType) {
    std::vector<double> epsilons = {0.1, 0.2, 0.4, 0.5};

    for (const double &epsilon: epsilons) {
        std::ofstream f_sort_time(appendEpsilonToFileName(heapType + "_approx_sort_time", epsilon),
                                  std::ios_base::trunc);
        f_sort_time << "# execution time of sorting in respect to N" << std::endl;
        f_sort_time << "# N" << padding << "time" << std::endl;

        int delta;
        for (int n = 1; n <= N; n += delta) {
            delta = std::min(static_cast<int>(std::pow(10, std::floor(std::log10(n)))), 10000);

            auto start = std::chrono::high_resolution_clock::now();
            std::vector<int> res = approxSorting<T>(n, epsilon);
            auto stop = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double> duration(stop - start);

            f_sort_time << n << padding << duration.count() << std::endl;
        }
    }
}


int main() {
    int N = 1000000;

    measureDeviation<TreeNode>(N, "tree");
    measureDeviation<SequenceNode>(N, "sequence");

    measureInversions<TreeNode>(N, "tree");
    measureInversions<SequenceNode>(N, "sequence");

    measureExecutionTime<TreeNode>(N, "tree");
    measureExecutionTime<SequenceNode>(N, "sequence");
}
