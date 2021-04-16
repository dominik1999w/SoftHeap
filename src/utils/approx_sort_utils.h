#ifndef APPROX_SORT_UTILS_H
#define APPROX_SORT_UTILS_H


#include <vector>


class approx_sort_utils {
private:
    static long long countInversions(std::vector<int> &A, int l, int r);

    static long long merge(std::vector<int> &A, int l, int r, int s);

public:
    static long long countInversions(std::vector<int> A);
};


#endif
