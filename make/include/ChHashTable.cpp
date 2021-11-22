#include "ChHashTable.h"

// ObjectAllocator: the usual.
// Config: the configuration for the hash table.
template <typename T>
ChHashTable<T>::ChHashTable(const HTConfig& Config, ObjectAllocator* allocator){
    (void)Config;
    (void)allocator;
    try{
        OAConfig config(true);
        oa_ = new ObjectAllocator(sizeof(ChHTNode), config);
    }
    catch(const std::exception& e){
        throw(BSTException(BSTException::E_NO_MEMORY, e.what()));
    }
}

template <typename T>
ChHashTable<T>::~ChHashTable(){

}

// Insert a key/data pair into table. Throws an exception if the
// insertion is unsuccessful.(E_DUPLICATE, E_NO_MEMORY)
template <typename T>
void ChHashTable<T>::insert(const char *Key, const T& Data){
    (void)Key;
    (void)Data;
}

// Delete an item by key. Throws an exception if the key doesn't exist.
// (E_ITEM_NOT_FOUND)
template <typename T>
void ChHashTable<T>::remove(const char *Key){
    (void)Key; 
}

// Find and return data by key. throws exception if key doesn't exist.
// (E_ITEM_NOT_FOUND)
template <typename T>
const T& ChHashTable<T>::find(const char *Key) const{
    (void)Key;
    return ret_;
}

// Removes all items from the table (Doesn't deallocate table)
template <typename T>
void ChHashTable<T>::clear(){

}

// Allow the client to peer into the data. Returns a struct that contains 
// information on the status of the table for debugging and testing. 
// The struct is defined in the header file.
template <typename T>
HTStats ChHashTable<T>::GetStats() const{
    HTStats ret;
    return ret;
}

template <typename T>
const typename ChHashTable<T>::ChHTHeadNode *ChHashTable<T>::GetTable() const{
    ChHTHeadNode *ret = nullptr;
    return ret;
}
