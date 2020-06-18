#pragma once

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>

#ifdef BYTESTREAM_DEBUG
#   define LOG_WRITE(ty,sz) printf("    LOG<<%s:%d b\n",#ty,(unsigned)sz)
#   define LOG_READ(ty,sz) printf("    LOG>>%s:%d b\n",#ty,(unsigned)sz)
#else
#   define LOG_WRITE(ty,sz)
#   define LOG_READ(ty,sz)
#endif

class ByteStream : public std::fstream
{
public:
    inline bool fail()
    {
        bool rc = std::fstream::fail();
        //std::cout << "ByteStream: fail: " << rc << std::endl;
        return rc || std::fstream::eof();
    }
    inline std::istream& read(char* __s, size_t __n)
    {
        //std::cout << "ByteStream: read: " << (void*)__s << " (" << __n << " bytes)" << std::endl;
        if(__n > 256)
        {
            //std::cout << "  rejected (__n > 256)" << std::endl;
            std::fstream::setstate(std::ios::failbit);
            return *this;
        }
        return std::fstream::read(__s, __n);
    }
    inline std::ostream& write(const char* __s, size_t __n)
    {
        //std::cout << "ByteStream: write: " << (void*)__s << " (" << __n << " bytes)" << std::endl;
        if(__n > 256)
        {
            //std::cout << "  rejected (__n > 256)" << std::endl;
            std::fstream::setstate(std::ios::failbit);
            return *this;
        }
        return std::fstream::write(__s, __n);
    }
    inline void setstate(std::ios::iostate __state)
    {
        //std::cout << "  state: " << __state << std::endl;
        std::fstream::setstate(__state);
    }
};

// Strings
ByteStream& operator<<(ByteStream& _str, const std::string& _data);
ByteStream& operator>>(ByteStream& _str, std::string& _data);

// Primitives
ByteStream& operator<<(ByteStream& _str, const char& _data);
ByteStream& operator>>(ByteStream& _str, char& _data);

ByteStream& operator<<(ByteStream& _str, const uint32_t& _data);
ByteStream& operator>>(ByteStream& _str, uint32_t& _data);

ByteStream& operator<<(ByteStream& _str, const uint64_t& _data);
ByteStream& operator>>(ByteStream& _str, uint64_t& _data);

// std::arrays
template<class V, size_t Sz>
ByteStream& operator<<(ByteStream& _str, const std::array<V,Sz>& _data)
{
    _str << (uint32_t)_data.size();
    LOG_WRITE("std::array<V,Sz>&", _data.size());
    for(auto& pr: _data)
    {
        _str << pr;
    }
    return _str;
}
template<class V, size_t Sz>
ByteStream& operator>>(ByteStream& _str, std::array<V,Sz>& _data)
{
    uint32_t mapSize = 0;
    _str >> mapSize;
    LOG_READ("std::array<V,Sz>&", mapSize);
    for(uint32_t s = 0; s < mapSize; s++)
    {
        if(s >= Sz)
        {
            _str.setstate(std::ios::failbit);
            return _str;
        }
        _str >> _data[s];
        if(_str.fail())
            return _str;
    }
    return _str;
}

// std::maps with pointer value
template<class K, class V>
ByteStream& operator<<(ByteStream& _str, const std::map<K,V*>& _data)
{
    _str << (uint32_t)_data.size();
    LOG_WRITE("std::map<K,V*>&", _data.size());
    for(auto& pr: _data)
    {
        _str << pr.first << *pr.second;
        if(_str.fail())
            return _str;
    }
    return _str;
}
template<class K, class V>
ByteStream& operator>>(ByteStream& _str, std::map<K,V*>& _data)
{
    uint32_t mapSize = 0;
    _str >> mapSize;
    LOG_READ("std::map<K,V*>&", mapSize);
    for(size_t s = 0; s < mapSize; s++)
    {
        // todo: avoid calling cctor here
        K _k; V* _v = new V;
        _str >> _k >> *_v;
        if(_str.fail())
            return _str;
        _data.insert(std::make_pair(_k, _v));
    }
    return _str;
}
