#include "hash.h"

#include <iostream>
#include <cmath>
#include <string>
#include <vector>

// Initializes hash table with a prime greater or equal to size
hashTable::hashTable(int size) {
    unsigned int primeSize = getPrime(size);
    data.resize(primeSize);
    capacity = primeSize;
    filled = 0;
    // Initialize each hash table slot as unoccupied and not deleted
    for (auto& item : data) {
        item.isOccupied = false;
        item.isDeleted = false;
    }
}

// Inserts a key value pair into the hash table
int hashTable::insert(const std::string &key, void *pv) {
    // If more than half the slots are filled, rehash the table
    if (static_cast<double>(filled) / capacity >= 0.5) {
        if (!rehash()) {
            // Return 2 for failed rehashing
            return 2; 
        }
    }
    // Find the slot for the key
    int hashVal = hash(key);
    while (data[hashVal].isOccupied) {
        // If key already exists and is not deleted, return 1
        if (data[hashVal].key == key && !data[hashVal].isDeleted) {
            return 1;
        }
        // Linear probing to find the next slot
        hashVal = (hashVal + 1) % capacity;
    }
    // Insert the key value pair into the hash table
    data[hashVal].key = key;
    data[hashVal].isOccupied = true;
    data[hashVal].isDeleted = false;
    data[hashVal].pv = pv;
    filled++;
    // Return 0 for success
    return 0;
}

// Check if the hash table contains a key
bool hashTable::contains(const std::string &key) {
    return findPos(key) != -1;
}

// Computes a hash value for the given key
// Hash function taken from Data Structures and Algorithms Analysis in C++ by Mark A. Weiss pg 195
int hashTable::hash(const std::string &key) {
    unsigned int hashVal = 0;
    for (char ch: key)
        hashVal = 37 * hashVal + ch;
    return hashVal % capacity;
}

// Finds the position of the key in the hash table
int hashTable::findPos(const std::string &key) {
    int pos = hash(key);
    while (data[pos].isOccupied) {
        if (data[pos].key == key && !data[pos].isDeleted) {
            // Return the position of key if found
            return pos;
        }
        // Linear probing to find the next slot
        pos = (pos + 1) % capacity;
    }
    // Return -1 if key not found
    return -1;
}

// Resize and rehash the table
bool hashTable::rehash() {
    int newCapacity = getPrime(capacity);
    std::vector<hashItem> tempVector = std::move(data);
    try {
        data.resize(newCapacity);
    } catch (const std::bad_alloc&) {
        // Return false on failed rehash
        return false;
    }
    capacity = newCapacity;
    filled = 0;
    // Reinsert old items into the resized hash table
    for (const auto& item : tempVector) {
        if (item.isOccupied && !item.isDeleted) {
            insert(item.key, item.pv);
        }
    }
    return true;
}

// Obtain prime number greater or equal to size
unsigned int hashTable::getPrime(int size) {
    static const std::vector<int> primes = {
        101039, 202079, 404113, 808217, 1616437, 3232877, 6465767, 12931529, 25863059, 
        51726121, 103452249, 206904483, 413809041, 827618087, 1655236173
    };
    for (int prime : primes) {
        if (prime > size) {
            return prime;
        }
    }
    // Return the largest prime number if size is bigger than all primes
    return primes.back();
}

// Get the pointer associated with the specified key
void *hashTable::getPointer(const std::string &key, bool *b) {
    int pos = findPos(key);
    // If b is provided, return true if key is in hashtable, false otherwise
    if (b != nullptr) {
        if (pos != -1) {
            *b = true;
        } else {
            *b = false;
        }
    }
    // If key does not exist in the hashtable, return nullptr
    if (pos == -1) {
        return nullptr;
    }
    // Otherwise, return pointer associated with key
    void* pointerValue = data[pos].pv;
    return pointerValue;
}

// Set the pointer associated with the specified key
int hashTable::setPointer(const std::string &key, void *pv) {
    int pos = findPos(key);
    if (pos == -1) {
        // Return 1 if key does not exist in the hash table
        return 1;
    }
    data[pos].pv = pv;
    // Return 0 on success
    return 0;
}

// Delete the item with the specified key
bool hashTable::remove(const std::string &key) {
    int pos = findPos(key);
    // Return false if the key does not exist in the hash table
    if (pos == -1) {
        return false;
    }
    data[pos].isOccupied = false;
    data[pos].isDeleted = true;
    // Return true on success
    return true;
}
