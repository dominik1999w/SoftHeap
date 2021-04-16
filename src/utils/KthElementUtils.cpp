#include <algorithm>
#include "KthElementUtils.h"

int KthElementUtils::medianOfMediansSelect(std::vector<int> &A, int l, int r, int k) {
    int n = r - l;
    if (n < 10) {
        std::sort(A.begin() + l, A.begin() + r);
        return A[l + k - 1];
    }

    int median_size = (n + 4) / 5;
    std::vector<int> median(median_size);
    for (int i = 0; i < n / 5; i++) {
        std::vector<int> tmp(5);
        for (int j = 0; j < 5; j++) {
            tmp[j] = A[l + i * 5 + j];
        }
        median[i] = medianOfMediansSelect(tmp, 0, 5, 5 / 2);
    }

    if (n / 5 * 5 < n) {
        std::vector<int> tmp(n % 5);
        for (int i = 0; i < n % 5; i++)
            tmp[i] = A[l + n / 5 * 5 + i];
        median[n / 5] = medianOfMediansSelect(tmp, 0, n % 5, (n % 5 + 1) / 2);
    }
    int medianOfMedians = medianOfMediansSelect(median, 0, median.size(), median_size / 2 + 1);
    int threshold = findPartition(A, l, r - 1, medianOfMedians);

    if (k < threshold - l + 1) {
        return medianOfMediansSelect(A, l, threshold, k);
    }

    return medianOfMediansSelect(A, threshold, r, k - (threshold - l));
}

int KthElementUtils::findPartition(std::vector<int> &A, int l, int r, int pivot) {
    int i = l;
    while (i <= r) {
        if (A[i] < pivot) {
            std::swap(A[l], A[i]);
            l++;
            i++;
        } else if (A[i] > pivot) {
            std::swap(A[i], A[r]);
            r--;
        } else {
            i++;
        }
    }
    return i;
}
