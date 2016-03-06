# Mitchel Fields
# Mux simulator for f(A,B,C,D) and g(A,B,C,D)

def NOT(a):
	return 1 - a

def mux(l0, l1, l2, l3, C1, C0):
	if (C1 & C0) == 1:
		return l0
	elif (NOT(C1) & C0) == 1:
		return l1
	elif (C1 & NOT(C0)) == 1:
		return l2
	else:
		return l3

def main():

	print( "  A  B  C  D  |  f  g  " )
	print( "--------------+--------" )

	for A in [0,1]:
		for B in [0,1]:
			for C in [0,1]:
				for D in [0,1]:
					f = mux(1,0,0,1,B,D)
					g = NOT(mux(B,0,0,NOT(B),A,D))

					print("%3d%3d%3d%3d  |%3d%3d" % 
						(A, B, C, D, f, g))

main()