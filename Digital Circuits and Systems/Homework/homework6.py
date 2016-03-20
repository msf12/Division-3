Q1 = 1
Q0 = 0

def NOT( a ):
    return 1 - a

for step in range( 20 ):

    J1 = Q1 | Q0
    K1 = NOT(Q1) + Q0
    J0 = K1
    K0 = Q1 + NOT(Q0)
    G = Q1 & Q0
    Y = NOT(Q1) & Q0
    R = NOT(Q0)

    print( "Q1 Q0 = %d %d | G Y R = %d %d %d" %
     ( Q1, Q0, G, Y, R ) )
    input( "> " )

    if(J1 ^ K1 == 1):
        Q1 = J1
    elif(J1 == 1):
        Q1 = NOT(Q1)

    if(J0 ^ K0 == 1):
        Q0 = J0
    elif(J0 == 1):
        Q0 = NOT(Q0)