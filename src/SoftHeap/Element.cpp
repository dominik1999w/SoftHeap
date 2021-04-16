#include "Element.h"

Element::Element(int key) : key(key) {}

int Element::getKey() const {
    return key;
}

bool Element::shouldRemoveStructure() const {
    return remove_structure;
}

void Element::setRemoveStructure(bool removeTree) {
    remove_structure = removeTree;
}

bool Element::shouldUpdateSuffmin() const {
    return update_suffmin;
}

void Element::setUpdateSuffmin(bool updateSuffmin) {
    update_suffmin = updateSuffmin;
}
