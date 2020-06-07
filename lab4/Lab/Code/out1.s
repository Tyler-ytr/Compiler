.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
_func_read:
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
#FUNCTION array_structure : 

_func_array_structure:
  addi $sp, $sp, -8160
  sw $ra, 8156($sp)
  sw $fp, 8152($sp)
  addi $fp, $sp, 8160
#DEC v0 8040
#t0 := #0 * #804
  li $t1, 0
  li $t2, 804
  mul $t0, $t1, $t2
  sw $t0, -8052($fp)
#t1 := &v0 + t0
  la $t1, -8048($fp)
  lw $t2, -8052($fp)
  add $t0, $t1, $t2
  sw $t0, -8056($fp)
#t2 := t1
  lw $t0, -8060($fp)
  lw $t1, -8056($fp)
  move $t0, $t1
  sw $t0, -8060($fp)
#*t2 := #2
  lw $t6, -8060($fp)
  lw $t0, 0($t6)
  li $t1, 2
  move $t0, $t1
  lw $t6, -8060($fp)
  sw $t0, 0($t6)
#t3 := #0 * #804
  li $t1, 0
  li $t2, 804
  mul $t0, $t1, $t2
  sw $t0, -8064($fp)
#t4 := &v0 + t3
  la $t1, -8048($fp)
  lw $t2, -8064($fp)
  add $t0, $t1, $t2
  sw $t0, -8068($fp)
#t5 := t4 + #4
  lw $t1, -8068($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8072($fp)
#t6 := #0 * #8
  li $t1, 0
  li $t2, 8
  mul $t0, $t1, $t2
  sw $t0, -8076($fp)
#t7 := t5 + t6
  lw $t1, -8072($fp)
  lw $t2, -8076($fp)
  add $t0, $t1, $t2
  sw $t0, -8080($fp)
#t8 := t7
  lw $t0, -8084($fp)
  lw $t1, -8080($fp)
  move $t0, $t1
  sw $t0, -8084($fp)
#*t8 := #1234
  lw $t6, -8084($fp)
  lw $t0, 0($t6)
  li $t1, 1234
  move $t0, $t1
  lw $t6, -8084($fp)
  sw $t0, 0($t6)
#t9 := #0 * #804
  li $t1, 0
  li $t2, 804
  mul $t0, $t1, $t2
  sw $t0, -8088($fp)
#t10 := &v0 + t9
  la $t1, -8048($fp)
  lw $t2, -8088($fp)
  add $t0, $t1, $t2
  sw $t0, -8092($fp)
#t11 := t10 + #4
  lw $t1, -8092($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8096($fp)
#t12 := #0 * #8
  li $t1, 0
  li $t2, 8
  mul $t0, $t1, $t2
  sw $t0, -8100($fp)
#t13 := t11 + t12
  lw $t1, -8096($fp)
  lw $t2, -8100($fp)
  add $t0, $t1, $t2
  sw $t0, -8104($fp)
#t14 := t13 + #4
  lw $t1, -8104($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8108($fp)
#t15 := #0 * #804
  li $t1, 0
  li $t2, 804
  mul $t0, $t1, $t2
  sw $t0, -8112($fp)
#t16 := &v0 + t15
  la $t1, -8048($fp)
  lw $t2, -8112($fp)
  add $t0, $t1, $t2
  sw $t0, -8116($fp)
#t17 := t16 + #4
  lw $t1, -8116($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8120($fp)
#t18 := #0 * #8
  li $t1, 0
  li $t2, 8
  mul $t0, $t1, $t2
  sw $t0, -8124($fp)
#t19 := t17 + t18
  lw $t1, -8120($fp)
  lw $t2, -8124($fp)
  add $t0, $t1, $t2
  sw $t0, -8128($fp)
#t20 := t19
  lw $t0, -8132($fp)
  lw $t1, -8128($fp)
  move $t0, $t1
  sw $t0, -8132($fp)
#*t14 := *t20
  lw $t6, -8108($fp)
  lw $t0, 0($t6)
  lw $t6, -8132($fp)
  lw $t1, 0($t6)
  move $t0, $t1
  lw $t6, -8108($fp)
  sw $t0, 0($t6)
#t21 := #0 * #804
  li $t1, 0
  li $t2, 804
  mul $t0, $t1, $t2
  sw $t0, -8136($fp)
#t22 := &v0 + t21
  la $t1, -8048($fp)
  lw $t2, -8136($fp)
  add $t0, $t1, $t2
  sw $t0, -8140($fp)
#t23 := t22 + #4
  lw $t1, -8140($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8144($fp)
#t24 := #0 * #8
  li $t1, 0
  li $t2, 8
  mul $t0, $t1, $t2
  sw $t0, -8148($fp)
#t25 := t23 + t24
  lw $t1, -8144($fp)
  lw $t2, -8148($fp)
  add $t0, $t1, $t2
  sw $t0, -8152($fp)
#t26 := t25 + #4
  lw $t1, -8152($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -8156($fp)
#WRITE *t26
  lw $t6, -8156($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t27 := #0
  lw $t0, -8160($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -8160($fp)
#RETURN t27
  lw $t0, -8160($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
#FUNCTION func_overflow : 

_func_func_overflow:
  addi $sp, $sp, -28
  sw $ra, 24($sp)
  sw $fp, 20($sp)
  addi $fp, $sp, 28
#PARAM v1
#t28 := #0 - #1
  li $t1, 0
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -12($fp)
#t29 := t28 * #4
  lw $t1, -12($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -16($fp)
#t30 := v1 + t29
  lw $t1, 0($fp)
  lw $t2, -16($fp)
  add $t0, $t1, $t2
  sw $t0, -20($fp)
#t31 := #1 * #4
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -24($fp)
#t32 := v1 + t31
  lw $t1, 0($fp)
  lw $t2, -24($fp)
  add $t0, $t1, $t2
  sw $t0, -28($fp)
#*t30 := *t32
  lw $t6, -20($fp)
  lw $t0, 0($t6)
  lw $t6, -28($fp)
  lw $t1, 0($t6)
  move $t0, $t1
  lw $t6, -20($fp)
  sw $t0, 0($t6)
#RETURN #0
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
#FUNCTION danger_ops : 

_func_danger_ops:
  addi $sp, $sp, -128
  sw $ra, 124($sp)
  sw $fp, 120($sp)
  addi $fp, $sp, 128
#DEC v2 12
#DEC v3 12
#t33 := &v2
  lw $t0, -36($fp)
  la $t1, -20($fp)
  move $t0, $t1
  sw $t0, -36($fp)
#*t33 := #2019
  lw $t6, -36($fp)
  lw $t0, 0($t6)
  li $t1, 2019
  move $t0, $t1
  lw $t6, -36($fp)
  sw $t0, 0($t6)
#t34 := &v2 + #8
  la $t1, -20($fp)
  li $t2, 8
  add $t0, $t1, $t2
  sw $t0, -40($fp)
#*t34 := #2020
  lw $t6, -40($fp)
  lw $t0, 0($t6)
  li $t1, 2020
  move $t0, $t1
  lw $t6, -40($fp)
  sw $t0, 0($t6)
#t35 := &v2 + #4
  la $t1, -20($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -44($fp)
#t36 := #0 - #1
  li $t1, 0
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -48($fp)
#t37 := t36 * #4
  lw $t1, -48($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -52($fp)
#t38 := t35 + t37
  lw $t1, -44($fp)
  lw $t2, -52($fp)
  add $t0, $t1, $t2
  sw $t0, -56($fp)
#t39 := #0 - #1
  li $t1, 0
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -60($fp)
#*t38 := t39
  lw $t6, -56($fp)
  lw $t0, 0($t6)
  lw $t1, -60($fp)
  move $t0, $t1
  lw $t6, -56($fp)
  sw $t0, 0($t6)
#t40 := &v2 + #4
  la $t1, -20($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -64($fp)
#t41 := #1 * #4
  li $t1, 1
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -68($fp)
#t42 := t40 + t41
  lw $t1, -64($fp)
  lw $t2, -68($fp)
  add $t0, $t1, $t2
  sw $t0, -72($fp)
#t43 := #0 - #2
  li $t1, 0
  li $t2, 2
  sub $t0, $t1, $t2
  sw $t0, -76($fp)
#*t42 := t43
  lw $t6, -72($fp)
  lw $t0, 0($t6)
  lw $t1, -76($fp)
  move $t0, $t1
  lw $t6, -72($fp)
  sw $t0, 0($t6)
#t44 := &v2
  lw $t0, -80($fp)
  la $t1, -20($fp)
  move $t0, $t1
  sw $t0, -80($fp)
#WRITE *t44
  lw $t6, -80($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t45 := #0
  lw $t0, -84($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -84($fp)
#t46 := &v2 + #8
  la $t1, -20($fp)
  li $t2, 8
  add $t0, $t1, $t2
  sw $t0, -88($fp)
#WRITE *t46
  lw $t6, -88($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t47 := #0
  lw $t0, -92($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -92($fp)
#t48 := &v3
  lw $t0, -96($fp)
  la $t1, -32($fp)
  move $t0, $t1
  sw $t0, -96($fp)
#*t48 := #123
  lw $t6, -96($fp)
  lw $t0, 0($t6)
  li $t1, 123
  move $t0, $t1
  lw $t6, -96($fp)
  sw $t0, 0($t6)
#t49 := &v3 + #8
  la $t1, -32($fp)
  li $t2, 8
  add $t0, $t1, $t2
  sw $t0, -100($fp)
#*t49 := #456
  lw $t6, -100($fp)
  lw $t0, 0($t6)
  li $t1, 456
  move $t0, $t1
  lw $t6, -100($fp)
  sw $t0, 0($t6)
#t51 := &v3 + #4
  la $t1, -32($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -104($fp)
#ARG t51 
  addi $sp,$sp,-4
  lw $s0, -104($fp)
  sw $s0, 0($sp)
#t50 := CALL func_overflow
  jal _func_func_overflow
  sw $v0, -108($fp)
#t52 := &v3
  lw $t0, -112($fp)
  la $t1, -32($fp)
  move $t0, $t1
  sw $t0, -112($fp)
#WRITE *t52
  lw $t6, -112($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t53 := #0
  lw $t0, -116($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -116($fp)
#t54 := &v3 + #8
  la $t1, -32($fp)
  li $t2, 8
  add $t0, $t1, $t2
  sw $t0, -120($fp)
#WRITE *t54
  lw $t6, -120($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t55 := #0
  lw $t0, -124($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -124($fp)
#t56 := #0 - #1
  li $t1, 0
  li $t2, 1
  sub $t0, $t1, $t2
  sw $t0, -128($fp)
#RETURN t56
  lw $t0, -128($fp)
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
#FUNCTION keyword_names : 

_func_keyword_names:
  addi $sp, $sp, -83
  sw $ra, 79($sp)
  sw $fp, 75($sp)
  addi $fp, $sp, 83
#DEC v4 3
#v21 := v22
  lw $t0, -15($fp)
  lw $t1, -19($fp)
  move $t0, $t1
  sw $t0, -15($fp)
#v20 := v21
  lw $t0, -23($fp)
  lw $t1, -15($fp)
  move $t0, $t1
  sw $t0, -23($fp)
#v19 := v20
  lw $t0, -27($fp)
  lw $t1, -23($fp)
  move $t0, $t1
  sw $t0, -27($fp)
#v18 := v19
  lw $t0, -31($fp)
  lw $t1, -27($fp)
  move $t0, $t1
  sw $t0, -31($fp)
#v17 := v18
  lw $t0, -35($fp)
  lw $t1, -31($fp)
  move $t0, $t1
  sw $t0, -35($fp)
#v16 := v17
  lw $t0, -39($fp)
  lw $t1, -35($fp)
  move $t0, $t1
  sw $t0, -39($fp)
#v15 := v16
  lw $t0, -43($fp)
  lw $t1, -39($fp)
  move $t0, $t1
  sw $t0, -43($fp)
#v14 := v15
  lw $t0, -47($fp)
  lw $t1, -43($fp)
  move $t0, $t1
  sw $t0, -47($fp)
#v13 := v14
  lw $t0, -51($fp)
  lw $t1, -47($fp)
  move $t0, $t1
  sw $t0, -51($fp)
#v12 := v13
  lw $t0, -55($fp)
  lw $t1, -51($fp)
  move $t0, $t1
  sw $t0, -55($fp)
#v11 := v12
  lw $t0, -59($fp)
  lw $t1, -55($fp)
  move $t0, $t1
  sw $t0, -59($fp)
#v10 := v11
  lw $t0, -63($fp)
  lw $t1, -59($fp)
  move $t0, $t1
  sw $t0, -63($fp)
#v9 := v10
  lw $t0, -67($fp)
  lw $t1, -63($fp)
  move $t0, $t1
  sw $t0, -67($fp)
#v8 := v9
  lw $t0, -71($fp)
  lw $t1, -67($fp)
  move $t0, $t1
  sw $t0, -71($fp)
#v7 := v8
  lw $t0, -75($fp)
  lw $t1, -71($fp)
  move $t0, $t1
  sw $t0, -75($fp)
#v6 := v7
  lw $t0, -79($fp)
  lw $t1, -75($fp)
  move $t0, $t1
  sw $t0, -79($fp)
#v5 := v6
  lw $t0, -83($fp)
  lw $t1, -79($fp)
  move $t0, $t1
  sw $t0, -83($fp)
#v4 := v5
  lw $t0, -11($fp)
  lw $t1, -83($fp)
  move $t0, $t1
  sw $t0, -11($fp)
#RETURN #0
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
#FUNCTION partial_array_copy : 

_func_partial_array_copy:
  addi $sp, $sp, -288
  sw $ra, 284($sp)
  sw $fp, 280($sp)
  addi $fp, $sp, 288
#DEC v23 100
#DEC v24 100
#v25 := #0
  lw $t0, -212($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -212($fp)
#LABEL label0 : 
label0:
#IF v25 >= #5 GOTO label1
  lw $t0, -212($fp)
  li $t1, 5
  bge $t0, $t1, label1
#t57 := #3 * #20
  li $t1, 3
  li $t2, 20
  mul $t0, $t1, $t2
  sw $t0, -216($fp)
#t58 := &v23 + t57
  la $t1, -108($fp)
  lw $t2, -216($fp)
  add $t0, $t1, $t2
  sw $t0, -220($fp)
#t59 := v25 * #4
  lw $t1, -212($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -224($fp)
#t60 := t58 + t59
  lw $t1, -220($fp)
  lw $t2, -224($fp)
  add $t0, $t1, $t2
  sw $t0, -228($fp)
#t61 := v25 * v25
  lw $t1, -212($fp)
  lw $t2, -212($fp)
  mul $t0, $t1, $t2
  sw $t0, -232($fp)
#*t60 := t61
  lw $t6, -228($fp)
  lw $t0, 0($t6)
  lw $t1, -232($fp)
  move $t0, $t1
  lw $t6, -228($fp)
  sw $t0, 0($t6)
#t62 := v25 + #1
  lw $t1, -212($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -236($fp)
#v25 := t62
  lw $t0, -212($fp)
  lw $t1, -236($fp)
  move $t0, $t1
  sw $t0, -212($fp)
#GOTO label0
  j label0
#LABEL label1 : 
label1:
#t63 := #2 * #20
  li $t1, 2
  li $t2, 20
  mul $t0, $t1, $t2
  sw $t0, -240($fp)
#t64 := &v24 + t63
  la $t1, -208($fp)
  lw $t2, -240($fp)
  add $t0, $t1, $t2
  sw $t0, -244($fp)
#t65 := #3 * #20
  li $t1, 3
  li $t2, 20
  mul $t0, $t1, $t2
  sw $t0, -248($fp)
#t66 := &v23 + t65
  la $t1, -108($fp)
  lw $t2, -248($fp)
  add $t0, $t1, $t2
  sw $t0, -252($fp)
#t67 := t64
  lw $t0, -256($fp)
  lw $t1, -244($fp)
  move $t0, $t1
  sw $t0, -256($fp)
#t68 := t66
  lw $t0, -260($fp)
  lw $t1, -252($fp)
  move $t0, $t1
  sw $t0, -260($fp)
#t69 := &v23 + #100
  la $t1, -108($fp)
  li $t2, 100
  add $t0, $t1, $t2
  sw $t0, -264($fp)
#LABEL label2 : 
label2:
#IF t68 >= t69 GOTO label3
  lw $t0, -260($fp)
  lw $t1, -264($fp)
  bge $t0, $t1, label3
#*t67 := *t68
  lw $t6, -256($fp)
  lw $t0, 0($t6)
  lw $t6, -260($fp)
  lw $t1, 0($t6)
  move $t0, $t1
  lw $t6, -256($fp)
  sw $t0, 0($t6)
#t67 := t67 + #4
  lw $t1, -256($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -256($fp)
#t68 := t68 + #4
  lw $t1, -260($fp)
  li $t2, 4
  add $t0, $t1, $t2
  sw $t0, -260($fp)
#GOTO label2
  j label2
#LABEL label3 : 
label3:
#v25 := #0
  lw $t0, -212($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -212($fp)
#LABEL label4 : 
label4:
#IF v25 >= #5 GOTO label5
  lw $t0, -212($fp)
  li $t1, 5
  bge $t0, $t1, label5
#t70 := #2 * #20
  li $t1, 2
  li $t2, 20
  mul $t0, $t1, $t2
  sw $t0, -268($fp)
#t71 := &v24 + t70
  la $t1, -208($fp)
  lw $t2, -268($fp)
  add $t0, $t1, $t2
  sw $t0, -272($fp)
#t72 := v25 * #4
  lw $t1, -212($fp)
  li $t2, 4
  mul $t0, $t1, $t2
  sw $t0, -276($fp)
#t73 := t71 + t72
  lw $t1, -272($fp)
  lw $t2, -276($fp)
  add $t0, $t1, $t2
  sw $t0, -280($fp)
#WRITE *t73
  lw $t6, -280($fp)
  lw $t0, 0($t6)
  move $a0, $t0
  jal _func_write
#t74 := #0
  lw $t0, -284($fp)
  li $t1, 0
  move $t0, $t1
  sw $t0, -284($fp)
#t75 := v25 + #1
  lw $t1, -212($fp)
  li $t2, 1
  add $t0, $t1, $t2
  sw $t0, -288($fp)
#v25 := t75
  lw $t0, -212($fp)
  lw $t1, -288($fp)
  move $t0, $t1
  sw $t0, -212($fp)
#GOTO label4
  j label4
#LABEL label5 : 
label5:
#RETURN #0
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
#FUNCTION main : 

main:
  addi $sp, $sp, -24
  sw $ra, 20($sp)
  sw $fp, 16($sp)
  addi $fp, $sp, 24
#t76 := CALL array_structure
  jal _func_array_structure
  sw $v0, -12($fp)
#t77 := CALL danger_ops
  jal _func_danger_ops
  sw $v0, -16($fp)
#t78 := CALL keyword_names
  jal _func_keyword_names
  sw $v0, -20($fp)
#t79 := CALL partial_array_copy
  jal _func_partial_array_copy
  sw $v0, -24($fp)
#RETURN #0
  li $t0, 0
  move $v0, $t0
  move $sp, $fp
  lw $ra, -4($fp)
  lw $fp, -8($fp)
  jr $ra
