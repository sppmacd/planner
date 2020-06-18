#pragma once

#include <string>

#include "ByteStream.h"

class GroupType
{
    public:
        GroupType(const std::string& name = ""): m_name(name) {}
        inline std::string getName() const { return m_name; }

    private:
        friend ByteStream& operator<<(ByteStream& _str, const GroupType& _data);
        friend ByteStream& operator>>(ByteStream& _str, GroupType& _data);

        std::string m_name;
};

bool operator<(const GroupType& _1, const GroupType& _2);
