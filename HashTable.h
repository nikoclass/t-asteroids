#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "Hasheable.h"

#include <list>
using std::list;

#ifndef HASH_TABLE_SIZE
#define HASH_TABLE_SIZE 100
#endif

#ifndef forx
#define forx(_type,_iter,_coll) for (_type::iterator _iter = _coll.begin(); _iter != _coll.end(); _iter++)
#endif

struct hashItem;
typedef list<hashItem> HashItemContainer;

class HashTable
{
    public:
        HashTable(int size = HASH_TABLE_SIZE);
        virtual ~HashTable();

        bool add(int key, Hasheable * value);
        bool add(Hasheable* value);

        Hasheable* remove(int key);
        void remove (Hasheable * value);
        void clear();

        bool containsValue(Hasheable* value);
        bool containsKey(int key);

        int getSize() const { return m_size; };

        Hasheable * getValue(int key);

        // esta va para el niko
        list<Hasheable*> obtenerElementos();

    protected:
    private:
        HashItemContainer  *m_items;     //[HASH_TABLE_SIZE];
        int m_size;
        int m_array_size;

};

#endif // HASHTABLE_H
