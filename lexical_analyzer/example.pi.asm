section .data

 : 
db "check assembler"
   


section .text
global _start
_start: 
mov rax , 1
mov rdi , 1
mov rsi , SRef
mov rdx , SRef_L
syscall


mov rax , 60
mov rdi , 172
syscall
