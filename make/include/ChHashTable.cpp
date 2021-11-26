#include "ChHashTable.h"

// ObjectAllocator: the usual.
// Config: the configuration for the hash table.
template <typename T>
ChHashTable<T>::ChHashTable(const HTConfig& Config, ObjectAllocator* allocator)
    :config_(Config), HTStats_(){

    // for(unsigned i{};i<Config.InitialTableSize_;i++){
    //     // auto& temp = new ChHTHeadNode();
    //     HashTable_.push_back(ChHTHeadNode());
    // }

    HashTable_ = new ChHTHeadNode[config_.InitialTableSize_];
    HTStats_.HashFunc_ = config_.HashFunc_;
    HTStats_.TableSize_ = config_.InitialTableSize_;
    HTStats_.Allocator_ = allocator;
}

template <typename T>
ChHashTable<T>::~ChHashTable(){

}

// Insert a key/data pair into table. Throws an exception if the
// insertion is unsuccessful.(E_DUPLICATE, E_NO_MEMORY)
template <typename T>
void ChHashTable<T>::insert(const char *Key, const T& Data){
    //   (void)Data;
    unsigned hashValue = config_.HashFunc_(Key, config_.InitialTableSize_);
    ChHTHeadNode* bucket = &HashTable_[hashValue];

    //check bucket for Data
    //if doesnt exist, then insert
    if(!find(bucket, Data, Key)){
        push_front(bucket, Data, Key);
    }
    else{
        // HTStats_.Probes_ += 1;
        push_front(bucket, Data, Key);
        // throw(HashTableException(HashTableException::E_DUPLICATE, "duplicate data in bucket"));
    }

}

template <typename T>
bool ChHashTable<T>::find(ChHTHeadNode* bucket, const T& Data, const char *Key){
    (void)Data;
    ChHTNode* ptr = bucket->Nodes;
    HTStats_.Probes_++;
    if(!ptr)
        return false; //base case

    while(ptr){
        HTStats_.Probes_++;
        if( ptr->Key == Key){
            return true;
        }
        ptr = ptr->Next;
    }
    return false;
}

template <typename T>
void ChHashTable<T>::push_front(ChHTHeadNode* bucket, const T& Data, const char *Key){
    // ChHTNode* ptr = bucket->Nodes;
    bucket->Count += 1;
    HTStats_.Count_ += 1;
    //go to last node

    ChHTNode* predessorNode = bucket->Nodes;
    ChHTNode* newNode = new ChHTNode(Data);
    strcpy(newNode->Key, Key);
    newNode->Next = predessorNode;
    bucket->Nodes = newNode;
    // newNode = new (reinterpret_cast<ChHTNode*>(oa_->Allocate()))ChHTNode(Data);
    
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
    unsigned hashValue = config_.HashFunc_(Key, config_.InitialTableSize_);
    ChHTNode* node = HashTable_[hashValue].Nodes;
    
    while(node)
    {
        HTStats_.Probes_++;
        
        if(!strcmp(Key, node->Key))
        {
            return node->Data;
        }
        
        node = node->Next;
    }
    
    throw HashTableException(HashTableException::E_ITEM_NOT_FOUND, "Not Found");
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
    return HTStats_;
}

template <typename T>
const typename ChHashTable<T>::ChHTHeadNode *ChHashTable<T>::GetTable() const{
    return HashTable_;
}
