#include "HashTable.h"

#include <iostream>

struct hashItem
{
    Hasheable* m_value;
    int m_key; // podria no usar el getHashKey() del hasheable
    hashItem(Hasheable * val, int key) : m_value(val), m_key(key) {}
};


HashTable::HashTable(int size)
{
    if (size <= 0) size = HASH_TABLE_SIZE;
    this->m_items = new HashItemContainer[size];
    m_array_size = size;
    m_size = 0;
}

HashTable::~HashTable()
{
    delete [] m_items;
}

bool HashTable::add(Hasheable * value)
{
    return add(value->getHashCode(), value);
}

bool HashTable::add(int key, Hasheable* value)
{
    int index = key % m_array_size;
    forx(HashItemContainer, iter, m_items[index])
    {
        if ((*iter).m_key == key) return false;
    }
    m_items[index].push_back(hashItem(value, key));
    m_size+= 1;
    return true;
}

Hasheable* HashTable::remove(int key)
{
    int index = key % m_array_size;
    forx(HashItemContainer, iter, m_items[index])
    {
        if ((*iter).m_key == key){
            Hasheable* ret = (*iter).m_value;
            m_items[index].erase(iter);
            m_size-= 1;
            return ret;
        }
    }
    return NULL;
}

void HashTable::remove(Hasheable * value)
{
    remove (value->getHashCode());
}

void HashTable::clear()
{
    for (int i = 0; i <  m_array_size; i++)
        m_items[i].clear();
    m_size = 0;
}

bool HashTable::containsValue(Hasheable* value)
{
    return containsKey(value->getHashCode());
}

bool HashTable::containsKey(int key)
{
    int index = key % m_array_size;
    forx(HashItemContainer, iter, m_items[index])
    {
        if ((*iter).m_key == key) return true;
    }
    return false;
}

Hasheable * HashTable::getValue(int key)
{
    int index = key % m_array_size;
    forx(HashItemContainer, iter, m_items[index])
    {
        if ((*iter).m_key == key){
            return (*iter).m_value;
        }
    }
    return NULL;
}

list<Hasheable*> HashTable::obtenerElementos()
{
    list<Hasheable*> resultado;
    for (int indice = 0; indice < m_array_size; indice++)
    {
        forx(HashItemContainer, iterador, m_items[indice])
        {
            resultado.push_back((*iterador).m_value);
        }
    }
    return resultado;
}
