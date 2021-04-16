#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include "../src/SoftHeap/SoftHeap.h"
#include "../src/KaplanSoftHeap/TreeNode.h"
#include "../src/SoftSequenceHeap/SequenceNode.h"

template<class T>
void extractMinInsert1Test() {
    std::unique_ptr<SoftHeap<T>> P(new SoftHeap<T>(5, 0));
    int e = SoftHeap<T>::extract_min(P);
    EXPECT_EQ(e, 5);
}

TEST(SoftHeapTest, extract_min_insert_treenode_1) {
    extractMinInsert1Test<TreeNode>();
}

TEST(SoftHeapTest, extract_min_insert_sequencenode_1) {
    extractMinInsert1Test<SequenceNode>();
}

template<class T>
void extractMinInsert2Test() {
    std::unique_ptr<SoftHeap<T>> P(new SoftHeap<T>(5, (double) 1 / (6 * 2))); // no corruption
    SoftHeap<T>::insert(P, 4);
    SoftHeap<T>::insert(P, 8);
    SoftHeap<T>::insert(P, 3);
    SoftHeap<T>::insert(P, 1);
    SoftHeap<T>::insert(P, 7);

    int e1 = SoftHeap<T>::extract_min(P);
    int e2 = SoftHeap<T>::extract_min(P);

    EXPECT_EQ(e1, 1);
    EXPECT_EQ(e2, 3);
}

TEST(SoftHeapTest, extract_min_insert_treenode_2) {
    extractMinInsert2Test<TreeNode>();
}

TEST(SoftHeapTest, extract_min_insert_sequencenode_2) {
    extractMinInsert2Test<SequenceNode>();
}

template<class T>
void extractMinInsert3Test() {
    std::unique_ptr<SoftHeap<T>> P(new SoftHeap<T>(0, (double) 1 / 600)); // no corruption
    std::vector<int> permutation;
    for (int i = 1; i < 600; i++) {
        permutation.push_back(i);
    }
    std::shuffle(permutation.begin(), permutation.end(), std::mt19937(std::random_device()()));
    for (const int &i: permutation) {
        SoftHeap<T>::insert(P, i);
    }
    for (int i = 0; i < 600; i++) {
        EXPECT_EQ(SoftHeap<T>::extract_min(P), i);
    }
}

TEST(SoftHeapTest, extract_min_insert_treenode_3) {
    extractMinInsert3Test<TreeNode>();
}

TEST(SoftHeapTest, extract_min_insert_sequencenode_3) {
    extractMinInsert3Test<SequenceNode>();
}

template<class T>
void extractMinInsert4Test() {
    std::unique_ptr<SoftHeap<TreeNode>> P(new SoftHeap<TreeNode>(5, 0));
    SoftHeap<TreeNode>::extract_min(P);
    EXPECT_ANY_THROW(SoftHeap<TreeNode>::extract_min(P));
}

TEST(SoftHeapTest, extract_min_insert_treenode_4) {
    extractMinInsert4Test<TreeNode>();
}

TEST(SoftHeapTest, extract_min_insert_sequencenode_4) {
    extractMinInsert4Test<SequenceNode>();
}

template<class T>
void extractMinMeld1Test() {
    std::unique_ptr<SoftHeap<T>> P(new SoftHeap<T>(5, 0.5)); // no corruption
    std::unique_ptr<SoftHeap<T>> Q(new SoftHeap<T>(4, 0.5)); // no corruption

    auto res = SoftHeap<T>::meld(std::move(P), std::move(Q));

    int e = SoftHeap<T>::extract_min(res);
    EXPECT_EQ(e, 4);
}

TEST(SoftHeapTest, extract_min_meld_treenode_1) {
    extractMinMeld1Test<TreeNode>();
}

TEST(SoftHeapTest, extract_min_meld_sequencenode_1) {
    extractMinMeld1Test<SequenceNode>();
}
