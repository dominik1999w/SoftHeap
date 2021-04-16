#include <gtest/gtest.h>
#include "../../src/KaplanSoftHeap/TreeNode.h"

TEST(NodeTest, combine_simple_1) {
    std::shared_ptr<TreeNode> a1(new TreeNode(1));
    std::shared_ptr<TreeNode> a2(new TreeNode(3));

    auto result = TreeNode::combine(a1, a2, 5); // result->target_size = 1

    EXPECT_EQ(result->getKey(), 1);
    EXPECT_EQ(result->getRank(), 1);
}

TEST(NodeTest, combine_simple_2) {
    std::shared_ptr<TreeNode> a1(new TreeNode(1));
    std::shared_ptr<TreeNode> a2(new TreeNode(3));

    auto result = TreeNode::combine(a1, a2, 0); // result->target_size = 2

    EXPECT_EQ(result->getKey(), 3);
    EXPECT_EQ(result->getRank(), 1);
}

TEST(NodeTest, combine_complex_1) {
    std::shared_ptr<TreeNode> a1(new TreeNode(1));
    std::shared_ptr<TreeNode> a2(new TreeNode(3));

    auto result = TreeNode::combine(a1, a2, 5);  // result->target_size = 1
    /*
     *      result [1]
     * null<-     -> a2 [3]
     *
     */

    std::shared_ptr<TreeNode> a3(new TreeNode(4));
    std::shared_ptr<TreeNode> a4(new TreeNode(5));

    auto result2 = TreeNode::combine(a3,a4, 0); //result2->target_size = 2
    /*
     *      result2 [4,5]
     * null<-     -> null
     *
     */

    auto result3 = TreeNode::combine(result2, result, 0); //result3->target_size = 3
    /*
    *               result3                               result3                                result3[1,3]    result3[1,3,4,5]
    * result2 [4,5]<-     ->result [1]     =>  result [1]<-     ->result2 [4,5]   => result2 [4,5]<-          =>
    *                           -> a2 [3]               -> a2[3]
    */

    EXPECT_EQ(result3->getKey(), 5);
    EXPECT_EQ(result3->getRank(), 2);
}

TEST(NodeTest, combine_complex_2) {
    std::shared_ptr<TreeNode> a1(new TreeNode(5));
    std::shared_ptr<TreeNode> a2(new TreeNode(4));

    auto result = TreeNode::combine(a1, a2, 5);  // result->target_size = 1
    /*
     *      result [4]
     * null<-     -> a2 [5]
     *
     */

    std::shared_ptr<TreeNode> a3(new TreeNode(2));
    std::shared_ptr<TreeNode> a4(new TreeNode(3));
    auto result2 = TreeNode::combine(a3, a4, 0); //result2->target_size = 2
    /*
     *      result2 [2,3]
     * null<-     -> null
     *
     */

    auto result3 = TreeNode::combine(result2, result, 0); //result3->target_size = 3
    /*
    *               result3                                   result3[2,3]                   result3[2,3,4]
    * result2 [2,3]<-     ->result [4]     => result2 [4] <-               =>   result2 [5]<-
    *                           -> a2 [5]             -> a2[5]
    */

    EXPECT_EQ(result3->getKey(), 4);
    EXPECT_EQ(result3->getRank(), 2);
}