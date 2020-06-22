quick sort: # sorting an array in adreesses 1024-1039

add $a0,$zero,$imm,1024 #$a0=1024
add $a1, $zero, $imm, 1039 # $a1= 1039
add $sp, $zero, $imm, 1		# set $sp = 1
sll $sp, $sp, $imm, 11		# set $sp = 1 << 11 = 2048
jal $imm, $zero, $zero, qsort
halt $zero,$zero,$zero,0 # exit program

qsort:
add $sp, $sp, $imm, -4 #adjust stack for 4 item
sw $a0, $sp, $imm,3 #saving $a0
sw $a1, $sp, $imm,2 #saving $a1
sw $v0, $sp, $imm,1 #saving $v0
sw $ra, $sp, $imm,0 #saving $ra

bge $imm, $a0, $a1,END #if a0>=a1, pc=$ra
jal $imm, $zero, $zero, partition #otherwise,branch to partition

add $a1, $v0, $imm, 0 # a1= $v0 (partition)
jal $imm, $zero, $zero, qsort #qsort from a0 to partition
add $a0, $v0, $imm, 1 # $a0=$v0+1
lw $a1, $sp, $imm, 2 #restore $a1
jal $imm, $zero, $zero, qsort #qsort from partition+1 to a1

END:
lw $ra, $sp, $imm, 0 #restore $ra
lw $v0, $sp, $imm, 1 #restore $v0
lw $a1, $sp, $imm, 2 #restore $a1
lw $a0, $sp, $imm, 3 #restore $a0
add $sp, $sp,$imm, 4 #pop 4 item from stack
beq $ra, $zero, $zero, 0 #pc=R[$ra] 

partition:# we define pivot=Mem[$a0] and $vo=partition, all the numbers to the left of partition will be lower or equal to pivot and to the right greater or equal to pivot.
add $t1, $a0, $imm,-1 #$t1= $a0 -1
add $t2, $a1, $imm, 1 # $t2= $a1+1

Loop:
lw $t0, $a0, $imm, 0 # $t0= Mem[$a0] (defining the pivot)

For1: #finding t1, such that $t0 <= Mem[t1]
add $t1, $t1, $imm,1 #$t1=$t1+1
lw $t3, $t1, $imm, 0 # t3= Mem[t1]
blt $imm , $t3, $t0,For1 #if Mem[t1]<$t0, loop again

For2: #finding t2, such that Mem[t2]=<t0
add $t2, $t2, $imm,-1 #$t2=$t2-1
lw $t3, $t2, $imm, 0 # t3= Mem[t2]
bgt $imm, $t3, $t0,For2 #if $t3>$t0, loop again

add $v0, $t2, $imm, 0 # $v0= $t2 
ble $ra, $t2, $t1, 0 #if t2<= t1, return; otherwise swap Mem[t1] and Mem[t2] and loop again
lw $t3, $t1, $imm, 0# $t3 = Mem[$t1] ( swaping $t2 with $t1)
lw $t0, $t2, $imm, 0 #$t0 = Mem[$t2]
sw $t3, $t2, $imm, 0 # Mem[$t2]= $t3
sw $t0, $t1, $imm, 0 # Mem[$t1]= $t0
beq $imm, $zero, $zero, Loop #more iteration


.word 1024 -5
.word 1025 25
.word 1026 10
.word 1027 -7
.word 1028 15
.word 1029 100
.word 1030 500
.word 1031 20
.word 1032 -10
.word 1033 80
.word 1034 99
.word 1035 1000
.word 1036 3000
.word 1037 55
.word 1038 2500
.word 1039 77