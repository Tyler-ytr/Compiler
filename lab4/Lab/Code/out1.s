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
_func_read:
  li $v0, 4
  la $a0, _prompt
  syscall
  li $v0, 5
  syscall
  jr $ra

_func_write:
  li $v0, 1
  syscall
  li $v0, 4
  la $a0, _ret
  syscall
  move $v0, $0
  jr $ra

_func_fact:
  addi $sp, $sp, -20
  sw $ra, 16($sp)
  sw $fp, 12($sp)
  addi $fp, $sp, 20
  lw $t0, 0($fp)
  li $t1, 1
  bne $t0, $t1, label0
  lw $t0, 0($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
  j label1
label0:
  lw $t1, 0($fp)
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -12($fp)
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_fact
  sw $v0, -16($fp)
  lw $t1, 0($fp)
  lw $t2, -16($fp)
  mul $t0, $t1, $t2
  sw $t0, -20($fp)
  lw $t0, -20($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
label1:

_func_main:
  addi $sp, $sp, -28
  sw $ra, 24($sp)
  sw $fp, 20($sp)
  addi $fp, $sp, 28
  jal _func_write
  move $t0, $v0
  sw $t0, -12($fp)
  lw $t0, -16($fp)
  lw $t1, -12($fp)
  move $t0, $t1
  sw $t0, -16($fp)
  lw $t0, -16($fp)
  li $t1, 1
  ble $t0, $t1, label2
  addi $sp,$sp,-4
  lw $s0, -16($fp)
  sw $s0, 0($sp)
  jal _func_fact
  sw $v0, -20($fp)
  lw $t0, -24($fp)
  lw $t1, -20($fp)
  move $t0, $t1
  sw $t0, -24($fp)
  j label3
label2:
  lw $t0, -24($fp)
  li $t1, 1
  move $t0, $t1
  sw $t0, -24($fp)
label3:
  lw $t0, -24($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -28($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -28($fp)
  move $v0, $0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
