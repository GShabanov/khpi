/*******************************************************************************
*   File:                                                                      *
*       hashTable.h                                                            *
*                                                                              *
*   Abstract:                                                                  *
*       hash table                                                             *
*                                                                              *
*   Author:                                                                    *
*       Gennady Shabanov (gennady.shabanov@syssafety.com) 1-Feb-2015           *
*                                                                              *
*   Revision History:                                                          *
*******************************************************************************/
// $Id: $
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

template <class T> class THashObject
{
public:
    inline THashObject(T type) throw ()
    {
    }


};

#define HASH_INDEX(key)((UCHAR)((ULONG_PTR)(key) >> 4) ^ (UCHAR)((ULONG_PTR)(key) >> 0xC))

class CMapPtrToPtr
{
private:
    typedef struct _HASH_OBJECT {
        struct _HASH_OBJECT  *next;
        void *object;
        void *key;
    } HASH_OBJECT, *PHASH_OBJECT;

    HASH_OBJECT  m_HashTable[16];
public:
    CMapPtrToPtr()
    {
        memset(m_HashTable, 0, sizeof(m_HashTable))
    }

    // number of elements
    SIZE_T GetCount() const;
    SIZE_T GetSize() const;
    BOOL IsEmpty() const;

    // Lookup and add if not there
    HASH_OBJECT operator[](void* key);

    // removing existing (key, ?) pair
    BOOL RemoveKey(void* key);
    void RemoveAll();

    HASH_OBJECT GetValueAt(void* key) const;
};


#endif // __HASHTABLE_H__