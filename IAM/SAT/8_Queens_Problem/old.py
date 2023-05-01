import sys


def bishop_clausules_count(N:int):
    A, B = N-1, N-1
    sum = 0
    while A >= 0:
        if(A == 0):
            sum+=B
            break
        sum+=A*B*4
        A-=2
        B+=2
    return sum

def rook_clausules_count(N:int):
    return N*N*(N-1)*2

def rook_clausules(place, N):
    # ROWS
    column = place % N
    row = place - column
    place+=1
    column += 1
    tmp = column
    
    for i in range(1, N+1):
        if(column!=i):
            print(-place, -(row+i),0)

    for i in range(N):
        if(place != tmp):
            print(-place, -tmp,0)
        tmp+=N

    return

def in_border(pos,N):
    if(pos[0]>=0 and pos[0]<N and pos[1]>=0 and pos[1]<N):
        return True
    return False

def move(pos, vector):
    pos[0]+=vector[0]
    pos[1]+=vector[1]
    return pos

def bishop_clausules(place,N):
    vectors = [(1,1),(1,-1),(-1,1),(-1,-1)]
    def_pos = [place//N, place%N]
    place+=1
    for v in vectors:
        pos = def_pos[:]
        move(pos, v)
        while in_border(pos,N):
            print(-place,-(pos[0]*N+pos[1]+1),0)
            move(pos, v)

    return


if(len(sys.argv)!=2):
    exit()

if(not sys.argv[1].isdecimal()):
    exit()

N = int(sys.argv[1])
print("c\tN queens problem\nc\tN =", N)
print("c")
clausules_count = 0
clausules_count += N
clausules_count += bishop_clausules_count(N)
clausules_count += rook_clausules_count(N)

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

