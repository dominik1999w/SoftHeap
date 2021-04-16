#include "SequenceNode.h"

void SequenceNode::reduce() {
    int counter = 0;
    auto curr = this;
    while (curr) {
        auto next_node = curr->next;
        if (counter > 0 && curr->next && counter & 1) {
            std::list<std::unique_ptr<Element>> to_append;
            to_append.push_back(std::move(curr->key_element));
            to_append.splice(to_append.end(), curr->C);
            next_node->C.splice(next_node->C.end(), to_append);

            next_node->prev = curr->prev;
            curr->prev.lock()->next = next_node;

        }
        curr = next_node.get();
        counter++;
    }
}

SequenceNode::SequenceNode(int key) {
    key_element = std::make_unique<Element>(key);
    rank = 0;
}

void SequenceNode::appendNode(std::shared_ptr<SequenceNode> &result, std::shared_ptr<SequenceNode> &to_append) {
    result->next = to_append;
    to_append = to_append->next;
    result->next->prev = result;
    result = result->next;
    result->next = nullptr;
    result->rank++;
}

std::shared_ptr<SequenceNode>
SequenceNode::combine(std::shared_ptr<SequenceNode> s1, std::shared_ptr<SequenceNode> s2, int r) {
    if (s1->getKey() > s2->getKey()) {
        s1.swap(s2);
    }

    std::shared_ptr<SequenceNode> result = std::move(s1);
    std::shared_ptr<SequenceNode> iter_1 = result->next;
    std::shared_ptr<SequenceNode> iter_2 = std::move(s2);

    result->next = nullptr;
    result->rank++;

    std::shared_ptr<SequenceNode> tmp = result;

    while (iter_1 && iter_2) {
        if (iter_1->getKey() > iter_2->getKey()) {
            iter_1.swap(iter_2);
        }
        appendNode(result, iter_1);

    }
    if (!iter_1) {
        iter_1 = iter_2;
    }
    while (iter_1) {
        appendNode(result, iter_1);
    }

    if (tmp->getRank() + 1 > r && !(tmp->getRank() + 1 - r & 1)) {
        tmp->reduce();
    }

    return tmp;
}

std::unique_ptr<Element> SequenceNode::extract_min() {
    if (!C.empty()) {
        std::unique_ptr<Element> item = std::move(C.front());
        C.pop_front();
        return item;

    } else {
        std::unique_ptr<Element> item = std::move(key_element);
        if (!next) {
            item->setRemoveStructure(true);
        } else {
            item->setUpdateSuffmin(true);

            if (next->next) {
                next->next->prev = std::move(next->prev);
            }
            C.splice(C.end(), std::move(next->C));
            key_element = std::move(next->key_element);
            rank = next->rank;
            next = next->next;
        }
        return item;

    }
}

int SequenceNode::getKey() const {
    return key_element->getKey();
}

int SequenceNode::getRank() const {
    return rank;
}



