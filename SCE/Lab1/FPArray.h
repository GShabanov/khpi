#pragma once

template <class Type>
class CFPArray
{
private:
    Type*  m_values;                    // таблиц€ значень побудована
    size_t m_count;                     // к≥льк≥сть значень в побудован≥й таблиц≥

    bool   m_mean_calculated;

    Type   m_mean;

public:
    CFPArray()
    {
        m_values = NULL;
        m_count = 0;
        m_mean = (Type)0;
        m_mean_calculated = false;
    }

    CFPArray(size_t count)  noexcept
        : CFPArray()
    {
        m_values = new Type[count];

        if (m_values != NULL)
        {
            m_count = count;
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
            this->m_mean_calculated = other.m_mean;

            memcpy(this->m_values, other.m_values, other.m_count * sizeof(Type));
        }
    }

    ~CFPArray()
    {
        if (m_values != NULL)
            delete[] m_values;
    }

    const size_t  Size()
    {
        return m_count;
    }

    const Type  GetSumm()
    {
        Type  summ = (Type)0;

        for (size_t i = 0; i < m_count; i++)
        {
            summ += this->m_values[i];
        }

        return summ;
    }

    Type  GetMean()
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

    const Type* operator()()
    {
        return m_values;
    }

    float& operator[](size_t index)
    {
        return m_values[index];
    }
};
