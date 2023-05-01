from sys import argv
from pkg_resources import resource_string
from z3 import *
from re import search, split


def parse_argument(argument):
    constr=[]
    re_result1=search(r"(zamestnanec\[)(\d(\|\d)+)(\]\[.*\]\[.*\]=\d)",argument)
    if re_result1:
        for i in re_result1.group(2).split("|"):
            constr.extend(parse_argument(re_result1.group(1)+i+re_result1.group(4)))
        return constr

    re_result2=search(r"(zamestnanec\[\d\]\[)(\d(\|\d)+)(\]\[.*\]=\d)",argument)
    if re_result2:
        for i in re_result2.group(2).split("|"):
            constr.extend(parse_argument(re_result2.group(1)+i+re_result2.group(4)))
        return constr
    
    re_result3=search(r"(zamestnanec\[\d\]\[\d\]\[)(\d(\|\d)+)(\]=\d)",argument)
    if re_result3:
        for i in re_result3.group(2).split("|"):
            constr.extend(parse_argument(re_result3.group(1)+i+re_result3.group(4)))
        return constr

    re_result4=search(r"(zamestnanec\[)(\d)(\]\[)(\d)(\]\[)(\d)(\]=)(\d)",argument)
    if re_result4:
        if re_result4.group(8)=="1":
            return [Zamestnanec[int(re_result4.group(2))][int(re_result4.group(4))*3+int(re_result4.group(6))]]
        if re_result4.group(8)=="0":
            return [Not(Zamestnanec[int(re_result4.group(2))][int(re_result4.group(4))*3+int(re_result4.group(6))])]
        raise BaseException(f"Zly argument (...=={re_result4.group(8)})")

    raise BaseException(f"Zly argument {argument}")

zamestnanec_count=9


Zamestnanec = [[Bool(f"Zamestnanec_{z}_Smena_{s}")for s in range(21)]for z in range(zamestnanec_count) ]

Zamestnanec_sum = [Sum([If(Zamestnanec[z][s],1,0) for s in range(21)]) for z in range(zamestnanec_count) ]

smena_c=[Sum([If(Zamestnanec[z][s],1,0) for z in range(zamestnanec_count)])==2 for s in range(21)]

denne_c=[Sum([If(Zamestnanec[z][d*3+s],1,0) for s in range(3)])<=1  for z in range(zamestnanec_count) for d in range(7)]

argument_c=[]
for argument in argv[1:]:
    re_result1=search(r"(zamestnanec\[)\*(\]\[.\]\[.\]=\d)",argument)
    if re_result1:
        argument=re_result1.group(1)+"|".join([z.__str__() for z in range(zamestnanec_count)])+re_result1.group(2)

    re_result2=search(r"(zamestnanec\[.*\]\[)\*(\]\[.\]=\d)",argument)
    if re_result2:
        argument=re_result2.group(1)+"|".join([d.__str__() for d in range(7)])+re_result2.group(2)
    
    re_result3=search(r"(zamestnanec\[.*\]\[.*\]\[)\*(\]=\d)",argument)
    if re_result3:
        argument=re_result3.group(1)+"|".join([s.__str__() for s in range(3)])+re_result3.group(2)
    argument_c.extend(parse_argument(argument))

s=Solver()
s.add(smena_c+denne_c+argument_c)
ret_check=s.check()
print(ret_check)

if ret_check==sat:
    m=s.model()
    mat=[" ".join(
            ["".join(
                ["0" if "True"==str(m.evaluate(Zamestnanec[z][d*3+s])) else "-" 
                for s in range(3) ]) 
            for d in range(7) ]) 
        for z in range(zamestnanec_count)]
    print_matrix(mat)