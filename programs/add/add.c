#include "libfive/uart.h"
#include "libfive/printf.h"

int main() {

  int a = 3000;
  int b = 5000;

  int c = a + b;

  printf("a + b = %d\n", c);

  while(1){;}
  return 0;
}
