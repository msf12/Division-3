def decoder(a,b,c,E):
	D0 = not a and not b and not c and E
	D1 = not a and not b and c and E
	D2 = not a and b and not c and E
	D3 = not a and b and c and E
	D4 = a and not b and not c and E
	D5 = a and not b and c and E
	D6 = a and b and not c and E
	D7 = a and b and c and E
	return [D0, D1, D2, D3, D4, D5, D6, D7]

def main():
	print( "  E  a  b  c  |  D7  D6  D5  D4  D3  D2  D1  D0" )
	print( "--------------+----------------------------------" )
	for E in [ 0, 1 ]:
		for a in [ 0, 1 ]:
			for b in [ 0, 1 ]:
				for c in [ 0, 1 ]:
					print( "%3d%3d%3d%3d  |" % 
						( E, a, b, c), end="")
					result = decoder(a, b, c, E)
					result.reverse()
					for D in result:
						print("%4d" % D, end="")
					print()

main()