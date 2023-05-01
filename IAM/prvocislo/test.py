
import math


def isPrvocislo(n):
    print("Cislo:", n)
    if n % 3 == 0:
        print("pass 3")
        return
    if n % 73 == 0:
        print("pass 73")
        return

    for i in range(2, 1000):
        if n % i == 0:
            print(i)


def getNumber(n):
    sum = 0
    c = 1
    for i in range(0, n):
        sum += c
        c *= 10000
    return sum


for i in range(2, 1000):
    if i % 2 == 0:
        continue
    if i % 3== 0:
        continue
    if i % 5 == 0:
        continue
    if i % 7 == 0:
        continue
    if i % 2 == 0:
        continue
    if i % 2 == 0:
        continue

    if i%13==0:
        print("     ", i, ":  ", end="")
        c = getNumber(i)
        isPrvocislo(c)

# isPrvocislo(100010001000100010001)
