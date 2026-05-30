
/////////////////////////////////////////////////////////////////////////////

class CMapPtrToPtr : public CObject
{
protected:
    // Association
    struct CAssoc
    {
        CAssoc* pNext;

        void* key;
        void* value;
    };

public:

// Construction
    /* explicit */ CMapPtrToPtr(INT_PTR nBlockSize = 10);

// Attributes
    // number of elements
    INT_PTR GetCount() const;
    INT_PTR GetSize() const;
    BOOL IsEmpty() const;

    // Lookup
    BOOL Lookup(void* key, void*& rValue) const;

// Operations
    // Lookup and add if not there
    void*& operator[](void* key);

    // add a new (key, value) pair
    void SetAt(void* key, void* newValue);

    // removing existing (key, ?) pair
    BOOL RemoveKey(void* key);
    void RemoveAll();

    // iterating all (key, value) pairs
    POSITION GetStartPosition() const;
    void GetNextAssoc(POSITION& rNextPosition, void*& rKey, void*& rValue) const;

    // advanced features for derived classes
    UINT GetHashTableSize() const;
    void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Overridables: special non-virtual (see map implementation for details)
    // Routine used to user-provided hash keys
    UINT HashKey(void* key) const;

// Implementation
protected:
    CAssoc** m_pHashTable;
    UINT m_nHashTableSize;
    INT_PTR m_nCount;
    CAssoc* m_pFreeList;
    struct CPlex* m_pBlocks;
    INT_PTR m_nBlockSize;

    CAssoc* NewAssoc();
    void FreeAssoc(CAssoc*);
    CAssoc* GetAssocAt(void*, UINT&, UINT&) const;

public:
    ~CMapPtrToPtr();
#ifdef _DEBUG
    void Dump(CDumpContext&) const;
    void AssertValid() const;
#endif

    void* GetValueAt(void* key) const;


protected:
    // local typedefs for CTypedPtrMap class template
    typedef void* BASE_KEY;
    typedef void* BASE_ARG_KEY;
    typedef void* BASE_VALUE;
    typedef void* BASE_ARG_VALUE;
};
