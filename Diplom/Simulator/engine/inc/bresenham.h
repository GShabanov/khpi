

#ifndef __BRESENHAME_H__
#define __BRESENHAME_H__

#define IS_SIGNED( T ) (((T)-1)<0)

template <class T>
class CBresenhame
{
public:
    typedef struct _POINT {
        T  base;
        T  result;
    } POINT, *PPOINT;

private:
    T     m_base;
    T     m_result;
    T     m_max;
    T     m_dbase;      // delta base
    T     m_dresult;    // delta result
    T     m_binc;       // base increment
    T     m_rinc;       // base decrement
    T     m_error;      // error

public:
    CBresenhame<T>(POINT *begin, POINT *end)
    {
        //assert(IS_SIGNED(T));

        this->m_base = begin->base;
        this->m_result = begin->result;

        this->m_max = end->result;
        this->m_dbase = (end->base - begin->base);
        this->m_dresult = (end->result - begin->result);

        this->m_binc = 1;

        if (this->m_dbase < 0) {
            this->m_binc = -1;
            this->m_dbase = -this->m_dbase;
        }

        this->m_rinc = 1;

        if (this->m_dresult < 0) {
            this->m_rinc = -1;
            this->m_dresult = -this->m_dresult;
        }

        this->m_error = (this->m_dresult << 1) - this->m_dbase;

    }

    inline CBresenhame<T> &operator()(POINT *begin, POINT *end)
    {
        this->m_base = begin->base;
        this->m_result = begin->result;

        this->m_max = end->result;
        this->m_dbase = (end->base - begin->base);
        this->m_dresult = (end->result - begin->result);

        this->m_binc = 1;

        if (this->m_dbase < 0) {
            this->m_binc = -1;
            this->m_dbase = -this->m_dbase;
        }

        this->m_rinc = 1;

        if (this->m_dresult < 0) {
            this->m_rinc = -1;
            this->m_dresult = -this->m_dresult;
        }

        this->m_error = (this->m_dresult << 1) - this->m_dbase;

        return *this;
    }

    inline void interate()
    {
        this->m_base += this->m_binc;
        this->m_error += (this->m_dresult << 1);

        while ((this->m_error > 0) && (this->m_result != this->m_max))
        {
            this->m_error -= (this->m_dbase << 1);
            this->m_result += this->m_rinc;
        }

    }

    inline CBresenhame<T> &operator ++()
    {
        iterate();
        return this;
    }

    inline T getCount()
    {
        return this->m_dbase;
    }

    inline T base() {
        return this->m_base;
    }

    inline T result() {
        return this->m_result;
    }
};

#endif // __BRESENHAME_H__