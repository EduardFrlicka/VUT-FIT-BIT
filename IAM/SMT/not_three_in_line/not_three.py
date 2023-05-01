from sys import argv
from z3 import *


if(len(argv)!=3):
    print("Usage: python3 not_three.py [N] [size]")
    exit(0)

N=int(argv[1])
size=int(argv[2])

Tiles = [ [Bool("Tile_%s_%s" % (i,j)) for i in range(size)] for j in range(size)]

number = sum([If(Tiles[i][j],1,0)for i in range(size) for j in range(size)])

def line_c(x1,y1,x2,y2):
    #  print(x1,y1,"  ",x2,y2)
    if(x1-x2==0):
        return [Not(Tiles[x1][y]) for y in range(size)if y != y1 and y != y2]

    if(y1-y2==0):
        return [Not(Tiles[x][y2]) for x in range(size)if x != x1 and x != x2]


    # print(Tiles[x1][y1],Tiles[x2][y2])
    # print([
    # Not(Tiles[i][j]) 
    # for i in range(size) for j in range(size)
    # if (not (x1==i and y1==j or x2==i and y2==j)) and (x1-i)/(x1-x2)==(y1-j)/(y1-y2)
    # ])
    return [
    Not(Tiles[i][j]) 
    for i in range(size) for j in range(size)
    if (not (x1==i and y1==j or x2==i and y2==j)) and (x1-i)/(x1-x2)==(y1-j)/(y1-y2)
    ]

constrains=[]

for x1,x2,y1,y2 in [(i1,i2,j1,j2) for i1 in range(size) for j1 in range(size)
    for i2 in range(size) for j2 in range(size)
    if (i1!=i2 or j1!=j2)]:
    line_constrains=line_c(x1,y1,x2,y2)
    line_len=len(line_constrains)
    if(line_len==0):
        continue
    if(line_len==1):
        constrains.append(Implies(And(Tiles[x1][y1],Tiles[x2][y2]),line_constrains[0])) 
    if line_len>1:
        constrains.append(Implies(And(Tiles[x1][y1],Tiles[x2][y2]),And(line_constrains))) 



# constrains = [Implies(And(Tiles[i1][j1],Tiles[i2][j2]),And(line_c(i1,j1,i2,j2)))    
#     for i1 in range(size) for j1 in range(size)
#     for i2 in range(size) for j2 in range(size)
#     if (i1!=i2 or j1!=j2) and (line_c(i1,j1,i2,j2))!=[]
# ]

s=Solver()
s.add(number==N)
s.add(constrains)
with open("SMT.txt","w") as file:
    file.write(s.sexpr())
print("start")
s.set("timeout", 180000)
ret = s.check()
print(ret)
if ret == sat:
    m = s.model()
    r = [ " ".join(["0" if "True"==str(m.evaluate(Tiles[i][j])) else "-" for j in range(size) ])
            for i in range(size) ]
    print_matrix(r)
    # print_matrix([[Tiles[i][j] for j in range(size) ] 
    #         for i in range(size) ])
exit(0)