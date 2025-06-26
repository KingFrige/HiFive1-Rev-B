#include <riscv_vector.h>
#include <stdio.h>
#include "libfive/uart.h"
#include "libfive/printf.h"

void vector_mul_rvv(const float *a, const float *b, float *dest, size_t vlen) {
  size_t vl;
  size_t i = 0;

  for (; i < vlen; i += vl) {
    // set vector length
    vl = __riscv_vsetvl_e32m1(vlen - i);

    vfloat32m1_t va = __riscv_vle32_v_f32m1(&a[i], vl);
    vfloat32m1_t vb = __riscv_vle32_v_f32m1(&b[i], vl);

    vfloat32m1_t vres = __riscv_vfmul_vv_f32m1(va, vb, vl);

    __riscv_vse32_v_f32m1(&dest[i], vres, vl);
  }
}

void vector_mul_real(const float *a, const float *b, float *dest, size_t vlen) {
  for (size_t i = 0; i < vlen; i++) {
    dest[i] = a[i] * b[i];
  }
}

void print_vec(float *v, size_t vlen, char *msg) {
  printf("%s={ ", msg);
  for (size_t i = 0; i < vlen; i++) {
    printf("%f, ", v[i]);
  }

  printf("}\n");
}

int main() {
  // check RVV support
#ifndef __riscv_v_intrinsic
  printf("RVV NOT supported in this compiler\n");
  return 0;
#endif

  float a[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
  float b[8] = {8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0};
  float c[8], c_real[8];

  vector_mul_rvv(a, b, c, 8);
  vector_mul_real(a, b, c_real, 8);

  print_vec(a, 8, "a[8]");
  print_vec(b, 8, "b[8]");

  for (size_t i = 0; i < 8; i++) {
    if (c[i] != c_real[i]) {
      printf("==========\nTest FAILED: pos %d mismatch\n", i);
      print_vec(c, 8, "c[10]");
      print_vec(c_real, 8, "c_real[10]");

      return -1;
    }
  }

  print_vec(c, 8, "c[10]");
  printf("==========\nTest PASSED\n");

  while (1) {
  }
  return 0;
}

