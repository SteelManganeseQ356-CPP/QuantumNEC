#include "MyLib.hpp"

template <>
void
print<int> (val)
{
  std::cout << "print a int\n";
}
template <typename T>
requires std::is_same_v<long long, T> void
print (T val)
{
  std::cout << "print a long long\n";
}
template <typename T>
requires std::is_same_v<char, T> void
print (T val)
{
  std::cout << "print a char\n";
}
