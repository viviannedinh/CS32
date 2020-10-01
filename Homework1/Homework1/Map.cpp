#include "Map.h"

Map::Map()
{
    m_numPairs = 0;
}

bool Map::empty() const
{
    if (m_numPairs == 0)
        return true;
    else
        return false;
}

int Map::size() const
{
    return m_numPairs;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    // if capacity is full
    if (m_numPairs == DEFAULT_MAX_ITEMS)
        return false;
    
    // if key is already in map
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
            return false;
    
    // add key/value pair to map
    m_map[m_numPairs].k = key;
    m_map[m_numPairs].v = value;
    m_numPairs++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    // update value if key already in map
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
        {
            m_map[i].v = value;
            return true;
        }
    
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    // update value if key already in map
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
        {
            m_map[i].v = value;
            return true;
        }
    
    // if capacity is full
    if (m_numPairs == DEFAULT_MAX_ITEMS)
        return false;
    
    // add key/value pair to map
    m_map[m_numPairs].k = key;
    m_map[m_numPairs].v = value;
    m_numPairs++;
    return true;
}

bool Map::erase(const KeyType& key)
{
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
        {
            if (m_numPairs > 1)
                m_map[i] = m_map[m_numPairs-1];
            
            m_numPairs--;
            return true;
        }
    
    return false;
}

bool Map::contains(const KeyType& key) const
{
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
            return true;
    
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    for (int i = 0; i < m_numPairs; i++)
        if (key == m_map[i].k)
        {
            value = m_map[i].v;
            return true;
        }
    
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if (i >= 0 && i < size())
    {
        key = m_map[i].k;
        value = m_map[i].v;
        return true;
    }
    
    return false;
}

void Map::swap(Map& other)
{
    Map temp = other;
    
    // copy current map to other map
    other.m_numPairs = m_numPairs;
    for (int i = 0; i < m_numPairs; i++)
    {
        other.m_map[i].k = m_map[i].k;
        other.m_map[i].v = m_map[i].v;
    }
    
    //  copy other(temp) map to current map
    m_numPairs = temp.m_numPairs;
    for (int i = 0; i < m_numPairs; i++)
    {
        m_map[i].k = temp.m_map[i].k;
        m_map[i].v = temp.m_map[i].v;
    }
}
