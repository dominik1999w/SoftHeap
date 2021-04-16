#ifndef HEAP_STRUCTURE_H
#define HEAP_STRUCTURE_H

#include <memory>
#include "Element.h"

template<class T>
class HeapStructure : public std::enable_shared_from_this<HeapStructure<T>> {
private:
    std::shared_ptr<T> first;

    std::shared_ptr<HeapStructure<T>> next;
    std::weak_ptr<HeapStructure<T>> prev;

    /* points to the structure with the smallest (c)key among all following structures */
    std::weak_ptr<HeapStructure<T>> suffmin;

public:
    explicit HeapStructure(int element);

    /* starting from this heap structure fix suffmin pointers of all preceding structures */
    void update_suffmin();

    std::unique_ptr<Element> extract_min();

    int getRank() const;

    std::shared_ptr<T> &getFirst();

    void setFirst(std::shared_ptr<T> first);

    std::shared_ptr<HeapStructure> getNext();

    void setNext(std::shared_ptr<HeapStructure> next);

    std::weak_ptr<HeapStructure> getPrev();

    void setPrev(std::weak_ptr<HeapStructure> prev);

    std::weak_ptr<HeapStructure> getSuffmin();

    void setSuffmin(std::weak_ptr<HeapStructure> suffmin);

};

template<class T>
HeapStructure<T>::HeapStructure(int element) {
    first = std::make_unique<T>(element);
}

template<class T>
void HeapStructure<T>::update_suffmin() {
    auto curr = this;
    while (curr) {
        if (!curr->next || curr->first->getKey() <= curr->next->suffmin.lock()->first->getKey()) {
            curr->suffmin = curr->weak_from_this();
        } else {
            curr->suffmin = curr->next->suffmin;
        }
        curr = curr->prev.lock().get();
    }
}

template<class T>
std::unique_ptr<Element> HeapStructure<T>::extract_min() {
    return first->extract_min();
}

template<class T>
int HeapStructure<T>::getRank() const {
    return first->getRank();
}

template<class T>
std::shared_ptr<T> &HeapStructure<T>::getFirst() {
    return first;
}

template<class T>
void HeapStructure<T>::setFirst(std::shared_ptr<T> f) {
    first = std::move(f);
}

template<class T>
std::shared_ptr<HeapStructure<T>> HeapStructure<T>::getNext() {
    return next;
}

template<class T>
void HeapStructure<T>::setNext(std::shared_ptr<HeapStructure<T>> n) {
    next = std::move(n);
}

template<class T>
std::weak_ptr<HeapStructure<T>> HeapStructure<T>::getPrev() {
    return prev;
}

template<class T>
void HeapStructure<T>::setPrev(std::weak_ptr<HeapStructure<T>> p) {
    prev = std::move(p);
}

template<class T>
std::weak_ptr<HeapStructure<T>> HeapStructure<T>::getSuffmin() {
    return suffmin;
}

template<class T>
void HeapStructure<T>::setSuffmin(std::weak_ptr<HeapStructure<T>> s) {
    suffmin = std::move(s);
}

#endif