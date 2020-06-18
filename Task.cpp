#include "Task.h"

#include <ostream>

#include "Time.h"

////////////////////////////////////////////
// 0-1----------------8-------------------12
// S------------------F
// @
////////////////////////////////////////////

bool Task::isRunningAt(const Time& time, size_t spacing) const
{
    return spacing != 0 ?
            (m_start < (time+spacing) && m_finish > time) :
            (m_start <= time && m_finish > time);
}

std::ostream& operator<<(std::ostream& stream, const Task& task)
{
    stream << "\033[31;1m" << task.getCharacter() << "\033[0m: \033[33m" << task.getName() << "\033[0m (\033[1;95m" << task.getStartTime() << "\033[0m to \033[1;95m" << task.getFinishTime() << "\033[0m)";
    //stream << task.getCharacter();
    return stream;
}

ByteStream& operator<<(ByteStream& _str, const Task& _data)
{
    LOG_WRITE(Task&, 0);
    _str << _data.m_name << _data.m_char << _data.m_start << _data.m_finish;
    return _str;
}
ByteStream& operator>>(ByteStream& _str, Task& _data)
{
    LOG_READ(Task&, 0);
    _str >> _data.m_name >> _data.m_char >> _data.m_start >> _data.m_finish;
    return _str;
}
