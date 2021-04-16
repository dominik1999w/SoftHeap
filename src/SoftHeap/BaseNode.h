#ifndef BASE_NODE_H
#define BASE_NODE_H

#include <memory>
#include "Element.h"

class BaseNode {
public:

    /*
     * create new node which contains (in its hierarchy) a and b
     * parameter r is used to define final size of the structure
     */
    static std::shared_ptr<BaseNode> combine(std::shared_ptr<BaseNode> a, std::shared_ptr<BaseNode> b, int r) {
        throw std::runtime_error("combine is not implemented...");
    }

    virtual std::unique_ptr<Element> extract_min() = 0;

    virtual int getKey() const = 0;

    virtual int getRank() const = 0;
};


#endif