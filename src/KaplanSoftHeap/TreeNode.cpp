#include "TreeNode.h"

TreeNode::TreeNode(int element) {
    ckey = element;
    rank = 0;
    target_size = 1;
    list.push_back(std::make_unique<Element>(element));
    left = nullptr;
    right = nullptr;
}

void TreeNode::sift() {
    while (list.size() < target_size) {
        if (!left && !right) {
            return;
        }

        if (!left || (right && left->ckey > right->ckey)) {
            left.swap(right);
        }
        ckey = left->ckey;
        list.splice(list.end(), left->list);
        if (!left->left && !left->right) {
            left = nullptr;
        } else {
            left->sift();
        }
    }
}

std::shared_ptr<TreeNode> TreeNode::combine(std::shared_ptr<TreeNode> a, std::shared_ptr<TreeNode> b, int r) {
    std::unique_ptr<TreeNode> result(new TreeNode());
    result->rank = a->getRank() + 1;

    if (result->rank <= r) {
        result->target_size = 1;
    } else {
        result->target_size = (3 * a->target_size + 1) / 2;
    }

    result->left = std::move(a);
    result->right = std::move(b);

    result->sift();

    return result;
}

std::unique_ptr<Element> TreeNode::extract_min() {
    std::unique_ptr<Element> min = std::move(list.front());
    list.pop_front();

    if (2 * list.size() <= target_size) {
        if (!left & !right && list.empty()) {
            min->setRemoveStructure(true);
        } else {
            sift();
            min->setUpdateSuffmin(true);
        }
    }
    return min;
}

int TreeNode::getKey() const {
    return ckey;
}

int TreeNode::getRank() const {
    return rank;
}


