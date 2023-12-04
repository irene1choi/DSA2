#include "heap.h"
#include <iostream>

// Use provided code for heap constructor
heap::heap(int capacity) : mapping(capacity * 2) {
    data.resize(capacity + 1);
    // Initialize size and capacity of heap
    this->capacity = capacity;
    size = 0;
}

// Insert a new node into the binary heap
int heap::insert(const std::string &id, int key, void *pv) {
    // Return 1 if heap filled to capacity
    if (size == capacity) {
        return 1;
    }
    // Insert if mapping does not contain id
    if (!mapping.contains(id)) {
        size++;
        data[size].id = id;
        data[size].key = key;
        data[size].pData = pv;
        mapping.insert(id, &data[size]);
        percolateUp(size);
        return 0;
    // Return 2 if given id exists
    } else {
        return 2;
    }
}

// Set the key of the specified node to the specified value
int heap::setKey(const std::string &id, int key) {
    // Return 1 if node does not exist
    if (!mapping.contains(id)) {
        return 1;
    }
    // Get a pointer to the node with the specified id
    node *pn = static_cast<node *>(mapping.getPointer(id));
    int pos = getPos(pn);
    int oldVal = pn->key;
    pn->key = key;
    // If the new key is larger, percolate down, else percolate up
    if (key > oldVal) {
        percolateDown(pos);
    } else {
        percolateUp(pos);
    }
    return 0;
}

// Return data with the smallest key and delete it from the heap
int heap::deleteMin(std::string *pId, int *pKey, void *ppData) {
    // Return 1 if heap is empty
    if (size == 0) {
        return 1;
    }
    // Remove smallest node
    mapping.remove(data[1].id);
    // Write data depending on specified arugment
    if (pId) {
        *pId = data[1].id;
    }
    if (pKey) {
        *pKey = data[1].key;
    }
    // Provided code for filling in ppData
    if (ppData) {
        *(static_cast<void **>(ppData)) = data[1].pData;
    }
    // Replace root with last node in heap
    data[1] = data[size--];
    // Restore heap property by percolating down
    percolateDown(1);
    return 0;
}

// Delete node with the specified id from the heap
int heap::remove(const std::string &id, int *pKey, void *ppData) {
    // Return 1 if node with id does not exist
    if (!mapping.contains(id)) {
        return 1;
    }
    // Get a pointer to the node with the specified id
    node *pn = static_cast<node *>(mapping.getPointer(id));
    // Write data depending on specified arugment
    if (pKey) {
        *pKey = pn->key;
    }
    if (ppData) {
        *(static_cast<void **>(ppData)) = pn->pData;
    }
    // Set key to small value to ensure it becomes the root and delete it
    setKey(id, -100000000);
    deleteMin();
    return 0;
}

// Percolate up functionality
void heap::percolateUp(int index) {
    node temp = data[index];
    // Continue while node is not the root and key is less than its parent's key
    for (; index > 1 && temp.key < data[index/2].key; index /= 2) {
        data[index] = data[index/2];
        mapping.setPointer(data[index].id, &data[index]);
    }
    // Place node in final position and update mapping
    data[index] = temp;
    mapping.setPointer(data[index].id, &data[index]);
}

// Percolate down functionality
void heap::percolateDown(int index) {
    node temp = data[index];
    int child;
    // Continue while index still has children
    for (; index * 2 <= size; index = child) {
        child = index * 2;
        // Choose child with the smaller key
        if (child != size && data[child + 1].key < data[child].key) {
            child++;
        }
        // Swap keys if child's key is smaller than current node's key
        if (data[child].key < temp.key) {
            data[index] = data[child];
            mapping.setPointer(data[index].id, &data[index]);
        } else {
            break;
        }
    }
    // Place node in final position and update mapping
    data[index] = temp;
    mapping.setPointer(data[index].id, &data[index]);
}

// getPos implementation taken from provided code
int heap::getPos(node *pn) {
    int pos = pn - &data[0];
    return pos;
}
