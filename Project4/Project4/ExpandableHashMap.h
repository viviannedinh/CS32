// ExpandableHashMap.h

// Skeleton for the ExpandableHashMap class template.  You must implement the first six
// member functions.
#ifndef ExpandableHashMap_h
#define ExpandableHashMap_h

#include <vector>
#include <list>
#include <iostream>

template<typename KeyType, typename ValueType>
class ExpandableHashMap {
public:
    ExpandableHashMap(double maximumLoadFactor = 0.5);
    ~ExpandableHashMap();
    void reset();
    int size() const;
    void associate(const KeyType& key, const ValueType& value);

      // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;

      // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key) {
        return const_cast<ValueType*>(const_cast<const ExpandableHashMap*>(this)->find(key));
    }

      // C++11 syntax for preventing copying and assignment
    ExpandableHashMap(const ExpandableHashMap&) = delete;
    ExpandableHashMap& operator=(const ExpandableHashMap&) = delete;

private:
    unsigned int getBucketNumber(const KeyType& key) const;
    struct Node {
        KeyType keyN;
        ValueType valueN;
    };
    std::vector<std::list<Node>> m_hash;
    int m_numBuckets;
    int m_maxLoadFactor;
    int m_numItems;
};

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::ExpandableHashMap(double maximumLoadFactor) {
    m_numBuckets = 8;

   if (maximumLoadFactor <= 0)
       m_maxLoadFactor = 0.5;
   else if (maximumLoadFactor > 0)
       m_maxLoadFactor = maximumLoadFactor;
   else
       m_maxLoadFactor = 0.5;
}

template <typename KeyType, typename ValueType>
ExpandableHashMap<KeyType, ValueType>::~ExpandableHashMap() {
    
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::reset() {
    for (int i = 0; i < m_numBuckets; i++) {
        m_hash[i].clear();
    }
    m_numBuckets = 8;
    m_maxLoadFactor = 0.5;
}

template <typename KeyType, typename ValueType>
int ExpandableHashMap<KeyType, ValueType>::size() const {
    return m_numItems;
}

template <typename KeyType, typename ValueType>
void ExpandableHashMap<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value) {
    int bucket = getBucketNumber(key) % m_numBuckets;
    
    if (find(key) == nullptr) {
        Node* p = new Node;
        p->keyN = key;
        p->valueN = value;
        m_hash[bucket].push_back(*p);
        m_numItems++;
    }
    else {
        int bucket = getBucketNumber(key) % m_numBuckets;
        typename std::list<Node>::iterator it;
        for (it = m_hash[bucket].begin(); it != m_hash[bucket].end(); it++) {
            if (it->keyN == key)
                it->valueN = value;
        }
    }
    
    double loadFactor = m_numItems / m_numBuckets;
    if (loadFactor > m_maxLoadFactor) {     // rehash all items
        std::vector<std::list<Node>> newHash;
        for (int i = 0; i < m_numBuckets; i++) {
            typename std::list<Node>::iterator it = m_hash[i].begin();
            for (; it != m_hash[i].end(); it++) {
                int b = getBucketNumber(it->keyN) % (m_numBuckets * 2);
                Node* p = new Node;
                p->keyN = it->keyN;
                p->valueN = it->valueN;
                newHash[b].push_back(*p);
            }
        }
        m_numBuckets = m_numBuckets * 2;
        m_hash = newHash;
    }
}

template <typename KeyType, typename ValueType>
const ValueType* ExpandableHashMap<KeyType, ValueType>::find(const KeyType& key) const {
    int bucket = getBucketNumber(key) % m_numBuckets;
    std::list<Node> p = m_hash[bucket];
    typename std::list<Node>::iterator it = p.begin();
    for (; it != p.end(); it++) {
        if (it->keyN == key)
            return &(it->valueN);
    }
    return nullptr;
}

template<typename KeyType, typename ValueType>
unsigned int ExpandableHashMap<KeyType, ValueType>::getBucketNumber(const KeyType& key) const {
    unsigned int hasher(const KeyType& k); // prototype
    unsigned int h = hasher(key);
    return h;
}

#endif /* ExpandableHashMap_h */
