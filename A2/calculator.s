# A terminal calculator
#
# Reads a line of input, interprets it as a simple arithmetic expression,
# and prints the result. The input format is
# <long_integer> <operation> <long_integer>

# Make `main` accessible outside of this module
.global main

# Start of the code section
.text

main:
  # Function prologue
  enter $0, $0

  # Use scanf to retrieve and process a line of input
  # This block implements the following line of C code: 
  #   scanf("%ld %c %ld", &a, &op, &b);
  # Take a look at the man page for scanf and ask questions. You can also look 
  # at scanf_example.c
  movq $scanf_fmt, %rdi
  movq $a, %rsi
  movq $op, %rdx
  movq $b, %rcx
  xorb %al, %al
  call scanf

  movb op, %r8b # TODO: load the operation for comparisons
  movq a, %rax  # TODO: and the LHS
  movq b, %r10

  # TODO: Analyze operation and execute
  cmpb $'+', %r8b
  je add

  cmpb $'-', %r8b
  je subtract

  cmpb $'*', %r8b
  je multiply

  cmpb $'/', %r8b
  je divide
  
  jmp unknown_operation

add:
  addq %r10, %rax
  jmp print_result

subtract:
  subq %r10, %rax
  jmp print_result

multiply:
  imulq %r10, %rax
  jmp print_result

divide:
  cmpq $0, %r10
  je division_error
  cqto
  idivq %r10

print_result:
  # Print the result and return success.
  movq $output_fmt, %rdi
  movq %rax, %rsi
  xorl %eax, %eax
  call printf

  xorl %eax, %eax
  jmp return

unknown_operation:
  movq $unknown_operation_msg, %rdi
  call puts
  movl $1, %eax
  jmp return

division_error:
  movq $division_error_msg, %rdi
  call puts
  movl $1, %eax

return:
  leave
  ret


# Start of the data section
.data

output_fmt: 
  .asciz "%ld\n"
scanf_fmt: 
  .asciz "%ld %c %ld"
unknown_operation_msg:
  .asciz "Unknown operation"
division_error_msg:
  .asciz "Division by zero error"

# "Slots" for scanf
a:  .quad 0
b:  .quad 0
op: .byte 0

