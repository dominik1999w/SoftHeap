#ifndef ELEMENT_H
#define ELEMENT_H

class Element {
private:
    int key;

    /*
     * after extracting element from soft heap, soft heap needs to know if any of the following operations should be executed:
     * 1) remove_structure - if extracted element was the only element in the entire heap structure
     * 2) update_suffmin - if extracted element corrupted the suffmin pointers
     */
    bool remove_structure = false;
    bool update_suffmin = false;

public:
    explicit Element(int key);

    int getKey() const;

    bool shouldRemoveStructure() const;

    void setRemoveStructure(bool removeTree);

    bool shouldUpdateSuffmin() const;

    void setUpdateSuffmin(bool updateSuffmin);

};

#endif
