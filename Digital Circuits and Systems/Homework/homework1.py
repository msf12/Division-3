def figureA(A,B,C,D,E):
	return (A and B) or (C and (D or E))

def figureB(A,B,C,D,E):
	return (A and B) or (C and D) or (C and E)
	
def main():
    print( "  A  B  C  D  E  |  f  g" )
    print( "-----------------+---------" )
    for A in [ 0, 1 ]:
        for B in [ 0, 1 ]:
            for C in [ 0, 1 ]:
                for D in [ 0, 1 ]:
                    for E in [ 0, 1 ]:
                        print( "%3d%3d%3d%3d%3d  |%3d%3d" % 
                         ( A, B, C, D, E,
                         figureA( A, B, C, D, E ),
                         figureB( A, B, C, D, E ) ) )

main()