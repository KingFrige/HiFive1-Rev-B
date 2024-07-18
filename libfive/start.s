.section ".text.boot"

.globl _start
_start:
	la sp, __stack_top__
.option push
.option norelax
	la gp, __global_pointer$
.option pop
  csrr   t0, mhartid
  lui    t1, 0
  beq    t0, t1, 2f

1:
  j      1b

2:
	call _cstart
