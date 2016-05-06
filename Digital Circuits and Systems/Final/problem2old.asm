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
	ADDA	#2
	SUBA	#2
	JMP	equal

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
0014	25 0D;[offset to less]
0016	22 19;[offset to more]
;equal
0018	B7 8000
001B	4C
001C	8B 02
001E	80 02
0020	7E 0018
;less
0023	B7 8000
0026	8B 02
0028	80 01
002A	B7 8000
002D	4C
002E	7E 001F
;more
0031	B7 8000
0034	4C
0035	B7 8000
0038	8B 02
003A	80 01
003C	7E 002D