#pragma once
#include <iostream>
#include <string>
template <typename... Args>
std::string
format (const char *fmt, const Args &...args)
{
  std::string buf{};
  (buf += format (fmt, args), ...);
}
std::string
format (char *fmt, const auto &val)
{
  std::string buf{};
  while (*fmt)
    {
      if (short flags{ 0 }; *fmt == '{')
        {
          for (unsigned long i{}; fmt[i]; i++)
            {
              if (fmt[i] == '}')
                {
                  flags = 1;
                }
              if (fmt[i] == '{')
                {
                  flags = -1;
                }
            }
          if (flags == 1)
            {
              switch (*(++fmt))
                {
                case '}':
                  {
                    buf += format (val);
                    fmt++;
                    break;
                  }
                case '{':
                  {
                    buf += '{';
                    fmt++;
                    break;
                  }
                case ':':
                  {
                    // 以后处理
                  }
                default:

                  break;
                }
            }
          else if (flags == 0)
            {
              buf += *fmt;
              fmt++;
              continue;
            }
          else if (flags == -1)
            {
              fmt++;
              while (*fmt != '{')
                {
                  buf += *fmt;
                }
              fmt++;
              continue;
            }
        }
      else
        {
          buf += *fmt;
        }
      fmt++;
    }
  return buf;
}
template <typename T>
std::string
format (T val)
{
  // 返回写入buf的val
}
template <>
std::string
format<const char> (const char val)
{
}
template <>
std::string
format<const short> (const short val)
{
}
template <>
std::string
format<const int> (const int val)
{
}
template <>
std::string
format<const long long> (const long long val)
{
}
template <>
std::string
format<const __int128> (const __int128 val)
{
}
template <>
std::string
format<const unsigned char> (const unsigned char val)
{
}
template <>
std::string
format<const unsigned short> (const unsigned short val)
{
}
template <>
std::string
format<const unsigned int> (const unsigned int val)
{
}
template <>
std::string
format<const unsigned long long> (const unsigned long long val)
{
}
template <>
std::string
format<const unsigned __int128> (const unsigned __int128 val)
{
}
template <>
std::string
format<const float> (const float val)
{
}
template <>
std::string
format<const double> (const double val)
{
}
template <>
std::string
format<const long double> (const long double val)
{
}
template <>
std::string
format<const char *> (const char *val)
{
}
template <>
std::string
format<const void *> (const void *val)
{
}