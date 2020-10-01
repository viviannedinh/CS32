//
//  Map.hpp
//  Project2
//
//  Created by Vivianne Dinh on 1/25/20.
//  Copyright © 2020 Vivianne Dinh. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <string>
#include <iostream>

const int DEFAULT_MAX_ITEMS = 240;

using KeyType = std::string;
using ValueType = double;


class Map
{
  public:
    Map();
    ~Map();
    Map(const Map& other);
    Map& operator=(const Map &other);
    bool empty() const;
    int size() const;
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    void dump();
private:
    struct Node
    {
        KeyType k;
        ValueType v;
        Node *next;
        Node *prev;
    };
    Node *head;
    int m_size;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

#endif /* Map_h */
