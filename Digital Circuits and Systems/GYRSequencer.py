# GYRSequencer.py
# D. Thiebaut
# A very quick and dirty way to check a sequencer...
# This sequencer activates a G, Y, and R light system
# s.t. G is on for 2 cycles, followed by Y for 1 cycle
# followed by R for 1 cycle.  Then the whole cycle repeats

Q1 = 0
Q2 = 0
Q3 = 0

def NOT( a ):
    return 1 - a

for step in range( 20 ):

    # the Q1 and Q2 outputs go through combinational logic to generate the new values
    # of D1, D2, and the outputs G, Y, R...
    D1 = NOT( Q2 )
    D2 = (Q1 | Q2) & NOT(Q3)
    D3 = NOT(Q1) & Q2 & NOT(Q3)
    G  = NOT(Q1 | Q2 | Q3)
    Y  = Q2 & Q3 | Q1 & NOT(Q2)
    R  = Q2

    # show the stable circuit signals
    print( "Q3 Q2 Q1 = %d %d %d  | GYR = %d %d %d" % ( Q3, Q2, Q1, G, Y, R ) )

    # wait for the next clock tick (the user presses Enter)
    input( "> " )

    # as soon as the clock has ticked, D1 and D2 get latched in the flipflops
    # and Q1 and Q2 reflect the values captured.
    Q1 = D1
    Q2 = D2
    Q3 = D3