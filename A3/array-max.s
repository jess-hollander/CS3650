.text
.globl array_max
.type array_max, @function

# unsigned long array_max(unsigned long count, unsigned long *values)
array_max:
  testq %rdi, %rdi
  jz .Lempty

  movq (%rsi), %rax
  movq $1, %rcx

.Lloop:
  cmpq %rdi, %rcx
  jae .Ldone

  movq (%rsi,%rcx,8), %rdx
  cmpq %rax, %rdx
  jbe .Lnext
  movq %rdx, %rax

.Lnext:
  incq %rcx
  jmp .Lloop

.Lempty:
  xorl %eax, %eax

.Ldone:
  ret

.size array_max, .-array_max
.section .note.GNU-stack,"",@progbits
