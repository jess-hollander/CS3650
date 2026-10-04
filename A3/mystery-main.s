.section .rodata
argument_error:
  .asciz "Two arguments required."
hat_message:
  .asciz "hat"
tea_message:
  .asciz "tea"
beer_message:
  .asciz "beer"

.text
.globl main
.extern atol
.extern crunch
.extern puts

main:
  pushq %rbp
  movq %rsp, %rbp
  subq $16, %rsp

  cmpl $3, %edi
  jne invalid_arguments

  movq %rsi, -8(%rbp)
  movq 8(%rsi), %rdi
  call atol
  movq %rax, -16(%rbp)

  movq -8(%rbp), %rax
  movq 16(%rax), %rdi
  call atol

  movq -16(%rbp), %rdi
  movq %rax, %rsi
  call crunch

  testq %rax, %rax
  js negative
  jz zero

  leaq beer_message(%rip), %rdi
  jmp print_success

negative:
  leaq hat_message(%rip), %rdi
  jmp print_success

zero:
  leaq tea_message(%rip), %rdi

print_success:
  call puts
  xorl %eax, %eax
  leave
  ret

invalid_arguments:
  leaq argument_error(%rip), %rdi
  call puts
  movl $1, %eax
  leave
  ret
