#include "GroupType.h"

bool operator<(const GroupType& _1, const GroupType& _2)
{
    return _1.getName() < _2.getName();
}

ByteStream& operator<<(ByteStream& _str, const GroupType& _data)
{
    return _str << _data.m_name;
}
ByteStream& operator>>(ByteStream& _str, GroupType& _data)
{
    return _str >> _data.m_name;
}
