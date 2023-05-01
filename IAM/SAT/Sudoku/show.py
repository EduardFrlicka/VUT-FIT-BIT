import re

file = open("sudoku.out","r")
lines = file.readlines()

line = re.sub(r"-\d+","",lines[1])
line = re.sub(r"\s+|0"," ",line)
line = line.strip(" ").split(" ")

matrix=[[],[],[],[],[],[],[],[],[]]

for e in line:
    matrix[int(e[0])-1].append(e[2])
for m in matrix:
    print(m)
