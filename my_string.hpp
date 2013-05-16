#ifndef MY_STRING_H_INCLUDED
#define MY_STRING_H_INCLUDED

#include <algorithm>
#include <cstring>
#include <istream>
#include <ostream>

const size_t TAB_SIZE = 32;

template <typename string_type>
struct String_Type {};

template <> struct String_Type<char>
{
    static size_t length(const char * str) { return std::strlen(str); }
    static char * cpy(char * s1, const char * s2) { return std::strcpy(s1, s2); }
    static char * cat(char * s1, const char * s2) { return std::strcat(s1, s2); }
    static int cmp(const char * s1, const char * s2) { return std::strcmp(s1, s2); }
    static char null_char() { return '\0'; }
    static const char * null_str() { return ""; }
};

template <> struct String_Type<wchar_t>
{
    static size_t length(const wchar_t * str) { return std::wcslen(str); }
    static wchar_t * cpy(wchar_t * s1, const wchar_t * s2) { return std::wcscpy(s1, s2); }
    static wchar_t * cat(wchar_t * s1, const wchar_t * s2) { return std::wcscat(s1, s2); }
    static int cmp(const wchar_t * s1, const wchar_t * s2) { return std::wcscmp(s1, s2); }
    static wchar_t null_char() { return L'\0'; }
    static const wchar_t * null_str() { return L""; }

};

template <typename T>
class my_basic_string
{
    #define TAB_SIZE(T) (TAB_SIZE / sizeof(T))
    typedef String_Type<T> string_type;

    private:
        union m_buff
        {
                struct m_pointer { T * m_ptr; size_t m_capacity; } m_pointer;
                T m_tab[TAB_SIZE(T)];
        } m_buff;
        size_t m_length;

        void free_memory()
        {
            if (m_length < TAB_SIZE(T))
            {
                delete [] m_buff.m_pointer.m_ptr;
                m_buff.m_pointer.m_ptr = NULL;
                m_buff.m_pointer.m_capacity = 0;
            }
        }

        T * buffer()
        {
            return m_length >= TAB_SIZE(T) ? m_buff.m_pointer.m_ptr : m_buff.m_tab;
        }

    public:
        my_basic_string()
        {
            m_buff.m_tab[0] = string_type::null_str();
            m_length = 0;
        }

        my_basic_string(const T & c)
        {
            m_length = 1;
            m_buff.m_tab[0] = c;
            m_buff.m_tab[1] = string_type::null_char();
        }

        my_basic_string(const T * str)
        {
            m_length = string_type::length(str);

            if (m_length >= TAB_SIZE(T))
            {
                m_buff.m_pointer.m_ptr = new T[m_length + 1];
                string_type::cpy(m_buff.m_pointer.m_ptr, str);
                m_buff.m_pointer.m_capacity = m_length;
            }

            else
            {
                string_type::cpy(m_buff.m_tab, str);
            }

        }

        my_basic_string(const my_basic_string & s)
        {
            if (s.m_length >= TAB_SIZE(T))
            {
                string_type::cpy(m_buff.m_pointer.m_ptr, s.m_buff.m_pointer.m_ptr);
                m_buff.m_pointer.m_capacity = s.m_buff.m_pointer.m_capacity;
            }
            else
            {
                string_type::cpy(m_buff.m_tab, s.m_buff.m_tab);
            }

            m_length = s.m_length;
        }

        ~my_basic_string()
        {
            this->free_memory();
        }

        friend void swap(my_basic_string & s1, my_basic_string & s2)
        {
            if (s1.m_length >= TAB_SIZE(T))
            {
                std::swap(s1.m_buff.m_pointer.m_ptr, s2.m_buff.m_pointer.m_ptr);
                std::swap(s1.m_buff.m_pointer.m_capacity, s2.m_buff.m_pointer.m_capacity);
            }
            else
                std::swap(s1.m_buff.m_tab, s2.m_buff.m_tab);

            std::swap(s1.m_length, s2.m_length);
        }

        const T & operator[](size_t n) const
        {
            return (this->c_str() + n)[0];
        }

        T & operator[](size_t n)
        {
            return (this->buffer() + n)[0];
        }

        void reserve (size_t n = 0)
        {
            if (m_length >= TAB_SIZE(T))
            {
                if (n < m_buff.m_pointer.m_capacity)
                    return;
            }
            else
            {
                if (n < TAB_SIZE(T))
                    return;
            }

            T * temp = new T[n + 1];
            string_type::cpy(temp, this->c_str());
            this->free_memory();
            m_buff.m_pointer.m_ptr = new T[n + 1];
            string_type::cpy(m_buff.m_pointer.m_ptr, temp);
            m_buff.m_pointer.m_capacity = n;

            delete [] temp, temp = NULL;
        }

        void resize (size_t n, T c = string_type::null_char())
        {
            this->reserve(n);

            if (n < m_length)
            {
                (*this)[n] = string_type::null_char();
            }

            else if (n > m_length)
            {
                std::memset(&(*this)[m_length], c, n - m_length);
                (*this)[n] = string_type::null_char();
            }

            m_length = n;
        }

        const T * c_str() const
        {
            return const_cast<my_basic_string*>(this)->buffer();
        }

        size_t length() const
        {
            return m_length;
        }

        size_t capacity() const
        {
            if (m_length >= TAB_SIZE(T))
                return m_buff.m_pointer.m_capacity;
            else
                return TAB_SIZE(T);
        }

        size_t max_size() const
        {
            return (size_t)-1 / sizeof(T);
        }

        void clear()
        {
            this->resize(0);
        }

        void print() const
        {
            std::cout << this->c_str() << std::endl;
        }

        bool empty() const
        {
            return m_length == 0;
        }
};

typedef my_basic_string<char> my_string;
typedef my_basic_string<wchar_t> my_wstring;

#endif
