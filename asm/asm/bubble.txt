#bubble sort of array 1924-1939  : 


		add $sp, $zero, $imm, 1
sll $sp, $sp, $imm, 11		# set $sp = 1 << 11 = 2048
		add $sp, $sp, $imm, -4 # adjust stack for 3 items
		sw $ra, $sp, $imm, 3 #save $ra
		sw $s0, $sp, $imm, 2 #save $s0
		sw $s1, $sp, $imm, 1 #save $s1
		sw $s2, $sp, $imm, 0 #save $s2


		add $a1, $zero, $imm, 1924 		# set $a1=&array[0]
		add $a0, $zero, $imm, 15 		# set $a0=n-1=15 
		add $t0, $zero, $imm, 0 		# set $to=i=0
FOR1:		bge $imm, $t0, $a0, END 	# beginning of FOR1, if i<n-1 than continue, else jump to end_of_func
		add $t3, $zero, $imm, 0 		# set $t3=swapped=false
		add $t1, $zero, $imm, 0 		# set j=0
		sub $t2, $a0, $t0, 0 		# set $t2= n-i-1
FOR2:		bge $imm, $t1, $t2, IF1 		# beginning of FOR2, if j<n-i-1 continue, else jump to IF1
		lw $s0, $a1, $t1, 0 			# $s0 = array[j]
		add $s1, $t1, $imm, 1 		# $s1= j+1
		lw $s2, $a1, $s1, 0 			# $s2= array[j+1]
		ble $imm, $s0, $s2, j_rise 		# if array[j]>array[j+1] continue, else jump to FOR2 

		sw $s2, $a1, $t1, 0 			# array[j]= array[j+1]
		sw $s0, $a1, $s1,0 			# array[j+1]=prev array[j]
		add $t3, $zero, $imm, 1 		# set swapped= true = 1 
j_rise:		add $t1, $t1, $imm, 1		# j++
		beq $imm, $zero, $zero, FOR2 	# jump to FOR2
IF1:		beq $imm, $t3, $zero, END 		# if swapped=false, break
		add $t0, $t0, $imm, 1 		# i++
		beq $imm, $zero, $zero, FOR1   
END:		lw $s2, $sp, $imm, 0 		# restore $s2
		lw $s1, $sp, $imm, 1 		# restore $s1
		lw $s0, $sp, $imm, 2		# restore $s0
		lw $ra, $sp, $imm, 3		# restore $ra from stack



		sub $sp, $sp, $imm, 4 		# free stack 


.word 1924 15
.word 1925 14
.word 1926 13
.word 1927 12
.word 1928 11
.word 1929 10
.word 1930 9
.word 1931 8
.word 1932 7
.word 1933 6
.word 1934 5
.word 1935 4
.word 1936 3
.word 1937 2
.word 1938 1
.word 1939 1
halt $zero, $zero, $zero, 0	# halt

		
		
		
	
	
	
	
