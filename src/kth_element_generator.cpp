#include <fstream>
#include <cassert>
#include <cmath>
#include <chrono>
#include <numeric>
#include <algorithm>
#include <random>
#include "utils/kth_element_utils.h"
#include "SoftHeap/SoftHeap.h"
#include "SoftSequenceHeap/SequenceNode.h"
#include "KaplanSoftHeap/TreeNode.h"

#define padding std::string(5, ' ')

template<class T>
int softHeapSelect(std::vector<int> &A, int l, int r, int k, double epsilon) {
    int n = r - l;
    if (n < 10) {
        std::sort(A.begin() + l, A.begin() + r);
        return A[l + k - 1];
    }

    auto P = std::make_unique<SoftHeap<T>>(A[l], epsilon);
    for (int i = 1; i < n; i++) {
        SoftHeap<T>::insert(P, A[l + i]);
    }
    int pivot = INT32_MIN;
    int max_iter = std::max(1, static_cast<int>((1 - epsilon) * n / 2));
    for (int i = 0; i < max_iter; i++) {
        int tmp = SoftHeap<T>::extract_min(P);
        pivot = tmp > pivot ? tmp : pivot;
    }

    int threshold = kth_element_utils::findPartition(A, l, r - 1, pivot);

    if (k < threshold - l + 1) {
        return softHeapSelect<T>(A, l, threshold, k, epsilon);
    }

    return softHeapSelect<T>(A, threshold, r, k - (threshold - l), epsilon);
}

inline std::string appendEpsilonToFileName(const std::string &prefix, double epsilon) {
    std::string epsilon_str = std::to_string(epsilon).substr(0, 4);
    epsilon_str[1] = '-';
    return prefix + "_" + epsilon_str + ".txt";
}

template<class T>
void measureExecutionTime(int N, const std::string &heapType) {
    std::vector<double> epsilons = {0.1, 0.2, 0.4};
    for (const double &epsilon: epsilons) {
        std::ofstream f_kth_element_time(appendEpsilonToFileName(heapType + "_kth_element_time", epsilon),
                                         std::ios_base::trunc);
        f_kth_element_time << "# execution time of selecting kth element in respect to N" << std::endl;
        f_kth_element_time << "# N" << padding << "time for soft heap" << padding << "time for median of medians"
                           << std::endl;

        int delta;
        for (int n = 1; n <= N; n += delta) {
            std::vector<int> permutation1(n);
            std::iota(permutation1.begin(), permutation1.end(), 0);
            std::shuffle(permutation1.begin(), permutation1.end(), std::mt19937(std::random_device()()));

            delta = std::min(static_cast<int>(std::pow(10, std::floor(std::log10(n)))), 10000);

            std::vector<int> permutation2 = permutation1;

            int k = rand() % (n) + 1;

            auto start1 = std::chrono::high_resolution_clock::now();
            int element1 = softHeapSelect<T>(permutation1, 0, n, k, epsilon);
            auto stop1 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration1(stop1 - start1);

            auto start2 = std::chrono::high_resolution_clock::now();
            int element2 = kth_element_utils::medianOfMediansSelect(permutation2, 0, n, k);
            auto stop2 = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> duration2(stop2 - start2);

            assert(element1 == element2);

            f_kth_element_time << n << padding << duration1.count() << padding << duration2.count() << std::endl;
        }
    }
}

int main() {
    int N = 1000000;

    measureExecutionTime<TreeNode>(N, "tree");
    measureExecutionTime<SequenceNode>(N, "sequence");

}