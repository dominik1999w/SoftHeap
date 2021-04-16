#include "ApproxSortUtils.h"

long long ApproxSortUtils::countInversions(std::vector<int> A) {
    return countInversions(A, 0, static_cast<int>(A.size() - 1));
}

long long ApproxSortUtils::countInversions(std::vector<int> &A, int l, int r) {
    long long inversions = 0;
    if (l < r) {
        int s = l + (r - l) / 2;
        inversions = countInversions(A, l, s);
        inversions += countInversions(A, s + 1, r);
        inversions += merge(A, l, r, s);
    }
    return inversions;
}

long long ApproxSortUtils::merge(std::vector<int> &A, int l, int r, int s) {
    long long inversions = 0;
    int l_size = s - l + 1;
    int r_size = r - s;
    std::vector<int> left, right;
    for (int i = 0; i < l_size; i++) {
        left.push_back(A[l + i]);
    }
    for (int i = 0; i < r_size; i++) {
        right.push_back(A[s + 1 + i]);
    }

    int l_index = 0;
    int r_index = 0;

    while (l_index < l_size && r_index < r_size) {
        if (right[r_index] < left[l_index]) {
            inversions += l_size - l_index;
            A[l++] = right[r_index++];
        } else {
            A[l++] = left[l_index++];
        }
    }

    while (l_index < l_size) {
        A[l++] = left[l_index++];
    }
    while (r_index < r_size) {
        A[l++] = right[r_index++];
    }

    return inversions;
}
