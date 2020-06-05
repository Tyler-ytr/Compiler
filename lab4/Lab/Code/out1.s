.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
v1: .word 4
v2: .word 4
t0: .word 4
t1: .word 4
t2: .word 4
t3: .word 4
t4: .word 4
t5: .word 4
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

fact:
  lw $t0, v0
  li $t1, 1
  bne $t0, $t1, label0
  lw $t0, v0
  move $v0, $t0
  jr $ra
  j label1
label0:
  lw $t1, v0
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, t2
  lw $t1, v0
  lw $t2, t1
  mul $t0, $t1, $t2
  sw $t0, t0
  lw $t0, t0
  move $v0, $t0
  jr $ra
label1:

main:
  lw $t0, v1
  lw $t1, t3
  move $t0, $t1
  sw $t0, v1
  lw $t0, v1
  li $t1, 1
  ble $t0, $t1, label2
  lw $t0, v2
  lw $t1, t4
  move $t0, $t1
  sw $t0, v2
  j label3
label2:
  lw $t0, v2
  li $t1, 1
  move $t0, $t1
  sw $t0, v2
label3:
  lw $t0, t5
  li $t1, 0
  move $t0, $t1
  sw $t0, t5
  move $v0, $0
  jr $ra
