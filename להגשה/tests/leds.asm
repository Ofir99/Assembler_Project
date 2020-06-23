		add $sp, $zero, $imm, 1
		sll $sp, $sp, $imm, 11		# set $sp = 1 << 11 = 2048
		add $sp, $sp, $imm, -2 # adjust stack for 3 items
		sw $ra, $sp, $imm, 1 #save $ra
		sw $s0, $sp, $imm, 0 #save $s0
		add $t0, $zero, $zero, 0 # set t0=0
		add $s0, $zero, $imm, 1 #set s0=1
		add $t1, $zero, $imm, 32 # set t1=32 
	LOOP1:	beq $imm, $t1, $zero, end # if t1=0 jump to label end 
		out $s0, $imm, $zero, 9 # set leds
		add $t2, $imm, $zero, 82 # t2= number of cycles 
	LOOP2:  beq $imm, $t2, $zero, raise # if the second over almost over, jump to raise 
		sub $t2, $t2, $imm, 1 # t2=t2-1
		beq $imm, $zero, $zero, LOOP2 # jump to loop2
	raise:	sll $s0, $s0, $imm, 1 # s0<<1
		sub $t1, $t1, $imm, 1 # t1=t1-1
		add $zero, $zero, $imm, 0 #do nothing
		add $zero, $zero, $imm, 0 #do nothing
		add $zero, $zero, $imm, 0 #do nothing
		beq $imm, $zero, $zero, LOOP1 # jump to loop1
	end:	out $s0, $imm, $zero, 9 # set leds
		lw $s0, $sp, $imm, 1		# restore $s0
		lw $ra, $sp, $imm, 0		# restore $ra from stack



		sub $sp, $sp, $imm, 2 		# free stack	
halt $zero, $zero, $zero, 0	# halt