add $sp, $zero, $imm, 1		# set $sp = 1
sll $sp, $sp, $imm, 11		# set $sp = 1 << 11 = 2048
add $sp, $sp, $imm, -2	# allocate space in stack
sw $s0, $sp, $imm, 0	# store current value of $s0
sw $ra, $sp, $imm, 1	# store current $ra in stack

add $s0, $zero, $zero, 0	# initialize i to 0
IF:	
add $t0, $zero, $imm, 16	# const - number of cells in matrix 4x4
bge $imm, $s0, $t0, ELSE	# if(i>=16) jump to ELSE
lw $t1, $imm, $s0, 0X100	# $t1 = Mat1[i] (0X100 = Mat1[0])
lw $t2, $imm, $s0, 0X110	# $t2 = Mat2[i] (0X110 = Mat2[0])
add $t3, $t1, $t2, 0		# $t3 = Mat1[i] + Mat2[i]
sw $t3, $imm, $s0, 0X120	# SUM[i] = Mat1[i] + Mat2[i]
add $s0, $s0, $imm, 1	# finished iteration, i++
jal $imm, $zero, $zero, IF	# jump to IF

ELSE:
lw $s0, $sp, $imm, 0	# restore $s0 from stack
lw $ra, $sp, $imm, 1	# restore $ra from stack
add $sp, $sp, $imm, 2	# release allocated space
halt $zero, $zero, $zero, 0	#halt execution - exit

.word 0x100 0xf
.word 0x101 1
.word 0x102 2
.word 0x103 3
.word 0x104 1000
.word 0x105 2500
.word 0x106 -500
.word 0x107 5
.word 0x108 3
.word 0x109 3
.word 0x10a 3
.word 0x10b 3
.word 0x10c 3
.word 0x10d 3
.word 0x10e 3
.word 0x10f 3

.word 0x110 15
.word 0x111 0xffffffff
.word 0x112 100
.word 0x113 1000
.word 0x114 1000
.word 0x115 1
.word 0x116 3
.word 0x117 3
.word 0x118 3
.word 0x119 3
.word 0x11a 3
.word 0x11b 3
.word 0x11c 3
.word 0x11d 3
.word 0x11e 3
.word 0x11f 3