.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
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

_func_hanoi:
  addi $sp, $sp, -24
  sw $ra, 20($sp)
  sw $fp, 16($sp)
  addi $fp, $sp, 24
  lw $t0, 0($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -12($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -12($fp)
  lw $t0, 4($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -16($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -16($fp)
  lw $t0, 8($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -20($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -20($fp)
  lw $t0, 12($fp)
  move $a0, $t0
  jal _func_write
  lw $t0, -24($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -24($fp)
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra

main:
  addi $sp, $sp, -16
  sw $ra, 12($sp)
  sw $fp, 8($sp)
  addi $fp, $sp, 16
  lw $t0, -12($fp)
  li $t1, 3
  move $t0, $t1
  sw $t0, -12($fp)
  addi $sp,$sp,-4
  li $s0, 3
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  li $s0, 2
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  li $s0, 1
  sw $s0, 0($sp)
  addi $sp,$sp,-4
  lw $s0, -12($fp)
  sw $s0, 0($sp)
  jal _func_hanoi
  sw $v0, -16($fp)
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
