.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
v1: .word 8
v2: .word 8
v3: .word 4
v4: .word 4
t0: .word 4
t1: .word 4
t2: .word 4
t3: .word 4
t4: .word 4
t5: .word 4
t6: .word 4
t7: .word 4
t8: .word 4
t9: .word 4
t10: .word 4
t11: .word 4
t12: .word 4
t13: .word 4
t14: .word 4
t15: .word 4
t16: .word 4
t17: .word 4
t18: .word 4
t19: .word 4
.text
read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra
li $t1, 0
li $t2, 4
mul $t0, $t1, $t2
sw $t0, t1
lw $t1, v0
lw $t2, t1
add $t0, $t1, $t2
sw $t0, t2
li $t1, 1
li $t2, 4
mul $t0, $t1, $t2
sw $t0, t3
lw $t1, v0
lw $t2, t3
add $t0, $t1, $t2
sw $t0, t4
lw $t6, t2
lw $t1, 0($t6)
lw $t6, t4
lw $t2, 0($t6)
add $t0, $t1, $t2
sw $t0, t0
lw $t1, v4
li $t2, 4
mul $t0, $t1, $t2
sw $t0, t5
la $t1, v1
lw $t2, t5
add $t0, $t1, $t2
sw $t0, t6
lw $t1, v3
lw $t2, v4
add $t0, $t1, $t2
sw $t0, t7
lw $t1, v4
li $t2, 1
add $t0, $t1, $t2
sw $t0, t8
li $t1, 0
li $t2, 8
mul $t0, $t1, $t2
sw $t0, t9
la $t1, v2
lw $t2, t9
add $t0, $t1, $t2
sw $t0, t10
lw $t1, v3
li $t2, 4
mul $t0, $t1, $t2
sw $t0, t11
lw $t1, t10
lw $t2, t11
add $t0, $t1, $t2
sw $t0, t12
li $t1, 0
li $t2, 8
mul $t0, $t1, $t2
sw $t0, t14
la $t1, v2
lw $t2, t14
add $t0, $t1, $t2
sw $t0, t15
lw $t1, v3
li $t2, 4
mul $t0, $t1, $t2
sw $t0, t16
lw $t1, t15
lw $t2, t16
add $t0, $t1, $t2
sw $t0, t17
lw $t1, v3
li $t2, 1
add $t0, $t1, $t2
sw $t0, t19
