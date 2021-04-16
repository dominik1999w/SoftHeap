#include <gtest/gtest.h>
#include <algorithm>
#include "../../src/SoftHeap/SoftHeap.h"
#include "../../src/SoftSequenceHeap/SequenceNode.h"

TEST(SequenceNodeTest, extract_min_1) {
    std::shared_ptr<SequenceNode> a1(new SequenceNode(1));
    EXPECT_EQ(a1->extract_min()->getKey(), 1);
}

TEST(SequenceNodeTest, combine_1) {
    std::shared_ptr<SequenceNode> a1(new SequenceNode(1));
    std::shared_ptr<SequenceNode> a2(new SequenceNode(3));

    auto result = SequenceNode::combine(a1, a2, 5);

    EXPECT_EQ(result->extract_min()->getKey(), 1);
    EXPECT_EQ(result->extract_min()->getKey(), 3);
    EXPECT_EQ(result->getRank(), 1);
}


TEST(SequenceNodeTest, combine_2) {
    std::shared_ptr<SequenceNode> a1(new SequenceNode(1));
    std::shared_ptr<SequenceNode> a2(new SequenceNode(3));
    auto result1 = SequenceNode::combine(a1, a2, 5);

    std::shared_ptr<SequenceNode> a3(new SequenceNode(5));
    std::shared_ptr<SequenceNode> a4(new SequenceNode(8));
    auto result2 = SequenceNode::combine(a3, a4, 5);

    auto result3 = SequenceNode::combine(result1, result2, 5);

    EXPECT_EQ(result3->extract_min()->getKey(), 1);
    EXPECT_EQ(result3->extract_min()->getKey(), 3);
    EXPECT_EQ(result3->extract_min()->getKey(), 5);
    EXPECT_EQ(result3->extract_min()->getKey(), 8);

    EXPECT_EQ(result3->getRank(), 2);
}

TEST(SequenceNodeTest, combine_3) {
    std::shared_ptr<SequenceNode> a1(new SequenceNode(1));
    std::shared_ptr<SequenceNode> a2(new SequenceNode(3));
    auto result1 = SequenceNode::combine(a1, a2, -2); // enforce execution of reduce

    std::shared_ptr<SequenceNode> a3(new SequenceNode(5));
    std::shared_ptr<SequenceNode> a4(new SequenceNode(8));
    auto result2 = SequenceNode::combine(a3, a4, -2); // enforce execution of reduce

    auto result3 = SequenceNode::combine(result1, result2, -1); // enforce execution of reduce
    // result3:
    // 1
    // 5 => C: 3
    // 8

    EXPECT_EQ(result3->extract_min()->getKey(), 1);
    EXPECT_EQ(result3->extract_min()->getKey(), 3);
    EXPECT_EQ(result3->extract_min()->getKey(), 5);
    EXPECT_EQ(result3->extract_min()->getKey(), 8);

    EXPECT_EQ(result3->getRank(), 2);
}

TEST(SequenceNodeTest, combine_4) {
    std::shared_ptr<SequenceNode> a1(new SequenceNode(1));
    std::shared_ptr<SequenceNode> a2(new SequenceNode(3));
    auto result1 = SequenceNode::combine(a1, a2, -2); // enforce execution of reduce

    std::shared_ptr<SequenceNode> a3(new SequenceNode(5));
    std::shared_ptr<SequenceNode> a4(new SequenceNode(8));
    auto result2 = SequenceNode::combine(a3, a4, -2); // enforce execution of reduce

    auto result3 = SequenceNode::combine(result1, result2, -1); // enforce execution of reduce
    // result3:
    // 1
    // 5 => C: 3
    // 8
    std::shared_ptr<SequenceNode> a5(new SequenceNode(0));
    std::shared_ptr<SequenceNode> a6(new SequenceNode(4));
    auto result4 = SequenceNode::combine(a6, a5, -1); // enforce execution of reduce

    auto result5 = SequenceNode::combine(result3, result4, -1); // enforce execution of reduce

    // result5:
    // 0                        0
    // 1                        4 => C:1
    // 4             reduce     8 => C:5,3
    // 5 => C:3
    // 8

    EXPECT_EQ(result5->extract_min()->getKey(), 0);
    EXPECT_EQ(result5->extract_min()->getKey(), 1);
    EXPECT_EQ(result5->extract_min()->getKey(), 4);
    EXPECT_EQ(result5->extract_min()->getKey(), 5);
    EXPECT_EQ(result5->extract_min()->getKey(), 3); // CORRUPTED
    EXPECT_EQ(result5->extract_min()->getKey(), 8);
}