#ifndef TREE_NODE_H
#define TREE_NODE_H

#include <list>
#include <memory>
#include "../SoftHeap/Element.h"
#include "../SoftHeap/BaseNode.h"

class TreeNode : public BaseNode {
private:

    /* the largest key in the list */
    int ckey = 0;

    std::list<std::unique_ptr<Element>> list;

    /* defined recursively as rank = (left/right)->rank - 1 ; rank of a node never changes */
    int rank = 0;

    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;

    /*
     * r is defined as ceil[log(1/epsilon)]+5 ; the target size of a node of rank k is equal to:
     * s_k = 1 if k<=r, ceil[3s_{k-1}/2] otherwise ; size of list should not significantly fall from target_size
     */
    int target_size = 0;

    /* refill elements to list from descendants' lists if size drops significantly from target_size */
    void sift();

    TreeNode() = default;

public:
    explicit TreeNode(int element);

    static std::shared_ptr<TreeNode> combine(std::shared_ptr<TreeNode> a, std::shared_ptr<TreeNode> b, int r);

    std::unique_ptr<Element> extract_min() override;

    int getKey() const override;

    int getRank() const override;
};

#endif
