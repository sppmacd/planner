#include "ByteStream.h"
#include <arpa/inet.h>

ByteStream& operator<<(ByteStream& _str, const std::string& _data)
{
    if(_str.fail())
        return _str;
    _str << (uint64_t)_data.size();
    LOG_WRITE(std::string, _data.size());
    _str.write(_data.data(), _data.size());
    return _str;
}
ByteStream& operator>>(ByteStream& _str, std::string& _data)
{
    if(_str.fail())
        return _str;
    uint64_t sz;
    _str >> sz;
    if(sz > 256)
    {
        _str.setstate(std::ios::failbit);
        return _str;
    }
    LOG_READ(std::string, sz);
    char* buffer = new char[sz];
    _str.read(buffer, sz);
    _data = std::string(buffer, sz);
    delete[] buffer;
    return _str;
}

ByteStream& operator<<(ByteStream& _str, const char& _data)
{
    if(_str.fail())
        return _str;
    LOG_WRITE(char&, 1);
    _str.write(&_data, 1);
    return _str;
}
ByteStream& operator>>(ByteStream& _str, char& _data)
{
    if(_str.fail())
        return _str;
    LOG_READ(char&, 1);
    _str.read(&_data, 1);
    return _str;
}
ByteStream& operator<<(ByteStream& _str, const uint32_t& _data)
{
    if(_str.fail())
        return _str;
    uint32_t sz = htonl(_data);
    LOG_WRITE(uint32_t&, 4);
    _str.write((char*)&sz, 4);
    return _str;
}
ByteStream& operator>>(ByteStream& _str, uint32_t& _data)
{
    if(_str.fail())
        return _str;
    uint32_t sz;
    _str.read((char*)&sz, 4);
    _data = ntohl(sz);
    LOG_READ(uint32_t&, 4);
    return _str;
}

#define TYP_INIT 0
#define TYP_SMLE 1
#define TYP_BIGE 2

unsigned long long htonll(unsigned long long src) {
  static int typ = TYP_INIT;
  unsigned char c;
  union {
    unsigned long long ull;
    unsigned char c[8];
  } x;
  if (typ == TYP_INIT) {
    x.c[7] = 0x00LL;
    x.ull = 0x01;
    typ = (x.c[7] == 0x01ULL) ? TYP_BIGE : TYP_SMLE;
  }
  if (typ == TYP_BIGE)
    return src;
  x.ull = src;
  c = x.c[0]; x.c[0] = x.c[7]; x.c[7] = c;
  c = x.c[1]; x.c[1] = x.c[6]; x.c[6] = c;
  c = x.c[2]; x.c[2] = x.c[5]; x.c[5] = c;
  c = x.c[3]; x.c[3] = x.c[4]; x.c[4] = c;
  return x.ull;
}

ByteStream& operator<<(ByteStream& _str, const uint64_t& _data)
{
    if(_str.fail())
        return _str;
    uint64_t sz = htonll(_data);
    LOG_WRITE(uint64_t&, 8);
    _str.write((char*)&sz, 8);
    return _str;
}
ByteStream& operator>>(ByteStream& _str, uint64_t& _data)
{
    if(_str.fail())
        return _str;
    uint64_t sz;
    LOG_READ(uint64_t&, 8);
    _str.read((char*)&sz, 8);
    _data = htonll(sz);
    return _str;
}
