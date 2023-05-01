from z3 import *


file_name="p_01"
with open(file_name+"c.txt") as f:
    capacity = int(f.readline().strip(" "))

i=0
weights = []
weights_c=[]
with open(file_name+"w.txt") as f:
    for line in f:
        weights.append(Int(f"Item_W_{i}"))
        weights_c.append(weights[i]==int(line.strip("\n ")))
        i+=1

prizes=[]
prizes_c=[]
max_prize=0
i=0
with open(file_name+"p.txt") as f:
    for line in f:
        max_prize+=int(line.strip("\n "))
        prizes.append(Int(f"Item_P_{i}"))
        prizes_c.append(prizes[i]==int(line.strip("\n ")))
        i+=1

if(len(prizes)!=len(weights)):raise BaseException("Bad input files (not equal number of prizes and weights)")

Items = [Bool("Item_%s" % (i)) for i in range(len(prizes))]

prize_sum = Sum([If(Items[i],prizes[i],0) for i in range(len(prizes))])

weight_sum = Sum([If(Items[i],weights[i],0) for i in range(len(weights))])


def solve(C):
    s=Solver()
    s.add(weight_sum<=capacity,prize_sum>=C)
    s.add(prizes_c+weights_c)
    heureka=s.check()
    # print(heureka)
    if(heureka==unsat):return False
    m=s.model()
    return m


upper=max_prize
lower=0
last_sol:ModelRef
last_c=0

s=Solver()
C=upper/2
s.add(weight_sum<=capacity,prize_sum>C)
s.add(prizes_c+weights_c)
# print(s)
while upper-lower>1:
    C=int((upper-lower)/2+lower)
    # print(C)
    ret = solve(C)
    if ret==False:
        upper=C
        continue
    last_sol=ret
    lower=C
    last_c=C


if last_c!=0:
    print([last_sol.evaluate(Items[i]) for i in range(len(Items))] )
    print([last_sol.evaluate(weights[i]) for i in range(len(Items))] )
    print([last_sol.evaluate(prizes[i]) for i in range(len(Items))] )

print(last_c)