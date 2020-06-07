.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
func_read:
li $v0, 5
syscall
jr $ra
func_write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra

# FUNCTION fact :
func_fact:
addi $sp, $sp, -20
sw $ra, 16($sp)
sw $fp, 12($sp)
addi $fp, $sp, 20
# PARAM v0
# IF v0 != #1 GOTO label0
lw $s0, 0($fp)
li $s1, 1
bne $s0, $s1, label0
sw $s0, 0($fp)
# RETURN v0
lw $s0, 0($fp)
move $v0, $s0
sw $s0, 0($fp)
move $sp, $fp
lw $ra, -4($fp)
lw $fp, -8($fp)
jr $ra
# GOTO label1
j label1
# LABEL label0 : 
label0:
# o6 := v0 - #1
lw $s0, -12($fp)
lw $s1, 0($fp)
li $s2, 1
sub $s0, $s1, $s2
sw $s0, -12($fp)
# ARG o6
addi $sp, $sp, -4
lw $s0, -12($fp)
sw $s0, 0($sp)
sw $s0, -12($fp)
# o5 := CALL fact
jal func_fact
lw $s0, -16($fp)
move $s0, $v0
sw $s0, -16($fp)
# o3 := v0 * o5
lw $s0, -20($fp)
lw $s1, 0($fp)
lw $s2, -16($fp)
mul $s0, $s1, $s2
sw $s0, -20($fp)
# RETURN o3
lw $s0, -20($fp)
move $v0, $s0
sw $s0, -20($fp)
move $sp, $fp
lw $ra, -4($fp)
lw $fp, -8($fp)
jr $ra
# LABEL label1 : 
label1:
# FUNCTION main :
main:
addi $sp, $sp, -16
sw $ra, 12($sp)
sw $fp, 8($sp)
addi $fp, $sp, 16
# READ v1
jal func_read
lw $s0, -12($fp)
move $s0, $v0
sw $s0, -12($fp)
# IF v1 <= #1 GOTO label2
lw $s0, -12($fp)
li $s1, 1
ble $s0, $s1, label2
sw $s0, -12($fp)
# ARG v1
addi $sp, $sp, -4
lw $s0, -12($fp)
sw $s0, 0($sp)
sw $s0, -12($fp)
# v2 := CALL fact
jal func_fact
lw $s0, -16($fp)
move $s0, $v0
sw $s0, -16($fp)
# GOTO label3
j label3
# LABEL label2 : 
label2:
# v2 := #1
lw $s0, -16($fp)
li $s0, 1
sw $s0, -16($fp)
# LABEL label3 : 
label3:
# WRITE v2
lw $s0, -16($fp)
move $a0, $s0
sw $s0, -16($fp)
jal func_write
# RETURN #0
li $s0, 0
move $v0, $s0
move $sp, $fp
lw $ra, -4($fp)
lw $fp, -8($fp)
jr $ra
