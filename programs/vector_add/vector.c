#include <riscv_vector.h>
#include <stdio.h>
#include "libfive/uart.h"
#include "libfive/printf.h"

int x[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 0};
int y[10] = {0, 9, 8, 7, 6, 5, 4, 3, 2, 1};
int z[10], z_real[10];

void vec_add_rvv(int* dst, int* lhs, int* rhs, size_t avl) {
  vint32m2_t vlhs, vrhs, vres;
  for (size_t vl; (vl = __riscv_vsetvl_e32m2(avl));
      avl -= vl, lhs += vl, rhs += vl, dst += vl) {
    vlhs = __riscv_vle32_v_i32m2(lhs, vl);
    vrhs = __riscv_vle32_v_i32m2(rhs, vl);
    vres = __riscv_vadd_vv_i32m2(vlhs, vrhs, vl);
    __riscv_vse32_v_i32m2(dst, vres, vl);
  }
}

void vec_add_real(int* dest, int* lhs, int* rhs, size_t vlen) {
  for (size_t i = 0; i < vlen; i++) {
    dest[i] = lhs[i] + rhs[i];
  }
}

void print_vec(int* v, size_t vlen, char* msg) {
  printf("%s={ ", msg);
  for (size_t i = 0; i < vlen; i++) {
    printf("%d, ", v[i]);
  }

  printf("}\n");
}

int main(int argc, char const* argv[]) {

  // check RVV support
#ifndef __riscv_v_intrinsic
  printf("RVV NOT supported in this compiler\n");
  return 0;
#endif

  vec_add_rvv(z, x, y, 10);
  vec_add_real(z_real, x, y, 10);

  print_vec(x, 10, "x[10]");
  print_vec(y, 10, "y[10]");

  for (size_t i = 0; i < 10; i++) {
    if (z[i] != z_real[i]) {
      printf("==========\nTest FAILED: pos %d mismatch\n", i);
      print_vec(z, 10, "z[10]");
      print_vec(z_real, 10, "z_real[10]");

      return -1;
    }
  }

  print_vec(z, 10, "z[10]");
  printf("==========\nTest PASSED\n");

  printf("Hello world!\n");
  while (1) {
  }

  return 0;
}
