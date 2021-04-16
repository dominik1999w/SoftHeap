#include <gtest/gtest.h>
#include "../src/SoftHeap/HeapStructure.h"
#include "../src/KaplanSoftHeap/TreeNode.h"
#include "../src/SoftSequenceHeap/SequenceNode.h"

template <class T>
void updateSuffixMinTest() {
    std::shared_ptr<HeapStructure<T>> s1(new HeapStructure<T>(10));
    std::shared_ptr<HeapStructure<T>> s2(new HeapStructure<T>(1));
    std::shared_ptr<HeapStructure<T>> s3(new HeapStructure<T>(8));
    std::shared_ptr<HeapStructure<T>> s4(new HeapStructure<T>(4));

    s1->setNext(s2);
    s2->setPrev(s1);
    s2->setNext(s3);
    s3->setPrev(s2);
    s3->setNext(s4);
    s4->setPrev(s3);

    s4->update_suffmin();

    EXPECT_EQ(s1->getSuffmin().lock().get(), s2.get());
    EXPECT_EQ(s2->getSuffmin().lock().get(), s2.get());
    EXPECT_EQ(s3->getSuffmin().lock().get(), s4.get());
    EXPECT_EQ(s4->getSuffmin().lock().get(), s4.get());
}

TEST(HeapStructureTest, update_suffmin_treenode) {
    updateSuffixMinTest<TreeNode>();
}

TEST(HeapStructureTest, update_suffmin_sequencenode) {
    updateSuffixMinTest<SequenceNode>();
}