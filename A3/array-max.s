.text
.globl array_max

# unsigned long array_max(unsigned long count, unsigned long *values)
array_max:
  testq %rdi, %rdi
  jz empty

  movq (%rsi), %rax
  movq $1, %rcx

loop:
  cmpq %rdi, %rcx
  jae done

  movq (%rsi,%rcx,8), %rdx
  cmpq %rax, %rdx
  jbe next
  movq %rdx, %rax

next:
  incq %rcx
  jmp loop

empty:
  xorl %eax, %eax

done:
  ret
