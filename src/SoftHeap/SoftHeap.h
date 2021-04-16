#include <memory>
#include <cmath>
#include "HeapStructure.h"

/*
 * SoftHeap representation parameterized with type of TreeNode extending BaseNode
 * a) SequenceNode - SoftHeap as a Soft Sequence Heap by Brodal
 * b) TreeNode - SoftHeap as a Simplified Soft Heap by Kaplan
 */
template<class N>
class SoftHeap {
private:
    double epsilon;
    int r;

    /* rank of the last HeapStructure (equivalent to the largest rank among all HeapStructures) */
    int rank;

    /* SoftHeap indirectly stores a list of HeapStructures */
    std::shared_ptr<HeapStructure<N>> first;

    /* merge Q into P by adding all HeapStructures from Q into P into proper (rank-wise) positions */
    static void merge_into(std::unique_ptr<SoftHeap<N>> &P, std::unique_ptr<SoftHeap<N>> &Q);

    static void insert_structure(std::unique_ptr<SoftHeap<N>> &P, const std::weak_ptr<HeapStructure<N>> &to_insert,
                                 const std::weak_ptr<HeapStructure<N>> &next);

    static void remove_structure(std::unique_ptr<SoftHeap<N>> &P, const std::weak_ptr<HeapStructure<N>> &t);

    /* combine HeapStructures with the same rank. keep the invariant that every Structure has unique rank */
    static void repeated_combine(std::unique_ptr<SoftHeap<N>> &Q, int rank);

public:
    SoftHeap(int element, double eps);

    static void insert(std::unique_ptr<SoftHeap<N>> &P, int e);

    /* meld given soft heaps and return the melded heap. P and Q will be destroyed in the process */
    static std::unique_ptr<SoftHeap<N>> meld(std::unique_ptr<SoftHeap<N>> P, std::unique_ptr<SoftHeap<N>> Q);

    /* return and remove the current min of P */
    static int extract_min(std::unique_ptr<SoftHeap<N>> &P);
};


template<class T>
SoftHeap<T>::SoftHeap(int element, double eps) {
    first = std::make_shared<HeapStructure<T>>(element);
    first->setSuffmin(first);
    rank = 0;
    epsilon = eps;
    r = static_cast<int>(ceil(log2(1 / eps))) + 5;
}

template<class T>
std::unique_ptr<SoftHeap<T>> SoftHeap<T>::meld(std::unique_ptr<SoftHeap<T>> P, std::unique_ptr<SoftHeap<T>> Q) {
    if (P->rank > Q->rank) {
        P.swap(Q);
    }

    merge_into(P, Q);
    repeated_combine(Q, P->rank);

    return Q;
}

template<class T>
int SoftHeap<T>::extract_min(std::unique_ptr<SoftHeap<T>> &P) {
    if (!P->first) {
        throw std::invalid_argument("extract_min called on empty soft heap...");
    }
    auto t = P->first->getSuffmin();
    auto e = t.lock()->extract_min();
    if (e->shouldUpdateSuffmin()) {
        t.lock()->update_suffmin();
    }
    if (e->shouldRemoveStructure()) {
        auto prev = t.lock()->getPrev();
        remove_structure(P, t);
        if (prev.lock().get()) {
            prev.lock()->update_suffmin();
        }
    }
    return e->getKey();
}

template<class T>
void SoftHeap<T>::repeated_combine(std::unique_ptr<SoftHeap> &Q, int rank) {
    std::weak_ptr<HeapStructure<T>> t = Q->first;

    while (t.lock()->getNext()) {
        if (t.lock()->getRank() == t.lock()->getNext()->getRank()) {
            if (!t.lock()->getNext()->getNext() || t.lock()->getRank() != t.lock()->getNext()->getNext()->getRank()) {
                t.lock()->setFirst(
                        T::combine(t.lock()->getFirst(), t.lock()->getNext()->getFirst(), Q->r));
                remove_structure(Q, t.lock()->getNext());
                continue;
            }
        } else if (rank < t.lock()->getRank()) {
            break;
        }
        t = t.lock()->getNext();
    }

    if (t.lock()->getRank() > Q->rank) {
        Q->rank = t.lock()->getRank();
    }

    t.lock()->update_suffmin();
}

template<class T>
void SoftHeap<T>::remove_structure(std::unique_ptr<SoftHeap<T>> &P, const std::weak_ptr<HeapStructure<T>> &t) {
    if (t.lock()->getNext()) {
        t.lock()->getNext()->setPrev(t.lock()->getPrev());
    } else {
        P->rank = t.lock()->getPrev().lock() ? t.lock()->getPrev().lock()->getRank() : 0;
    }
    if (!t.lock()->getPrev().lock()) {
        P->first = t.lock()->getNext();
    } else {
        t.lock()->getPrev().lock()->setNext(t.lock()->getNext());
    }
}

template<class T>
void SoftHeap<T>::insert_structure(std::unique_ptr<SoftHeap<T>> &P, const std::weak_ptr<HeapStructure<T>> &to_insert,
                                   const std::weak_ptr<HeapStructure<T>> &next) {
    to_insert.lock()->setNext(next.lock());
    if (!next.lock()->getPrev().lock()) {
        P->first = to_insert.lock();
    } else {
        next.lock()->getPrev().lock()->setNext(to_insert.lock());
    }
    to_insert.lock()->setPrev(next.lock()->getPrev());
    next.lock()->setPrev(to_insert);
}


template<class T>
void SoftHeap<T>::merge_into(std::unique_ptr<SoftHeap<T>> &P, std::unique_ptr<SoftHeap<T>> &Q) {
    std::weak_ptr<HeapStructure<T>> structure_P = P->first;
    std::weak_ptr<HeapStructure<T>> structure_Q = Q->first;

    while (structure_P.lock()) {
        while (structure_Q.lock()->getNext() && structure_P.lock()->getRank() > structure_Q.lock()->getRank()) {
            structure_Q = structure_Q.lock()->getNext();
        }
        std::weak_ptr<HeapStructure<T>> tmp = structure_P.lock()->getNext();
        insert_structure(Q, structure_P, structure_Q);
        structure_P = tmp;
    }
}


template<class T>
void SoftHeap<T>::insert(std::unique_ptr<SoftHeap<T>> &P, int e) {
    double epsilon_copy = P->epsilon;
    P = meld(std::move(P), std::make_unique<SoftHeap>(e, epsilon_copy));
}