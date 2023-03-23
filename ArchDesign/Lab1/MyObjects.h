#pragma once
#include <assert.h>

template <class Poi> class  CPoiArray;


//
// iterator base class
//
template <class Poi>
class Iterator
{
    friend class  CPoiArray<Poi>;

protected:
    const CPoiArray<Poi>       *m_array;
    int                         m_index;
public:
    Iterator()
    {
        m_index = 0;
    }

    Iterator(const CPoiArray<Poi>* s)
        : Iterator()
    {
        m_array = s;
    }

    virtual void first() {};
    virtual void next() {};
    virtual bool isDone() { return true; };
    virtual Poi currentItem() { return 0; };
};


//
// Tourist iterator
//

template <class Poi>
class CTouristIterator : public Iterator<Poi>
{
public:
    CTouristIterator(const CPoiArray<Poi>* s)
        : Iterator<Poi>(s)
    {
    }

    virtual void first()
    {        
        this->m_index = 0;
    }

    virtual void next()
    {
        this->m_index++;
    }

    virtual bool isDone()
    {
        return (this->m_index == this->m_array->m_PoiArraySize);
    }

    virtual Poi currentItem()
    {
        return this->m_array->m_PoiArray[this->m_index];
    }
};


//
// Navigator iterator
//

template <class Poi>
class CNaviIterator : public Iterator<Poi>
{
public:
    CNaviIterator(const CPoiArray<Poi>* s)
        : Iterator<Poi>(s)
    {
        this->m_index = (int)this->m_array->m_PoiArraySize - 1;
    }

    virtual void first()
    {
        this->m_index = (int)this->m_array->m_PoiArraySize - 1;
    }

    virtual void next()
    {
        this->m_index--;
    }

    virtual bool isDone()
    {
        return (this->m_index < 0);
    }

    virtual Poi currentItem()
    {
        return this->m_array->m_PoiArray[this->m_index];
    }
};


//
// Gyde iterator  (same implementation as Tourist for simplify)
//

template <class Poi>
class CGydeIterator : public Iterator<Poi>
{
public:
    CGydeIterator(const CPoiArray<Poi>* s)
        : Iterator<Poi>(s)
    {
    }

    virtual void first()
    {
        this->m_index = 0;
    }

    virtual void next()
    {
        this->m_index++;
    }

    virtual bool isDone()
    {
        return (this->m_index == this->m_array->m_PoiArraySize);
    }

    virtual Poi currentItem()
    {
        return this->m_array->m_PoiArray[this->m_index];
    }
};

//
// container for data
//

template <class Poi>
class  CPoiArray : public Iterator<Poi>
{

    friend class CTouristIterator<Poi>;
    friend class CNaviIterator<Poi>;
    friend class CGydeIterator<Poi>;

    Poi*     m_PoiArray;
    size_t   m_PoiArraySize;

public:

    CPoiArray() {

        m_PoiArray = NULL;
        m_PoiArraySize = 0;
    }

    void AddNew(Poi poi)
    {
        Poi* poiArray = new Poi[m_PoiArraySize + 1];

        for (size_t n = 0; n < m_PoiArraySize; ++n)
        {
            poiArray[n] = m_PoiArray[n];
        }

        m_PoiArraySize++;

        poiArray[m_PoiArraySize - 1] = poi;

        delete[] m_PoiArray;

        m_PoiArray = poiArray;

    }

    ~CPoiArray() {

        for (size_t n = 0; n < m_PoiArraySize; ++n)
        {
            (m_PoiArray + n)->~Poi();
        }
    }

    size_t Size() const {
        return m_PoiArraySize;
    }

    Poi operator[](int n)
    {
        assert(n < m_PoiArraySize);

        return m_PoiArray[n];
    }

    Iterator<Poi> *getTouristIterator()
    {
        return new CTouristIterator<Poi>(this);
    }

    Iterator<Poi>* getNavigatorIterator()
    {
        return new CNaviIterator<Poi>(this);
    }

    Iterator<Poi>* getGydeIterator()
    {
        return new CGydeIterator<Poi>(this);
    }

};