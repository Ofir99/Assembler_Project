
disktest:

add $sp, $zero, $imm, 1500 #$sp=1500
add $sp, $sp, $imm, -3		# adjust stack for 3 items
sw $s0, $sp, $imm, 0		# save $s0
sw $s1, $sp, $imm, 1		# save $s1
sw $s2, $sp, $imm, 2		# save $s2

add $t0, $imm, $zero, 1 #$t0=1
out $t0,$imm,$zero,0 # enable IRQ 0
add $t1, $imm, $zero, 6 #$t1=6
out $imm, $t1, $zero, ChangeMode  #irqhandler=ChangeMode 

add $t1, $imm, $zero, 1024 #$t1=1024
out $t1,$imm,$zero,12 #timermax=1024

add $s0, $zero, $zero,0 #$s0=0 
add $s1, $imm, $zero, 3000 #$s1=3000 
add $s2, $imm, $zero, 1 #$s2=1

Read/WriteToSector:
in $t1, $imm, $zero, 17 #read disk status
bne $imm, $t1,$zero, Read/WriteToSector#if disk is free start reading/writing, else PC=Read/WriteToSector
out $zero, $imm, $zero, 11 #timerenable is off
add $t1,$imm,$zero,8 #$t1=8
beq $imm, $t1,$s0,END  #if disksector ==8, PC=END
out $s0 ,$imm,$zero,15 #disksector=$s0
out $s1 ,$imm,$zero,16 #disksbuffer=$s1
out $s2 ,$imm,$zero,14 #diskcmd=$s2
out $t0, $imm, $zero, 11 #timerenable is on
beq $imm, $zero, $zero, Read/WriteToSector #jump to Read/WriteToSector

ChangeMode:
out $zero, $imm, $zero, 3 #set IRQ0 STATUS to 0
add $s1, $imm, $s1, 128 #$s1=$s1+128
add $s0, $imm, $s0, 1 #$s0++
add $t1,$imm,$zero,4 #$t1=4
beq $imm, $t1,$s0,ChangeDiskCMD  #if disksector ==4, PC=ChangeDiskCMD
RETURN: reti $zero, $zero, $zero, 0	# return from interrupt

ChangeDiskCMD:
add $s0, $imm, $zero,4 #$s0=4
add $s1, $imm, $zero, 3000 #$s1=3000
add $s2, $imm, $zero, 2 #$s2=2
beq $imm, $zero, $zero, RETURN#jump to RETURN


END:
lw $s0, $sp, $imm, 0		# restore $s0
lw $s1, $sp, $imm, 1		# restore $s1
lw $s2, $sp, $imm, 2		# restore $s2
add $sp, $sp, $imm, 3		# pop 3 items from stack
