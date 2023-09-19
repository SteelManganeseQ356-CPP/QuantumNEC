#pragma once
#include <iostream>
using namespace std;
template <typename T> struct TEST
{
  TEST (T) { cout << "T TEST\n"; }
};
template <> struct TEST<int>
{
  TEST (int) { cout << "int TESE\n"; }
  template <typename T>
  void
  p (T)
  {
  }
};