#ifndef KTH_ELEMENT_UTILS_H
#define KTH_ELEMENT_UTILS_H


#include <vector>

class KthElementUtils {
public:
    static int findPartition(std::vector<int> &A, int l, int r, int pivot);
    static int medianOfMediansSelect(std::vector<int> &A, int l, int r, int k);
};

#endif
