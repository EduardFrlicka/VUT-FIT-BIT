from math import sqrt
from os import close
from sys import argv

row = "[ {y:2d}, {x:2d} ]:  {G:2.2f}  {H:2.2f}  {F:2.2f}".format

class point:
    x:int
    y:int

    def __init__(self,x,y):
        self.x=x
        self.y=y

    def __str__(self) -> str:
        return f"x: {self.x}, y: {self.y}"

    def __eq__(self, o: object) -> bool:
        if(isinstance(o,self.__class__)): 
            if(self.x==o.x and self.y==o.y):
                return True
            return False
        if(isinstance(o,tile)):
            if(self.x==o.P.x and self.y==o.P.y):
                return True
            return False
        raise BaseException(f"Type point cannot be compared to {type(o)}") 
    
    def __add__(self,other:object):
        if(not isinstance(other,self.__class__)): raise f"cannot add point and {type(other)}"
        return self.__class__(self.x+other.x,self.y+other.y)

    def __sub__(self,other:object):
        if(not isinstance(other,self.__class__)): raise f"cannot sub point and {type(other)}"
        return self.__class__(self.x-other.x,self.y-other.y)

    def __hash__(self) -> int:
        return self.x*300 + self.y

class tile:
    H_value=0
    G_value=0
    F_value=0
    parent:object
    P:point

    def __str__(self) -> str:
        return row(x=self.P.x,y=self.P.y,G=self.G_value,H=self.H_value,F=self.F_value)

    def toPrint(self)->str:
        return f"[{self.P.y},{self.P.x}] {self.F_value}"

    def __init__(self,P:point, parent=False):
        self.parent=parent
        self.P=P
        self.calc()
    
    def calc(self):
        self.H_value=self.pointsDistance(End)
        if self.parent!=False:
            self.G_value=round(self.parent.G_value + self.pointsDistance(self.parent.P),2)
        else:
            self.G_value=0
        self.F_value=round(self.H_value+self.G_value,2)
    
    def consider_new_parent(self, parent):
        # print(f"{self}    Curr: {self.parent}    Considering: {parent}")
        global cnt
        tmp_G=round(parent.G_value+self.pointsDistance(parent.P),2)
        if tmp_G<self.G_value:
            self.G_value=tmp_G
            self.F_value=round(self.H_value+self.G_value,2)
            self.parent=parent
            path_file.write(f"{cnt:<3d}C{self}\n")
            cnt+=1

    def pointsDistance(self,P:point):
        Vector=self.P-P
        return round(sqrt(Vector.x*Vector.x+Vector.y*Vector.y),2) 


class openedStack:
    stack={}
    
    def __init__(self, P:point):
        self.stack={}
        self.stack.update({P:tile(P)})

    def __getitem__(self,i)-> tile:
        return self.stack.get(i)

    def get(self,P:point):
        return self.stack.get(P)

    def getTile(self) -> tile:
        if(len(self.stack)==0):return False
        min_value=self.stack.values().__iter__().__next__().F_value
        min_key=self.stack.values().__iter__().__next__().P
        for key,value in self.stack.items():
            if(min_value>value.F_value):
                min_value=value.F_value
                min_key=key
        return self.stack.__getitem__(min_key)

    def remove(self, T:tile):
        self.stack.pop(T.P)

    def addTile(self,P:point,parent:tile=False):
        global cnt
        T=tile(point(P.x,P.y),parent)
        path_file.write(f"{cnt:<3d} {T}\n")
        cnt+=1
        self.stack.update({P:T})


vectors=[
    point(-1,-1),
    point(0,-1),
    point(1,-1),
    point(-1,0),
    point(1,0),
    point(-1,1),
    point(0,1),
    point(1,1)
]


def isWall(P:point):
    for wall in Walls:
        if wall==P: return True
    return False


def processTile(processing_tile:tile):
    path_file.write(f"P:{' '*41}{processing_tile}\n")
    for v in vectors:
        # print(v)
        toAdd=processing_tile.P+v
        if(isWall(toAdd)):
            continue
        if(toAdd in Closed):
            continue
        if(toAdd in Opened.stack):
            Opened[toAdd].consider_new_parent(processing_tile)
            continue
        Opened.addTile(toAdd,processing_tile)
    Closed.append(processing_tile)


def printStacks():
    stacks_file.write("Opened: ")
    for v in Opened.stack.values():
        stacks_file.write(f"{v.toPrint()}\t")
    stacks_file.write("\nClosed: ")
    for t in Closed:
        stacks_file.write(f"{t.toPrint()}\t")
    stacks_file.write("\n\n")


def printPath(last:tile):
    path = []
    while type(last)==tile:
        path.append(last.toPrint())
        last=last.parent
    path_file.write("\n\nPath:\t")
    for i in path[::-1]:
        path_file.write(f"{i}\t")
    path_file.write("\n")
def main():
    if len(argv)!=6:
        print("Usage: python3 main.py [Start_X] [Start_Y] [End_X] [End_Y] [Walls_File]")
        print("\tStart_X - is column number of starting tile")
        print("\tStart_Y - is row number of starting tile")
        print("\tEnd_X - is column number of ending tile")
        print("\tEnd_Y - is row number of ending tile")
        print("\tWalls_File - is name of file, where are written walls")
        print("\t\t each row represents one wall, their coordinates are divided by one space")
        print("\t\t <Wall_Column> <Wall_Row>. E.g.: 4 1")
        exit(0)

    global path_file
    path_file=open("path.txt","w")

    global stacks_file
    stacks_file=open("stacks.txt","w")

    global cnt
    cnt=1

    global End
    End = point(int(argv[3]),int(argv[4]))

    global Start
    Start = point(int(argv[1]),int(argv[2]))

    global Opened
    Opened = openedStack(Start)

    global Closed
    Closed = []

    global Walls
    Walls=[]
    with open(argv[5],"r") as f:
        for line in f:
            x,y=line.split(" ",2)
            Walls.append(point(int(x),int(y)))

    path_file.write(" Adding Tiles:                             Processing Tiles:\n")
    path_file.write("P.  [  X,  Y ]:  G     H     F             [  X,  Y ]:  G     H     F\n")
    path_file.write(f"{'-'*65}\n")
    i=Opened.getTile()
    path_file.write(f"{cnt}\t{i}\n")
    cnt+=1
    while i!=False:
        printStacks()
        
        if(i.P==End):
            path_file.write(f"\nE:\t{i}\n")
            printPath(i)
            exit(0)

        processTile(i)
        Opened.remove(i)
        i=Opened.getTile()
    close(path_file)
    close(stacks_file)



if(__name__=="__main__"):
    main()
    