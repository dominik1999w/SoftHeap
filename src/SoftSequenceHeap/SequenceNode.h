#ifndef SEQUENCE_NODE_H
#define SEQUENCE_NODE_H


#include <memory>
#include <list>
#include "../SoftHeap/BaseNode.h"
#include "../SoftHeap/Element.h"

class SequenceNode : public BaseNode {
private:
    std::unique_ptr<Element> key_element;

    std::list<std::unique_ptr<Element>> C;

    int rank = 0;

    std::shared_ptr<SequenceNode> next;
    std::weak_ptr<SequenceNode> prev;

    SequenceNode() = default;

    static void appendNode(std::shared_ptr<SequenceNode> &result, std::shared_ptr<SequenceNode> &to_append);

    void reduce();

public:
    explicit SequenceNode(int element);

    static std::shared_ptr<SequenceNode>
    combine(std::shared_ptr<SequenceNode> a, std::shared_ptr<SequenceNode> b, int r);

    std::unique_ptr<Element> extract_min() override;

    int getKey() const override;

    int getRank() const override;

};


#endif
