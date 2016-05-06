	ORG	0000
a	DB	0 ;0 = fast, 1 = medium, 2 = slow

	ORG	0010
	LDAA	a ;load frequency
	CMPA	#1 ;compare to 1
	BLO	less ;if frequency is less than 1 jump to less
	BHI	more ;if frequency is more than 1 jump to more

;if the frequency option is 1: output ACCA, increment it, store it on the stack,
;wait one second, pop ACCA from the stack, and jump to equal
equal:
	STAA	8000
	INCA
	PSHA
	JSR	C027
	PULA
	JMP	equal

;if the frequency option is 0: output ACCA, increment it, and jump to less
less:
	STAA	8000
	INCA
	JMP	less

;if the frequency option is 2: output ACCA, increment it, store it on the stack,
;wait one second, pop ACCA from the stack, push ACCA back onto the stack,
;wait one second, pop ACCA from the stack, and jump to more
;the middle pop and push are to ensure that ACCA remains at the top of the stack
;after two JSR commands
more:
	STAA	8000
	INCA
	PSHA
	JSR	C027
	PULA
	PSHA
	JSR	C027
	PULA
	JMP	more

;change this byte to change the oscillation frequency
0000	00
...
0010	96 00
0012	81 01
0014	25 0E;[offset to less]
0016	22 13;[offset to more]
;equal
0018	B7 8000
001B	4C
001C	36
001D	BD C027
0020	32
0021	7E 0018
;less
0024	B7 8000
0027	4C
0028	7E 0024
;more
002B	B7 8000
002E	4C
002F	36
0030	BD C027
0033	32
0034	36
0035	BD C027
0038	32
0039	7E 0029