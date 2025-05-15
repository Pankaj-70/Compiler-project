; $ represents current address that is after "Hello there"

section .data
SRef1 : 
db "Hello there"
SRef1_L : eq $ - SRef1 


section .text
global _start
_start: 

mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L
syscall
