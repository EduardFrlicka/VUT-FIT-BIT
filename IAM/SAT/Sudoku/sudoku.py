import sys
import re

lines = [re.sub(r"\s+","",line).strip(" ") for line in sys.stdin.readlines() if (re.sub(r"\s+"," ",line).strip(" ")!="")]
clausules_count=11745+len(lines)
print("p cnf",999,clausules_count)
for line in lines:
    print(line,0)

for r in range(1,10):
    for c in range(1,10):
        for n in range(1,10):
            print("{}{}{}".format(r,c,n),end=" ")
        print("0")
        for n in range(1,10):
            for i in range(n+1,10):
                print("-{}{}{} -{}{}{} 0".format(r,c,n,r,c,i))

for n in range(1,10):
    for r in range(1,10):
        for c in range(1,10):
            for i in range(c+1,10):
                print("-{}{}{} -{}{}{} 0".format(r,c,n,r,i,n))
    for c in range(1,10):
        for r in range(1,10):
            for i in range(r+1,10):
                print("-{}{}{} -{}{}{} 0".format(r,c,n,i,c,n))


for r in range(1,10,3):
    for c in range(1,10,3):
        box = []
        for i in range(r,r+3):
            for j in range(c,c+3):
                box.append(10*i+j)
        for n in range(1,10):
            for i in range(9):
                for j in range(i+1,9):
                    print("-{}{} -{}{} 0".format(box[i],n,box[j],n))

                    