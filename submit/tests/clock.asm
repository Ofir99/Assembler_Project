#clock test

add $sp, $zero, $imm, 1		# set $sp = 1
sll $sp, $sp, $imm, 11		# set $sp = 1 << 11 = 2048
add $sp, $sp, $imm, -3		# allocate space in stack
sw $s1, $sp, $imm, 0		# store current value of $s0
sw $s0, $sp, $imm, 1		# store current value of $s0
sw $ra, $sp, $imm, 2		# store current $ra in stack

add $s0, $zero, $zero, 0		#s0=0
add $s1, $zero, $imm, 125		#s1=125

add $t3, $zero, $imm, 5		# t3=5
add $t2, $zero, $imm, 1		# t2=1
add $t1, $zero, $imm, 10		# t1=10 
add $t0, $zero, $imm, 0x195		# t0=0X00000195
sll $t0, $t0, $imm, 12		# t0=0X00195000
add $t0, $t0, $imm, 1194		# t0=0X00195955 initial clock value (max value for imm=2047) part 1: 1194
add $t0, $t0, $imm, 1195		# t0=0X00195955 initial clock value. part 2: 1195

out $t0, $t1, $zero, 0 		# IORegister[t1=10]=current clock t0

add $zero, $zero, $imm, 0		# do nothing, waste clock cycle
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle ???
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle

RUN1:				# 250 instructions + one of them is jump to UP1
add $s0, $s0, $imm, 1		# s0++
bne $imm, $s0, $s1, RUN1		#if(s0!=(250/2=125)) do run1 again

UP1:
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle
add $t0, $t0, $imm, 0X1		# sec++
out $t0, $t1, $zero, 0 		# IORegister[t1=10]=current clock t0
add $s0, $zero, $imm, 0		# s0=0
add $t2, $t2, $imm, 1		# t2++		
bne $imm, $t2, $t3, RUN1 		# if(t2!=5) go to waste instructions if we didnt finish part one of the clock	

RUNm:				# 250 instructions + one of them is jump to UP1
add $s0, $s0, $imm, 1		# s0++
bne $imm, $s0, $s1, RUNm		#if(s0!=(250/2=125)) do run1 again

add $t0, $zero, $imm, 0X200	# t0=0X00000200
sll $t0, $t0, $imm, 12		# t0=0X00200000  initial part 2 clock value
out $t0, $t1, $zero, 0 		# IORegister[t1=10]=current clock t0 ???
add $s0, $zero, $imm, 0		# s0=0

add $zero, $zero, $imm, 0		# do nothing, waste clock cycle ???
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle ???
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle ???

RUN2:				# 256-2(t2++, bne)-2(add, sll)-2(UP2)=250 ---> 250 ins + one of them is jump to UP2	
add $s0, $s0, $imm, 1		# s0++ it starts with value of s0=125, we want it to do 125 
bne $imm, $s0, $s1, RUN2		#if(s0!=125) do run2 again

UP2:
add $t0, $t0, $imm, 0X1		# sec++
out $t0, $t1, $zero, 0 		# IORegister[t1=10]=current clock t0
add $t2, $t2, $imm, 1		# t2++	

add $s0, $zero, $imm, 0		# s0=0
add $zero, $zero, $imm, 0		# do nothing, waste clock cycle
bne $imm, $t2, $t1, RUN2 		# if(t2!=10) go to waste instructions if we didnt finish part 2 of the clock

lw $s1, $sp, $imm, 0		# restore $s1 from stack
lw $s0, $sp, $imm, 1	 	# restore $s0 from stack
lw $ra, $sp, $imm, 2		# restore $ra from stack
add $sp, $sp, $imm, 3		# release allocated space
halt $zero, $zero, $zero, 0		#halt execution - exit
