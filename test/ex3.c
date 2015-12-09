
#include <stdbool.h>
#include <stdio.h>

extern void f(bool,bool);

int main()
{
  bool x = true;
  bool y = false;
  f(x, y);

  return 0;
}

