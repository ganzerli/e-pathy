# e-pathy
e-pathy saves integers (also in a file) in a way that you can find them following a path. /0/1/50/0 for example.
Is currently missing a garbage collection, so you cannot delete stuff right now, but the program is done in the way that is appliable.

Example:
/[1][2][100][3][99][11]
     |
      -> /[2]/[1][2][3]
               |  |  |
            []<-  |   -> /[2]/[3]/ [0]
           []     |                [1]
          []      V                [2] --> 0,1,2,3
              [][][][][][][]              

like that is also a possibility

Compile linux with:
gcc -o e-pathy e-pathy.c
