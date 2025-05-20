section .data

SRef1 : 
db "check assembler"
   
SRef1_L : equ $ -SRef1

SRef2 : 
db "next statement"
   
SRef2_L : equ $ -SRef2

section .text
global _start
_start: 
mov rax , 1
mov rdi , 1
mov rsi , SRef1
mov rdx , SRef1_L
syscall

mov rax , 1
mov rdi , 1
mov rsi , SRef2
mov rdx , SRef2_L
syscall


mov rax , 60
mov rdi , 172
syscall
