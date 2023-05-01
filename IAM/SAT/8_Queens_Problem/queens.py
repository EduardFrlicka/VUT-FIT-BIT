import sys

def bishop_clausules_count(N:int):
    A, B = N-1, N-1
    sum = 0
    while A >= 0:
        if(A == 0):
            sum+=int(B/2)
            break
        sum+=A*B*2
        A-=2
        B+=2
    return sum

def rook_clausules(place, N):
    print_place=place+1
    for i in range(print_place+1, place - place % N + N + 1):
        print("-{} -{} 0".format(print_place,i))
    for i in range(print_place+N, N*N-N+place%N+2,N):
        print("-{} -{} 0".format(print_place,i))
    return

def bishop_clausules(place,N):
    print_place=place+1
    max=N*N
    for i in range(print_place+N+1,print_place+(N-place%N-1)*(N+1)+1 ,N+1):
        if(i>max):break
        print("-{} -{} 0".format(print_place,i))

    for i in range(print_place+N-1,print_place + (place%N)*(N-1)+1,N-1):
        if(i>max):break
        print("-{} -{} 0".format(print_place,i))


    return

if(len(sys.argv)!=2):
    exit()

if(not sys.argv[1].isdecimal()):
    exit()

N = int(sys.argv[1])
print("c\tN queens problem\nc\tN =", N)
print("c")
clausules_count = bishop_clausules_count(N) + N*N*(N-1) + N

print("p cnf",N*N,clausules_count)
print("c --------------------- ROWS")

for i in range( N ):
    for j in range(1,N+1):
        print(j+i*N, end=" ")
    print(0)

print("c --------------------- SHOULDERING")

for i in range(N*N):
    print("c ##",i,"##")
    rook_clausules(i,N)
    bishop_clausules(i,N)