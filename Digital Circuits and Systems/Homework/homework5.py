Q2 = 1
Q1 = 1
Q0 = 1

def NOT( a ):
    return 1 - a

for step in range( 20 ):

    D2 = Q2 & (Q1 | Q0)
    D1 = Q0 & (NOT(Q2) | NOT(Q1))
    D0 = (NOT(Q2) & NOT(Q1)) | (Q2 & Q0)
    R = (Q2 & NOT(Q0)) | Q1
    Y = NOT(Q2) & (Q1 ^ Q0)
    G = NOT(Q2) & NOT(Q1) & NOT(Q0)

    print( "Q2 Q1 Q0 = %d %d %d | R Y G = %d %d %d" %
     ( Q2, Q1, Q0, R, Y, G ) )
    input( "> " )

    Q0 = D0
    Q1 = D1
    Q2 = D2