//
//  Map.cpp
//  Project2
//
//  Created by Vivianne Dinh on 1/25/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

#include "Map.h"

Map::Map()
{
    // create empty map
    m_size = 0;
    head = nullptr;
}

Map::~Map()
{
    // traverse map
    while(head != nullptr)
    {
        // delete node
        Node *p = head;
        head = p->next;
        delete p;
    }
}

Map::Map(const Map& other)
{
    // create empty map
    m_size = 0;
    head = nullptr;
    
    // copy nodes from other to new map
    for (int i = other.m_size-1; i >= 0; i--)
    {
        KeyType otherK;
        ValueType otherV;
        other.get(i, otherK, otherV);
        insert(otherK, otherV);
    }
}

Map& Map::operator=(const Map &other)
{
    // check if maps are equal
    if (&other == this)
        return (*this);
    
    // delete nodes in left map
    Node *p = head;
    while (p != nullptr)
    {
        head = p->next;
        delete p;
        p = head;
    }
    
    // create empty map
    m_size = 0;
    head = nullptr;
    
    // copy each node over from right map to left map
    for (int i = other.m_size-1; i >= 0; i--)
    {
        KeyType otherK;
        ValueType otherV;
        other.get(i, otherK, otherV);
        insert(otherK, otherV);
    }
    
    // return map object
    return (*this);
}

bool Map::empty() const
{
    if (m_size == 0)
        return true;
    else
        return false;
}

int Map::size() const
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    // check is capacity is full
    if (m_size == DEFAULT_MAX_ITEMS)
        return false;
    
    // check is map already has key
    if (contains(key))
        return false;
    
    // create new node to top of list
    Node *n = new Node;
    n->k = key;
    n->v = value;
    n->next = head;
    n->prev = nullptr;
    
    // update node pointers
    if (head != nullptr)
    {
        Node *p = head;
        p->prev = n;
        head = n;
    }
    else
        head = n;
    
    // increment size
    m_size++;
    
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    // traverse map
    Node *p = head;
    while (p != nullptr)
    {
        // key is in map
        if (key == p->k)
        {
            // update value
            p->v = value;
            return true;
        }
        // move pointer to next node
        p = p->next;
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    // if key is in map, update value
    if (update(key, value))
        return true;
    // if key is not in map, add node to map
    else
        return insert(key, value);
}

bool Map::erase(const KeyType& key)
{
    // traverse map
    Node *p = head;
    while (p != nullptr)
    {
        if (p->k == key)
        {
            // first node
            if (p->prev == nullptr)
                head = p->next;
            // last node
            else if (p->next == nullptr)
            {
                Node *pre = p->prev;
                pre->next = nullptr;
            }
            // middle nodes
            else
            {
                Node *pre = p->prev;
                Node *nex = p->next;
                pre->next = nex;
                nex->prev = pre;
            }
            delete p;
            m_size--;
            return true;
        }
        // move pointer to next node
        p = p->next;
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    // traverse map
    Node *p = head;
    while (p != nullptr)
    {
        if (key == p->k)
            return true;
        // move pointer to next node
        p = p->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    // traverse map
    Node *p = head;
    while (p != nullptr)
    {
        if (key == p->k)
        {
            // copy value onto value parameter
            value = p->v;
            return true;
        }
        // move pointer to next node
        p = p->next;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < m_size)
    {
        // traverse to ith node
        int n = 0;
        Node *p = head;
        while (n < i)
        {
            p = p->next;
            n++;
        }
        
        // copy key and value onto parameters
        key = p->k;
        value = p->v;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
{
    // swap sizes
    int temp = m_size;
    m_size = other.m_size;
    other.m_size = temp;
    
    // swap heads
    Node *tempHead = head;
    head = other.head;
    other.head = tempHead;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    // variable tells whether a key in both maps have different values
    bool retTrue = true;
    
    // create new map that is a copy of m1
    Map newResult(m1);
    
    // traverse m2
    for (int i = 0; i < m2.size(); i++)
    {
        // get key and value
        KeyType key;
        ValueType value;
        m2.get(i, key, value);
        
        // if key is in both maps
        if (newResult.contains(key))
        {
            // get value of key in m1
            ValueType value2;
            newResult.get(key, value2);
            
            // delete node if key thats in both maps have different values
            // will return false
            if (value != value2)
            {
                retTrue = false;
                newResult.erase(key);
            }
        }
        // if key is only in m2
        else
            newResult.insert(key, value);
    }
    
    result.swap(newResult);
    
    return retTrue;
}

void reassign(const Map& m, Map& result)
{
    // create a copy of map m
    Map newResult(m);
    
    // if only one key/value pair
    if (m.size() == 1)
    {
        result.swap(newResult);
        return;
    }
    
    // create key and value holders
    KeyType key, key2;
    ValueType value, value2;
    // traverse newResult and reassign values
    for (int i = 0; i < m.size(); i++)
    {
        newResult.get(i, key2, value2);
        if (i == m.size()-1)
            m.get(0, key, value);
        else
            m.get(i+1, key, value);
            
        // update value with value of next pair
        newResult.update(key2, value);
    }
    
    result.swap(newResult);
}

void Map::dump()
{
    Node* p = head;
    for (int i = 0; p != nullptr; i++)
    {
        std::cerr << "Node " << i + 1;
        std::cerr << " ------ Key: " << p->k;
        std::cerr << " Value: " << p->v << std::endl;
        p = p->next;
    }
    std::cerr << std::endl;
}
