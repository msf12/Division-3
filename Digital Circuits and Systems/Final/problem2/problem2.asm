	ORG	0000
a	DB	0

	ORG	0010
	LDAA	a
	CMPA	#1
	BLO	less
	BHI	more

equal:
	STAA	8000
	INCA
	JMP	loop

less:
	STAA	8000
	ADDA	#2
	SUBA	#1
	STAA	8000
	INCA
	JMP	less

more:
	STAA	8000
	INCA
	STAA	8000
	ADDA	#2
	SUBA	#1
	JMP	more

;change this byte to change the oscillation pattern
0000	00
...
0010	96 00
0012	81 01
0014	21 0A;[offset to less]
0016	22 15;[offset to more]
;equal
0018	B7 8000
001A	4C
001C	7E 0018
;less
001F	B7 8000
0022	8B 02
0024	80 01
0026	B7 8000
0029	4C
002A	7E 001F
;more
002D	B7 8000
0030	4C
0031	B7 8000
0034	8B 02
0036	80 01
0038	7E 002D