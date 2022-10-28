#pragma once

template <class Type>
class CFPArray
{
private:
    Type*  m_values;                    // таблиц€ значень побудована
    size_t m_count;                     // к≥льк≥сть значень в побудован≥й таблиц≥

    bool   m_mean_calculated;
    bool   m_summ_calculated;

    Type   m_mean;
    Type   m_Summ;

public:
    CFPArray()
    {
        m_values = NULL;
        m_count = 0;
        m_mean = (Type)0;
        m_summ_calculated = false;
        m_mean_calculated = false;
    }

    CFPArray(size_t count)  noexcept
    {
        m_values = new Type[count];

        if (m_values != NULL)
        {
            m_count = count;
            memset(m_values, 0, count * sizeof(Type));
        }
    }

    CFPArray(const Type* inputArray, size_t count) noexcept
        : CFPArray()
    {
        m_values = new Type[count];

        if (m_values != NULL)
        {
            m_count = count;
            memcpy(m_values, inputArray, count * sizeof(Type));
        }
    }

    CFPArray(const CFPArray& other) noexcept
    {
        this->m_values = new Type[other.m_count];

        if (m_values != NULL)
        {
            this->m_count = other.m_count;
            this->m_mean = other.m_mean;
            this->m_Summ = other.m_Summ;
            this->m_mean_calculated = other.m_mean_calculated;
            this->m_summ_calculated = other.m_summ_calculated;

            memcpy(this->m_values, other.m_values, other.m_count * sizeof(Type));
        }
    }

    ~CFPArray()
    {
        if (m_values != NULL)
            delete m_values;
    }

    const size_t  Size() const
    {
        return m_count;
    }

    //
    // summ for array
    //
    const Type  GetSumm()
    {
        if (this->m_summ_calculated)
        {
            return this->m_Summ;
        }

        Type  summ = (Type)0;

        for (size_t i = 0; i < m_count; i++)
        {
            summ += this->m_values[i];
        }

        this->m_Summ = summ;
        this->m_summ_calculated = true;

        return summ;
    }

    //
    // mean for array
    //
    const Type  GetMean()
    {
        if (this->m_mean_calculated == true)
        {
            return this->m_mean;
        }

        Type  summ = GetSumm();

        this->m_mean = summ / m_count;

        this->m_mean_calculated = true;

        return this->m_mean;
    }

    CFPArray & operator --()
    {
        m_count--;

        return *this;
        
    }

    const Type* operator()()
    {
        return m_values;
    }

    CFPArray& operator=(const CFPArray& other)
    {
        this->m_values = new Type[other.m_count];

        if (m_values != NULL)
        {
            this->m_count = other.m_count;
            this->m_mean = other.m_mean;
            this->m_Summ = other.m_Summ;
            this->m_mean_calculated = other.m_mean_calculated;
            this->m_summ_calculated = other.m_summ_calculated;

            memcpy(this->m_values, other.m_values, other.m_count * sizeof(Type));
        }

        return *this;
    }

    Type& operator[](size_t index)
    {
        assert(index < this->m_count);
        return m_values[index];
    }
};
