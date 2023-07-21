# x20 number of unprocessed items in array left
# x21 address to the next item in array
# x12 actual number loaded from memory (function argument)
# x28 temporary value for prime test comparings
# x29 temporary remainder
# x10 flag whether number is prime or not (function return value)


# load init info from memory
lw x20, 4(x0)
lw x21, 8(x0)

## loop through number array
loop: 
	# load number from address
	lw x12, 0(x21)
	
	# check if number is prime and save the result back to memory
	jal x1, prime
	sw x10, 0(x21)
	
	# update values and check condition
	addi x20, x20, -1
	addi x21, x21, 4
	blt x0, x20, loop

end: jal x0, end 


# FUNCTIONS

prime:
	# check if number is 0, 1 or 2
	beq x12, x0, prime_decline # 0 is not a prime
	addi x28, x0, 1
	beq x12, x28, prime_decline # 1 is not a prime
	addi x28, x0, 2
	beq x12, x28, prime_accept # 2 is a prime

	# otherwise loop through all integers in range <2,tested_number) and test divisibility
	loop_prime:
		rem x29, x12, x28
		beq x29, x0, prime_decline # number is divisible by some lesser number --> not a prime
	
		# update values and check condition
		addi x28, x28, 1
		blt x28, x12, loop_prime
	# not divisible by any lesser number --> is a prime
		
	prime_accept:
		addi x10, x0, 1
		jal x0, prime_end
	prime_decline:	
		addi x10, x0, 0	
		
	prime_end:
		jalr x0, x1, 0