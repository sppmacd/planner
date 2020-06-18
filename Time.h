#pragma once

#include <ostream>
#include <string>

#include "Assertions.h"
#include "ByteStream.h"

class Time
{
    public:
        inline Time(size_t h, size_t m, size_t s = 0)
            : m_hours(h)
            , m_minutes(m)
            , m_seconds(s)
            , m_valid(true)
        {
            ASSERT(m < 60);
            ASSERT(s < 60);
        }
        explicit Time(size_t timestamp = 0);
        Time(std::string str);

        inline size_t h() const {return m_hours;}
        inline size_t m() const {return m_minutes;}
        inline size_t s() const {return m_seconds;}
        size_t timestamp() const;
        bool isValid() const { return m_valid; }

    private:
        friend ByteStream& operator<<(ByteStream& _str, const Time& _data);
        friend ByteStream& operator>>(ByteStream& _str, Time& _data);

        size_t m_hours;
        char m_minutes;
        char m_seconds;
        bool m_valid;
};

std::ostream& operator<<(std::ostream& stream, const Time& time);
bool operator<(const Time& _1, const Time& _2);
bool operator>(const Time& _1, const Time& _2);
bool operator>=(const Time& _1, const Time& _2);
bool operator<=(const Time& _1, const Time& _2);
bool operator==(const Time& _1, const Time& _2);
bool operator!=(const Time& _1, const Time& _2);
Time operator+(const Time& _1, size_t _s);
