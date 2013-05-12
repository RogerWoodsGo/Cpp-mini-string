#ifndef MY_STRING_H_INCLUDED
#define MY_STRING_H_INCLUDED

#include <cstring>
#include <iostream>

static const size_t TAB_SIZE = 32;

template <typename T>
class Base_string
{
    private:
        union Buff_u
        {
                struct Ptr_s { T * m_ptr; size_t m_capacity; } ptr_s;
                T m_tab[TAB_SIZE];
        } buff_u;
        size_t m_length;

        void Set_Buffer(const T * str)
        {
            m_length = Length(str);
            buff_u.ptr_s.m_capacity = 0;
            buff_u.ptr_s.m_ptr = NULL;

            if (m_length >= TAB_SIZE)
            {
                buff_u.ptr_s.m_capacity = m_length;
                buff_u.ptr_s.m_ptr = new T[m_length + 1];
                Copy(buff_u.ptr_s.m_ptr, str);
                buff_u.ptr_s.m_ptr[m_length] = 0;
            }

            else
            {
                if (buff_u.ptr_s.m_ptr != NULL)
                {
                    delete [] buff_u.ptr_s.m_ptr;
                    buff_u.ptr_s.m_ptr = NULL;
                }

                Copy(buff_u.m_tab, str);
            }
        }

        size_t Length(const char * str) const { return std::strlen(str); }
        size_t Length(const wchar_t * str) const { return std::wcslen(str); }

        char * Copy(char * s1, const char * s2) { return std::strcpy(s1, s2); }
        wchar_t * Copy(wchar_t * s1, const wchar_t * s2) { return std::wcscpy(s1, s2); }

        char * Cat(char * s1, const char * s2) { return std::strcat(s1, s2); }
        wchar_t * Cat(wchar_t * s1, const wchar_t * s2) { return std::wcscat(s1, s2); }

        int Comp(const char * s1, const char * s2) const { return std::strcmp(s1, s2); }
        int Comp(const wchar_t * s1, const wchar_t * s2) const { return std::wcscmp(s1, s2); }

    public:
        Base_string()
        {
            m_length = 0;
            buff_u.ptr_s.m_ptr = NULL;
            buff_u.ptr_s.m_capacity = 0;
            std::memset(buff_u.m_tab, 0, TAB_SIZE);
        }

        Base_string(const T c)
        {
            m_length = 1;
            buff_u.m_tab[0] = c;
            std::memset(buff_u.m_tab + 1, 0, TAB_SIZE - 1);
        }

        Base_string(const T * str)
        {
            Set_Buffer(str);
        }

        Base_string(const Base_string & s)
        {
            if (s.m_length >= TAB_SIZE)
            {
                Set_Buffer(s.buff_u.ptr_s.m_ptr);
            }

            else
            {
                Set_Buffer(s.buff_u.m_tab);
            }
        }

        ~Base_string()
        {
            if (buff_u.ptr_s.m_ptr != NULL)
            {
                delete [] buff_u.ptr_s.m_ptr;
                buff_u.ptr_s.m_ptr = NULL;
            }
        }

        Base_string & operator=(const T * str)
        {
            Base_string<T> temp(str);
            swap(*this, temp);
            return *this;
        }

        Base_string & operator=(Base_string s)
        {
            swap(*this, s);
            return *this;
        }

        Base_string & operator+=(const T * str)
        {
            m_length += Length(str);
            T * temp = new T[m_length + 1];

            if (m_length >= TAB_SIZE)
                Copy(temp, buff_u.ptr_s.m_ptr);
            else
                Copy(temp, buff_u.m_tab);

            Cat(temp, str);
            temp[m_length] = 0;
            Set_Buffer(temp);
            delete [] temp, temp = NULL;

            return *this;
        }

        Base_string & operator+=(const Base_string & s)
        {
            const T * temp;
            if (s.m_length >= TAB_SIZE)
                temp = s.buff_u.ptr_s.m_ptr;
            else
                temp = s.buff_u.m_tab;
            return operator+=(temp);
        }

        const T operator[](const size_t n) const
        {
            if (n == m_length)
                return 0;
            else
            {
                if (m_length >= TAB_SIZE)
                    return buff_u.ptr_s.m_ptr[n];
                else
                    return buff_u.m_tab[n];
            }
        }

        friend void swap(Base_string & s1, Base_string & s2)
        {
            if (s1.m_length >= TAB_SIZE)
            {
                std::swap(s1.buff_u.ptr_s.m_ptr, s2.buff_u.ptr_s.m_ptr);
                std::swap(s1.buff_u.ptr_s.m_capacity, s2.buff_u.ptr_s.m_capacity);
            }
            else
                std::swap(s1.buff_u.m_tab, s2.buff_u.m_tab);
            std::swap(s1.m_length, s2.m_length);
        }

        friend std::ostream & operator<<(std::ostream & out, const Base_string & s)
        {
            if (s.m_length >= TAB_SIZE)
                out << s.buff_u.ptr_s.m_ptr;
            else
                out << s.buff_u.m_tab;
            return out;
        }

        friend std::wostream & operator<<(std::wostream & out, const Base_string & s)
        {
            if (s.m_length >= TAB_SIZE)
                out << s.buff_u.ptr_s.m_ptr;
            else
                out << s.buff_u.m_tab;
            return out;
        }

        friend std::istream & operator>>(std::istream & in, Base_string & s)
        {
            T c;
            s = "";

            do
            {
                in.get(c);
                s += c;
            } while (in.fail() == false && c != '\n');

            Cat(s, "");
            return in;
        }

        friend std::wistream & operator>>(std::wistream & in, Base_string & s)
        {
            T c;
            s = "";

            do
            {
                in.get(c);
                s += c;
            } while (in.fail() == false && c != '\n');

            Cat(s, "");
            return in;
        }

        const T * c_str() const
        {
            if (m_length >= TAB_SIZE)
                return buff_u.ptr_s.m_ptr;
            else
                return buff_u.m_tab;
        }

        size_t length() const
        {
            return m_length;
        }

        void clear()
        {
            Base_string<T> temp;
            swap(*this, temp);
        }

        int compare(const Base_string & s) const
        {
            return Comp(this->c_str(), s.c_str());
        }
};

template <typename T>
inline Base_string<T> operator+(Base_string<T> s, const T * str)
{
    s += str;
    return s;
}

template <typename T>
inline Base_string<T> operator+(Base_string<T> left, const Base_string<T> & right)
{
    T * temp;
    if (right.m_length >= TAB_SIZE)
        temp = right.buff_u.ptr_s.m_ptr;
    else
        temp = right.buff_u.m_tab;

    return operator+(left, temp);
}

template <typename T>
inline bool operator==(const Base_string<T> & s1, const Base_string<T> & s2)
{
    if (s1.compare(s2) == 0)
        return true;
    return false;
}

template <typename T>
inline bool operator!=(const Base_string<T> & s1, const Base_string<T> & s2)
{
    return !operator==(s1, s2);
}

typedef Base_string<char> my_string;
typedef Base_string<wchar_t> my_wstring;

#endif
