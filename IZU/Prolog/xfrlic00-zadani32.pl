uloha32(SEZNAM1, SEZNAM2):-
    countZeroes(SEZNAM1,C1),
    countZeroes(SEZNAM2,C2),
    C1>C2.

countZeroes([H|T], Count):-
    countZeroes(T,Count_Next),
    H =:= 0 , Count is Count_Next + 1.

countZeroes([H|T], Count):-
    countZeroes(T,Count_Next),
    H =\= 0 ,Count = Count_Next.

countZeroes([], 0).