#include "Time.h"

#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

#include <cli/utils.h>

Time::Time(size_t timestamp)
    : m_valid(true)
{
    m_seconds = timestamp % 60;
    timestamp /= 60;
    m_minutes = timestamp % 60;
    timestamp /= 60;
    m_hours = timestamp;
}

Time::Time(std::string str)
{
    // !! lexer !!
    std::vector<std::string> components;
    int last = -1;
    for(size_t s = 0; s <= str.size(); s++)
    {
        char c = 0;
        if(s != str.size())
            c = str[s];
        if(c == ':' || s == str.size())
        {
            std::string comp = str.substr(last + 1, s - last - 1);
            if(!comp.empty())
                components.push_back(comp);
            last = s;
        }
    }
    //DBGPRINT_VECTOR(components);

    if(components.size() > 3 || components.size() < 1)
    {
        printf("Time: invalid format, should be hh[:mm[:ss]]\n");
        m_valid = false;
        return;
    }

    std::string h = components[0];
    std::string m = (components.size() > 1 ? components[1] : "00");
    std::string s = (components.size() > 2 ? components[2] : "00");

    try
    {
        m_hours = std::stoi(h);
        m_minutes = std::stoi(m);
        m_seconds = std::stoi(s);
    }
    catch(...)
    {
        printf("Time: invalid number format, should be [+-]?[0-9]+\n");
        m_valid = false;
        return;
    }
    m_valid = true;
    return;
}

size_t Time::timestamp() const
{
    return h() * 3600 + m() * 60 + s();
}

std::ostream& operator<<(std::ostream& stream, const Time& time)
{
    //stream << task.getCharacter() << ": " << task.getName() << " (" << task.getStartTime() << " to " << task.getFinishTime() << ")";
    stream << std::setfill('0') << std::setw(2) << time.h() << ":" << std::setw(2) << time.m() << ":" << std::setw(2) << time.s() << std::setfill(' ');
    return stream;
}

ByteStream& operator<<(ByteStream& _str, const Time& _data)
{
    _str << _data.m_hours << _data.m_minutes << _data.m_seconds << (char)_data.m_valid;
    return _str;
}
ByteStream& operator>>(ByteStream& _str, Time& _data)
{
    _str >> _data.m_hours >> _data.m_minutes >> _data.m_seconds >> (char&)_data.m_valid;
    return _str;
}

bool operator<(const Time& _1, const Time& _2)
{
    return _1.timestamp() < _2.timestamp();
}

bool operator>(const Time& _1, const Time& _2)
{
    return _1.timestamp() > _2.timestamp();
}

bool operator>=(const Time& _1, const Time& _2)
{
    return !(_1 < _2);
}

bool operator<=(const Time& _1, const Time& _2)
{
    return !(_1 > _2);
}

bool operator==(const Time& _1, const Time& _2)
{
    return _1.timestamp() == _2.timestamp();
}

bool operator!=(const Time& _1, const Time& _2)
{
    return !(_1 == _2);
}

Time operator+(const Time& _1, size_t _s)
{
    return Time(_1.timestamp() + _s);
}
