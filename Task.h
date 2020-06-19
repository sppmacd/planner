#pragma once

#include <string>

#include "ByteStream.h"
#include "Time.h"

class Task
{
    public:
        inline Task(const std::string& name, const Time& start, const Time& finish)
            : m_name(name)
            , m_start(start)
            , m_finish(finish)
        {
            ASSERT(m_start <= m_finish);
            if(name.empty())
                m_char = '?';
            else
                m_char = name[0];
        }
        inline Task() : m_char('\0') {}

        inline std::string getName() const { return m_name; }
        inline void setName(const std::string& name) { m_name = name; }

        inline char getCharacter() const { return m_char; }
        inline void setCharacter(char _char) { m_char = _char; }

        inline Time getStartTime() const { return m_start; }
        inline void setStartTime(const Time& start) { m_start = start; }

        inline Time getFinishTime() const { return m_finish; }
        inline void setFinishTime(const Time& finish) { m_finish = finish; }

        bool isRunningAt(const Time&, size_t spacing = 0) const;

    private:
        friend ByteStream& operator<<(ByteStream& _str, const Task& _data);
        friend ByteStream& operator>>(ByteStream& _str, Task& _data);

        std::string m_name;
        char m_char;
        Time m_start;
        Time m_finish;
};

std::ostream& operator<<(std::ostream& stream, const Task& task);
