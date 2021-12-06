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
    if(allocator != 0){
        oa_ = allocator;
        oa_node = allocator;
    }
    else{
        OAConfig config(true);
        oa_ = new ObjectAllocator(sizeof(ChHTHeadNode), config);
        oa_node = new ObjectAllocator(sizeof(ChHTNode), config);
    }
}

template <typename T>
ChHashTable<T>::~ChHashTable(){
    if(HTStats_.Allocator_ == 0){
        //delete oa_ and oa_node
    }
}

// Insert a key/data pair into table. Throws an exception if the
// insertion is unsuccessful.(E_DUPLICATE, E_NO_MEMORY)
template <typename T>
void ChHashTable<T>::insert(const char *Key, const T& Data){
    //update/check load factor
    double loadFactor = static_cast<double>(HTStats_.Count_+1)/
                        static_cast<double>(HTStats_.TableSize_);
   if(loadFactor > config_.MaxLoadFactor_){
        GrowTable();
    }

    unsigned hashValue = config_.HashFunc_(Key, HTStats_.TableSize_);
    ChHTHeadNode* bucket = &HashTable_[hashValue];

    //check bucket for Data
    //if doesnt exist, then insert
    if(!findInBucket(bucket, Data, Key))
        push_front(bucket, Data, Key);
    else
        throw HashTableException(HashTableException::E_DUPLICATE, "E_DUPLICATE!");

    
}

template <typename T>
void ChHashTable<T>::GrowTable(){
    HTStats_.Expansions_++;
    double factor = std::ceil(HTStats_.TableSize_ * config_.GrowthFactor_);  // Need to include <cmath>
    unsigned new_size = GetClosestPrime(static_cast<unsigned>(factor)); // Get new prime size
    unsigned old_size = HTStats_.TableSize_;
    HTStats_.TableSize_ = new_size;
    ChHTHeadNode* NewTable = new ChHTHeadNode[new_size];
    ChHTHeadNode* OldTable = HashTable_;
    HashTable_ = NewTable;
    HTStats_.Count_ = 0;
    for(unsigned i{};i<old_size;i++){
        ChHTNode* bucketPtr = OldTable[i].Nodes;
        while(bucketPtr){
            insert(bucketPtr->Key, bucketPtr->Data); //...
            bucketPtr = bucketPtr->Next;
        }
    }
    delete [] OldTable;
}
template <typename T>
bool ChHashTable<T>::findInBucket(ChHTHeadNode* bucket, const T& Data, const char *Key){
    (void)Data;
    ChHTNode* ptr = bucket->Nodes;
    HTStats_.Probes_++;
    if(!ptr)
        return false; //base case

    while(ptr){
        HTStats_.Probes_++;
        if(!strcmp(Key, ptr->Key)){
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
    // ChHTNode* newNode = new ChHTNode(Data);
    ChHTNode* newNode = new (reinterpret_cast<ChHTNode*>(oa_node->Allocate()))ChHTNode(Data);
    strcpy(newNode->Key, Key);
    newNode->Next = predessorNode;
    bucket->Nodes = newNode;
    
}

// Delete an item by key. Throws an exception if the key doesn't exist.
// (E_ITEM_NOT_FOUND)
template <typename T>
void ChHashTable<T>::remove(const char *Key){
    // Delete an item by key. Throws an exception if the key doesn't exist.
    find(Key); 
    
    unsigned hashValue = config_.HashFunc_(Key, HTStats_.TableSize_);
    ChHTHeadNode* bucket = &HashTable_[hashValue];
    ChHTNode* temp = bucket->Nodes;
    ChHTNode* prev = nullptr;
    // HTStats_.Probes_++; // increment probe

    while(temp){
        if(!strcmp(Key, temp->Key)){
            if(prev)
                prev->Next = temp->Next;
            else
                bucket->Nodes = temp->Next;
            
            if(temp->Next) // if node have next, link to next next node
                    temp->Next = temp->Next->Next;
            if(config_.FreeProc_)
                config_.FreeProc_(temp->Data);
                
            if(!oa_node) // if allocator is nullptr
                delete(temp); // delete node
            else
                oa_node->Free(temp); // free node

            HTStats_.Count_--;
            return;
        }
        
        // HTStats_.Probes_++; // increment probe
        prev = temp;
        temp = temp->Next;
    }
    
}

// Find and return data by key. throws exception if key doesn't exist.
// (E_ITEM_NOT_FOUND)
template <typename T>
const T& ChHashTable<T>::find(const char *Key) const{
    unsigned hashValue = config_.HashFunc_(Key, HTStats_.TableSize_);
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
    for(unsigned i = 0; i < HTStats_.TableSize_; ++i)
    {
        ChHTNode* node = HashTable_[i].Nodes;
        ChHTNode* nxt;
        // while node is not empty
        while(node)
        {
        nxt = node->Next;
        if(config_.FreeProc_)
            config_.FreeProc_(node->Data);
        if(!oa_node) // if object allocator is nullptr
            delete(node); // delete node
        else
            oa_node->Free(node); // free node
        
        /* update stats */
        HTStats_.Count_--;
        
        node = nxt; // go to nxt node
        }
        
        HashTable_[i].Nodes = nullptr; // assign node to nullptr
    }
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
