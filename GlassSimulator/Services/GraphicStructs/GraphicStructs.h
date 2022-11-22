#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <exception>
struct Vertex
{
	glm::vec4 pos;
	glm::vec4 col;
	glm::vec4 norm;
};

class ModInt
{
public:
    ModInt(int from, int to, int num)
    {
        m_from = from;
        m_to = to;
        m_num = num;
        m_size = to - from;
        if (m_num >= m_to)
        {
            m_num = m_from + ((m_num - m_from) % m_size);
        }
        else if (m_num < from)
        {
            int a = from - m_num;
            int b = ((from - m_num) % m_size);
            m_num = to - ((from - m_num) % m_size);
            if (m_num == to)
            {
                m_num = from;
            }
        }
    }

    ModInt& operator++()
    {
        ++m_num;
        if (m_num == m_to)
        {
            m_num = m_from;
        }
        return *this;
    }

    ModInt operator++(int)
    {

        ModInt temp = *this;
        ++m_num;
        if (m_num == m_to)
        {
            m_num = m_from;
        }
        return temp;
    }

    ModInt operator + (ModInt const& obj) {
        ModInt res(m_from, m_to, m_num);
        res.m_num += obj.m_num;

        if (res.m_num >= res.m_to)
        {
            res.m_num = res.m_from + ((res.m_num - res.m_from) % res.m_size);
        }
        return res;
    }

    ModInt operator + (int obj) {
        ModInt res(m_from, m_to, m_num);
        res.m_num += obj;

        if (res.m_num >= res.m_to)
        {
            res.m_num = res.m_from + ((res.m_num - res.m_from) % res.m_size);
        }
        return res;
    }
    ModInt operator - (ModInt const& obj) {
        ModInt res(m_from, m_to, m_num);
        int moded = obj.m_num % res.m_size;
        res.m_num = res.m_num - moded;

        if (res.m_num < res.m_from)
        {
            int a = res.m_from - res.m_num;
            int b = ((res.m_from - res.m_num) % res.m_size);
            res.m_num = res.m_to - ((res.m_from - res.m_num) % res.m_size);
            if (res.m_num == res.m_to)
            {
                res.m_num = res.m_from;
            }
        }
        return res;
    }

    ModInt operator - (int obj) {
        ModInt res(m_from, m_to, m_num);
        int moded = obj % res.m_size;
        res.m_num -= obj;

        if (res.m_num < res.m_from)
        {
            int a = res.m_from - res.m_num;
            int b = ((res.m_from - res.m_num) % res.m_size);
            res.m_num = res.m_to - ((res.m_from - res.m_num) % res.m_size);
            if (res.m_num == res.m_to)
            {
                res.m_num = res.m_from;
            }
        }
        return res;
    }

    ModInt& operator= (int num)
    {
        m_num = num;
        if (m_num >= m_to)
        {
            m_num = m_from + ((m_num - m_from) % m_size);
        }
        else if (m_num < m_from)
        {
            int a = m_from - m_num;
            int b = ((m_from - m_num) % m_size);
            m_num = m_to - ((m_from - m_num) % m_size);
            if (m_num == m_to)
            {
                m_num = m_from;
            }
        }
        return *this;
    }


    int GetDist(const ModInt& val)
    {
        if (m_from != val.m_from || m_to != val.m_to)
        {
            throw std::exception("Ranges are inconsistent!");
        }

        int valTemp = val.m_num - val.m_from;
        int thisTemp = m_num - m_from;
        int res = valTemp - thisTemp;

        if (res < 0)
        {
            res = m_size + res;
        }
        return res;
    }

    operator int() const { return m_num; }
private:
    int m_from = 0;
    int m_to = 0;
    int m_num = 0;
    int m_size = 0;
};

struct Triangle
{
    int a = -1;
    int b = -1;
    int c = -1;
};