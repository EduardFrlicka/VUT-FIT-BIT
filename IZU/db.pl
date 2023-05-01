% Zadání č. 1:
% Napište program řešící úkol daný predikátem u1(LIN,VOUT), kde LIN je vstupní
% celočíselný seznam a VOUT je výstupní proměnná, ve které se vrací délka
% seznamu LIN bez započítání nulových čísel.

% Testovací predikáty:                                          % VOUT
u1_1:- u1([5,-20,0,16,0,23,-4],VOUT),write(VOUT).       % 5
u1_2:- u1([0,1,2],VOUT),write(VOUT).                            % 2
u1_3:- u1([0,0,0],VOUT),write(VOUT).                            % 0
u1_r:- write('Zadej LIN: '),read(LIN),u1(LIN,VOUT),write(VOUT).

u1([],0).
u1([0|T],C):-u1(T, C),!.
u1([_|T],C):-u1(T, CC), C is CC + 1,!.


% Zadání č. 2:
% Napište program řešící úkol daný predikátem u2(LIN,LOUTL,LOUTR), kde LIN je 
% vstupní seznam s nejméně dvěma prvky, výstupní proměnná LOUTL vrací levou
% polovinu seznamu LIN a výstupní proměnná LOUTR vrací pravou polovinu 
% seznamu LIN. Pokud má seznam LIN lichý počet prvků, je prostřední prvek 
% součástí seznamu LOUTR.

% Testovací predikáty:                                  % LOUTL        LOUTR
u2_1:- u2([5,a0,b,16,c,kolo,-4],LOUTL,LOUTR),
       write(LOUTL),write(' '),write(LOUTR).          % [5,a0,b]    [16,c,kolo,-4]
u2_2:- u2([5,a0,b,c,kolo,-4],LOUTL,LOUTR),
       write(LOUTL),write(' '),write(LOUTR).          % [5,a0,b]    [c, kolo, -4]
u2_3:- u2([5], LOUTL, LOUTR),
       write(LOUTL),write(' '),write(LOUTR).          % []        [5]
u2_r:- write('Zadej LIN: '),read(LIN),
       u2(LIN,LOUTL,LOUTR),write(LOUTL),write(' '),write(LOUTR).

len([], 0).
len([_|T], C):- len(T, CC), C is CC + 1.

split([], _, [], []):-!.
split(R, 0, [], R):-!.
split([H|T], C, [H|TT], R):-CC is C -1, split(T, CC, TT, R).

u2(IN, L, R):-len(IN, C), CC is C // 2, split(IN, CC, L, R).


% Zadání č. 3:
% Napište program řešící úkol daný predikátem u3(LIN,VIN,LOUT), kde LIN je 
% vstupní seznam s nejméně jedním prvkem, VIN je vstupní proměnná, jejíž 
% hodnotu je kladné celé číslo a LOUT je podseznam seznamu LIN, který začíná jeho
% prvním prvkem a má délku danou hodnotou proměnné VIN. Pokud je hodnota proměnné
% VIN větší než délka seznamu LIN, tak se v seznamu LOUT vrací seznam LIN.

% Testovací predikáty:                                               % LOUT
u3_1:- u3([5,a0,b,16,c,kolo,-4],4,LOUT),write(LOUT).        % [5,a0,b,16]
u3_2:- u3([5,a0,b],5,LOUT),write(LOUT).                % [5,a0,b]
u3_3:- u3([], 3, LOUT),write(LOUT).                    % []
u3_r:- write('Zadej LIN: '),read(LIN),
       write('Zadej VIN: '),read(VIN),
       u3(LIN,VIN,LOUT),write(LOUT).

u3(IN, C, OUT):- split(IN, C, OUT, _).


% Zadání č. 4:
% Napište program řešící úkol daný predikátem u4(LIN,VOUT), kde LIN je 
% vstupní číselný seznam s nejméně dvěma prvky a VOUT je výstupní 
% proměnná, ve které se vrací hodnota rozdílu mezi největším a nejmenším 
% číslem seznamu LIN.

% Testovací predikáty:                                             % VOUT 
u4_1:- u4([5,27,-1,0,16,-4],VOUT),write(VOUT).        % 31
u4_2:- u4([5,27,-1,0,16,-24],VOUT),write(VOUT).             % 51
u4_3:- u4([5.5,27.2],VOUT),write(VOUT).                     % 21.7
u4_r:- write('Zadej LIN: '),read(LIN),u4(LIN,VOUT),write(VOUT).

u4(L, C):- max(L, Max), min(L, Min), C is Max - Min.


% Zadání č. 5:
% Napište program řešící úkol daný predikátem u5(LIN,VOUT), kde LIN je vstupní 
% celočíselný seznam a VOUT je výstupní proměnná, ve které se vrací hodnota -1/1/0,
% pokud seznam LIN obsahuje pouze lichá čísla/pouze sudá čísla/v ostatních případech.

% Testovací predikáty:                                               % VOUT 
u5_1:- u5([5,27,-1,0, 16,-4],VOUT),write(VOUT).                    % 0
u5_2:- u5([5,27,-1,1,15,-5],VOUT),write(VOUT).                   % -1
u5_3:- u5([6,28,-2,0,16,-4],VOUT),write(VOUT).               % 1
u5_r:- write('Zadej LIN: '),read(LIN),u5(LIN,VOUT),write(VOUT).

% https://gist.github.com/CMCDragonkai/9196cde3940ca4acb377311b03a9a604
odd(X):- 1 is mod(X,2).
even(X):- 0 is mod(X,2).

u5([H],-1):-odd(H),!.
u5([H], 1):-even(H),!.

u5([H|T], -1):- odd(H), u5(T,-1),!.
u5([H|T], 1):- even(H), u5(T,1),!.
u5(_,0).


% Zadání č. 6:
% Napište program řešící úkol daný predikátem u6(LIN,VOUT), kde LIN je vstupní 
% číselný seznam a VOUT je výstupní proměnná, ve které se vrací hodnota -1/1/0, 
% pokud seznam LIN obsahuje pouze záporná čísla/pouze kladná čísla/v ostatních 
% případech (nula není ani kladné, ani záporné číslo). 

% Testovací predikáty:                                       % VOUT 
u6_1:- u6([5,27,-1,0, 16,-4],VOUT),write(VOUT).                % 0
u6_2:- u6([-5.3,-27,-1,-15.8,-5],VOUT),write(VOUT).               % -1
u6_3:- u6([6.1,28,2,1,16,4.2],VOUT),write(VOUT).               % 1
u6_r:- write('Zadej LIN: '),read(LIN),u6(LIN,VOUT),write(VOUT).

u6([H],-1):-H < 0,!.
u6([H], 1):-H > 0,!.

u6([H|T], -1):- H < 0, u6(T,-1),!.
u6([H|T], 1):- H > 0, u6(T,1),!.
u6(_,0).


% Zadání č. 7:
% Napište program řešící úkol daný predikátem u7(LIN,VOUT), kde LIN je vstupní 
% číselný seznam s nejméně jedním prvkem a VOUT je výstupní proměnná, ve které  
% se vrací hodnota rozdílu počtu kladných a záporných čísel v seznamu LIN (nula  
% není ani kladné, ani záporné číslo). 

% Testovací predikáty:                                                  % VOUT 
u7_1:-u7([5,27.5,-1.4,0,16,-4], VOUT),write(VOUT).                  % 1
u7_2:-u7([0],VOUT),write(VOUT).                                          % 0
u7_3:-u7([-1,-34,98.5,-2.78,-4],VOUT),write(VOUT).                 % -3
u7_r:- write('Zadej LIN: '),read(LIN),u7(LIN,VOUT),write(VOUT).

u7([],0).
u7([0|T],C):-u7(T,C),!.
u7([H|T],C):-u7(T,CC),( H > 0 , C is CC + 1 ; H < 0, C is CC - 1 ),!.


% Zadání č. 8:
% Napište program řešící úkol daný predikátem u8(LIN,VIN), kde LIN je vstupní 
% číselný seznam s nejméně dvěma prvky a VIN je vstupní proměnná, jejíž 
% hodnotou musí být kladné číslo. Predikát je pravdivý (má hodnotu true), pokud 
% absolutní hodnoty rozdílů mezi všemi sousedními čísly v seznamu LIN jsou menší
% než číslo v proměnné VIN, jinak je predikát nepravdivý (má hodnotu false). 

% Testovací predikáty:                                          
u8_1:- u8([5,6.2,4,2.1,-1,1],4).                         %  true
u8_2:- u8([5,-6.2,4,2,-1,1],3).                          %  false
u8_3:- u8([5.1,6,4,2,-1,1.2],2).                         %  false
u8_r:- write('Zadej LIN: '),read(LIN),
       write('Zadej VIN: '),read(VIN),
       u8(LIN,VIN).

u8([_],_):-!.
u8([A|[B|T]], MAX):- C is abs(A - B), C < MAX, u8([B|T], MAX).


% Zadání č. 9:
% Napište program řešící úkol daný predikátem u9(LIN, VOUT), kde LIN je vstupní 
% číselný seznam s nejméně dvěma prvky a VOUT je výstupní proměnná, ve které
% se vrací absolutní hodnota maximálního rozdílu mezi dvěma sousedními prvky 
% v seznamu LIN. 

% Testovací predikáty:                                          % VOUT
u9_1:- u9([5,27.5,-1,28.3,19,-4],VOUT),write(VOUT).               % 29.3
u9_2:- u9([1.2,3,5,-7.4],VOUT),write(VOUT).                    % 12.4
u9_3:- u9([1,3,5,7],VOUT),write(VOUT).                        % 2
u9_r:- write('Zadej LIN: '),read(LIN),u9(LIN,VOUT),write(VOUT).

u9([_],0).
u9([A,B|T], C):- u9([B|T], CC), N is abs(A-B), N > CC, C is N, !.
u9([_,B|T], C):- u9([B|T], C).


% Zadání č. 10:
% Napište program řešící úkol daný predikátem u10(VIN,LOUT), kde VIN je vstupní 
% proměnná obsahující přirozené číslo menší než 20 určující požadovaný počet prvků  
% výstupního seznamu LOUT, který vrací čísla Fibonacciho posloupnosti. 

% Testovací predikáty:                          % LOUT
u10_1:- u10(7,LOUT),write(LOUT).                   % [0,1,1,2,3,5,8]
u10_2:- u10(3,LOUT),write(LOUT).                % [0,1,1]
u10_3:- skip_test. 
u10_r:- write('Zadej VIN: '),read(VIN),
        u10(VIN,LOUT),write(LOUT).

u10(VIN,LOUT):- fibonachi(0,1, VIN, LOUT).

fibonachi(_,_,0,[]).
fibonachi(A,B,R,[A|T]):-C is A + B, RR is R-1, fibonachi(B, C, RR, T).


% Zadání č. 11:
% Napište program řešící úkol daný predikátem u11(LIN,VIN,SOUT,LOUT), kde
% LIN je vstupní celočíselný seznam s nejméně dvěma prvky, VIN je vstupní
% proměnná obsahující přirozené číslo/práh, SOUT je výstupní seznam obsahující
% všechna čísla seznamu LIN, která jsou menší nebo stejná jako práh, LOUT je
% výstupní seznam obsahující všechna čísla seznamu LIN, která jsou větší než práh.

% Testovací predikáty:                                  % SOUT      LOUT
u11_1:- u11([5,27,-1,28,19,-4,10],10,SOUT,LOUT),
        write(SOUT),write(' '),write(LOUT).        % [5,-1,-4,10]    [27,28,19]
u11_2:- u11([5,27,-1,28,19,-4,10],28,SOUT,LOUT),
    write(SOUT),write(' '),write(LOUT).        % LIN           []        
u11_3:- u11([5,27,-1,28,19,-4,10],-8,SOUT,LOUT),
    write(SOUT),write(' '),write(LOUT).        % []         LIN
u11_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN: '),read(VIN),
        u11(LIN,VIN,SOUT,LOUT),
    write(SOUT),write(' '),write(LOUT).

u11([],_,[],[]).
u11([H|T], P, L, [H|R]):- H > P, u11(T, P, L, R), !.
u11([H|T], P, [H|R], L):- u11(T, P, R, L).

% Zadání č. 12:
% Napište program řešící úkol daný predikátem u12(LIN), kde LIN je neprázdný 
% vstupní seznam. Predikát je pravdivý (má hodnotu true), pokud prvky seznamu
% LIN představují palindrom, jinak je predikát nepravdivý (má hodnotu false).

% Testovací predikáty:                                  
u12_1:- u12([2,1,k,r,k,1,2]).                % true
u12_2:- u12([j,e,l,e,n,o,v,i,p,i,v,o,n,e,l,e,j]).        % true
u12_3:- u12([j,e,l,e,n,o,v,i]).                % false
u12_r:- write('Zadej LIN: '),read(LIN),u12(LIN).

join([],L,L).
join([H|T], L, [H|TT]):-join(T,L,TT).

reverse([],[]).
reverse([H|T], R):- reverse(T,TT), join(TT, [H], R).

u12(L):- reverse(L,L).

% Zadání č. 13:
% Napište program řešící úkol daný predikátem u13(LIN), kde LIN je vstupní
% číselný seznam s nejméně jedním prvkem. Predikát je pravdivý (má hodnotu true),
% pokud je hodnota největšího prvku seznamu LIN větší než absolutní hodnota
% nejmenšího prvku seznamu LIN, jinak je predikát nepravdivý má hodnotu false.

% Testovací predikáty:
u13_1:- u13([22.15, 1, -23, 12, 6, -8, 2]).        % false
u13_2:- u13([22, 1.8, -23, 42.5, 6, -8, 2]).               % true
u13_3:- u13([7]).                                       % false
u13_r:- write('Zadej LIN: '),read(LIN),u13(LIN).

max([H],H):-!.
max([H|T], H):- max(T,R), R < H, !.
max([_|T], R):- max(T,R).

min([H],H):-!.
min([H|T], H):- min(T,R), R > H, !.
min([_|T], R):- min(T,R).

u13(L):- max(L,Max),min(L,Min), Max > abs(Min).

% Zadání č. 14:
% Napište program řešící úkol daný predikátem u14(LIN,VIN,VOUT), kde LIN je 
% vstupní číselný seznam s nejméně jedním prvkem, VIN je vstupní proměnná  
% obsahující číslo/práh a VOUT je výstupní proměnná vracející průměrnou 
% hodnotu čísel seznamu LIN, jejichž absolutní hodnota je větší než práh.

% Testovací predikáty:                                  % VOUT
u14_1:- u14([5,27,-1,28,19,-40],10,VOUT),write(VOUT).    % 8.5
u14_2:- u14([2.5,3.6,4.7],3,VOUT),write(VOUT).             % 4.15
u14_3:- u14([8,6,-6,-8],2,VOUT),write(VOUT).        % 0
u14_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN: '),read(VIN),
        u14(LIN,VIN,VOUT),write(VOUT).

u14_sum([],_,0,0).
u14_sum([H|T], P, C, S):-u14_sum(T, P, CC, SS), abs(H) > P, C is CC + 1, S is SS + H, !.
u14_sum([_|T], P, C, S):- u14_sum(T,P,C,S).

u14(L,P,R):-u14_sum(L,P,C,S), R is S / C. % possible zero division ?


% Zadání č. 15:
% Napište program řešící úkol daný predikátem u15(LIN), kde LIN je vstupní 
% číselný seznam s nejméně jedním prvkem. Predikát je pravdivý (má hodnotu 
% true), pokud počet kladných čísel seznamu LIN větších než 10 je větší,
% než počet záporných čísel seznamu LIN menších než -10, jinak je predikát 
% nepravdivý (má hodnotu false). 

% Testovací predikáty:
u15_1:- u15([5.5,27,-1.2,0,16,-40,8.13,-15]).        % false
u15_2:- u15([25.1,27,-1,0,16,-40.8,8,-15.6]).        % true
u15_3:- u15([9]).                                   % false
u15_r:- write('Zadej LIN: '),read(LIN),u15(LIN).

u15_A([],0).
u15_A([H|T], C):- u15_A(T,CC),( H > 10, C is CC+1; H =< 10 , C is CC),!.

u15_B([],0).
u15_B([H|T], C):- u15_B(T,CC),( H < -10, C is CC+1; H >= -10, C is CC),!.

u15(L):- u15_A(L,A), u15_B(L,B), A>B.


% Zadání č. 16:
% Napište program řešící úkol daný predikátem u16(LIN1,LIN2,LOUT), kde LIN1
% a LIN2 jsou vstupní celočíselné seznamy a LOUT je výstupní seznam. Seznam
% LOUT je dán seznamem LIN1 zašifrovaným neprázdným seznamem/klíčem LIN2 tak,   
% že od každého prvku seznamu LIN1 je odečtena hodnota příslušného prvku klíče
% LIN2. Pokud je klíč LIN2 kratší než seznam LIN1, tak se šifrování opakuje
% od začátku klíče LIN2.

% Testovací predikáty:                                  % LOUT
u16_1:- u16([13,20,7,5,11,34],[4,3,5],LOUT),
        write(LOUT).                    % [9,17,2,1,8,29]
u16_2:- u16([13,20,7,5,11,34],[9,3,15,-4],LOUT),
    write(LOUT).                    % [4,17,-8,9,2,31]
u16_3:- u16([13,20],[9,3,15,-4],LOUT),
    write(LOUT).                    % [4,17]
u16_r:- write('Zadej LIN1: '),read(LIN1),
        write('Zadej LIN2: '),read(LIN2),
        u16(LIN1,LIN2,LOUT),write(LOUT).

rol([],[]).
rol([H|T], TT):- join(T, [H], TT).

u16([],_,[]):-!.
u16([HE|TE], [HK|TK], [HD|TD]):- rol([HK|TK], NewKey), u16(TE, NewKey, TD), HD is HE-HK.


% Zadání č. 17:
% Napište program řešící úkol daný predikátem u17(LIN1,LIN2,VOUT), kde LIN1 a 
% LIN2 jsou vstupní číselné seznamy/vektory se stejným počtem prvků a VOUT je 
% proměnná, ve které se vrací hodnota skalárního součinu vektorů LIN1 a LIN2. 

% Testovací predikáty:                                              % VOUT
u17_1:- u17([5,-3,2,4,12],[-3,1,0,6,-2],VOUT),write(VOUT).    % -18
u17_2:- u17([],[],VOUT),write(VOUT).                                    % 0
u17_3:- u17([5.1],[3.3],VOUT),write(VOUT).                              % 16.83
u17_r:- write('Zadej LIN1: '),read(LIN1),
        write('Zadej LIN2: '),read(LIN2),
        u17(LIN1,LIN2,LOUT),write(LOUT).

u17([],_,0):-!.
u17(_,[],0):-!.

u17([A|TA], [B|TB], R):- u17(TA,TB, RR), R is RR + A*B.


% Zadání č. 18:
% Napište program řešící úkol daný predikátem u18(LIN,VOUT), kde LIN je vstupní
% číselný seznam obsahující celá čísla z intervalu <2,20> a VOUT je proměnná,
% ve které se vrací délka seznamu LIN be započítání prvočísel.

% Testovací predikáty:                                  % VOUT
u18_1:- u18([5,3,2,4,9,12,17],VOUT),write(VOUT).    % 3
u18_2:- u18([5,3,1,4,9,19,17],VOUT),write(VOUT).    % 3
u18_3:- u18([7,3,2,2,19,17],VOUT),write(VOUT).        % 0
u18_r:- write('Zadej LIN: '),read(LIN),
        u18(LIN,VOUT),write(VOUT).

prime(2).
prime(3).
prime(5).
prime(7).
prime(11).
prime(13).
prime(17).
prime(19).

u18([], 0).
u18([H|T], R):- prime(H), u18(T, R), !.
u18([_|T], R):- u18(T, RR), R is RR +1.


% Zadání č. 19:
% Napište program řešící úkol daný predikátem u19(LIN), kde LIN je vstupní 
% seznam, který obsahuje číselné podseznamy. Predikát bude pravdivý, pokud  
% vypíše matici jejímiž řádky budou čísla z podseznamů seznamu LIN. 

% Testovací predikáty:                                  
u19_1:- u19([[1,2,3,0],[5,6,7,4],[9,8,7,8]]).       % 1 2 3 0
                                                        % 5 6 7 4
                                                        % 9 8 7 8
 
u19_2:- u19([[12,2,3,4,5,6],[7,8,9]]).              % 12 2 3 4 5 6 
                                                        % 7 8 9 

u19_3:-skip_test.
u19_r:- write('Zadej LIN: '),read(LIN),u19(LIN).

write_line([]):-nl,!.
write_line([H|T]):- write(H),write(" "), write_line(T).

u19([]):-!.
u19([H|T]):-write_line(H), u19(T).


% Zadání č. 20:
% Napište program řešící úkol daný predikátem u20(LIN,LOUT), kde LIN je  
% vstupní celočíselný seznam a LOUT je výstupní seznam obsahující všechny  
% prvky seznamu LIN, které mají liché hodnoty.

% Testovací predikáty:                              % LOUT        
u20_1:- u20([5,-3,2,4,12,-3,1,0,6,-2],LOUT),
        write(LOUT).                    % [5,-3,-3,1]
u20_2:- u20([6, -4, 2, 4, 12, 6, -2], LOUT),
        write(LOUT).                    % []
u20_3:- u20([5, -3, -17, -3, -15], LOUT),
    write(LOUT).                    % [5,-3,-17,-3,-15]
u20_r:- write('Zadej LIN: '),read(LIN),
        u20(LIN,LOUT),write(LOUT).

u20([],[]):-!.
u20([H|T], [H|TT]):-odd(H), u20(T,TT),!.
u20([_|T], TT):-u20(T,TT).


% Zadání č. 21:
% Napište program řešící úkol daný predikátem u21(LIN,VOUT), kde LIN je  
% vstupní celočíselný seznam obsahující čísla v intervalu <-10,10> a VOUT 
% je proměnná, ve které se vrací součin všech lichých čísel seznamu LIN. 

% Testovací predikáty:                                 % LOUT        
u21_1:- u21([5,3,2,4,-9,12,17],VOUT),write(VOUT).    % -2295
u21_2:- u21([8,10,2,4],VOUT),write(VOUT).        % 0
u21_3:- u21([5,3],VOUT),write(VOUT).            % 15
u21_r:- write('Zadej LIN: '),read(LIN),
        u21(LIN,LOUT),write(LOUT).

filter_odd([], []).
filter_odd([H|T], [H|TT]):-odd(H), filter_odd(T, TT), !.
filter_odd([_|T], TT):- filter_odd(T, TT).

product([], 0).
product([H], H):-!.
product([H|T], R):- !, product(T, RR), R is RR * H.

u21(L, R):- filter_odd(L, OddL), product(OddL, R).


% Zadání č. 22:
% Napište program řešící úkol daný predikátem u22(LIN1,LIN2,LOUT), kde LIN1
% a LIN2 jsou vstupní seznamy a LOUT je výstupní seznam vytvořený sloučením
% seznamů LIN1 a LIN2 systémem zip.

% Testovací predikáty:                              % LOUT        
u22_1:- u22([a,b,c,d,e],[1,2,3],LOUT),write(LOUT).  % [a,1,b,2,c,3,d,e]
u22_2:- u22([a,b,c],[1,2,3,4,5],LOUT),write(LOUT).  % [a,1,b,2,c,3,4,5]
u22_3:- u22([a,b,c],[1,2,3],LOUT),write(LOUT).      % [a,1,b,2,c,3]
u22_r:- write('Zadej LIN1: '),read(LIN1),
    write('Zadej LIN2: '),read(LIN2),
    u22(LIN1,LIN2,LOUT),write(LOUT).

u22([], L, L):-!.
u22(L, [], L):-!.
u22([HA|TA],[HB|TB],[HA,HB|R]):-u22(TA,TB, R).


% Zadání č. 23:
% Napište program řešící úkol daný predikátem u23(LIN,VOUT), kde LIN je  
% vstupní číselný seznam s nejméně jedním prvkem a VOUT je proměnná, ve
% které se vrací součet absolutních hodnot všech prvků/čísel seznamu LIN. 

% Testovací predikáty:                                 % VOUT        
u23_1:- u23([5,-3,2,4,9,12,-17],VOUT),write(VOUT).    % 52
u23_2:- u23([1,2,3],VOUT),write(VOUT).            % 6
u23_3:- u23([-5.5,-2.3,-4.2,-12], VOUT),write(VOUT).    % 24.0
u23_r:- write('Zadej LIN: '),read(LIN),
        u23(LIN,VOUT),write(VOUT).

u23([], 0).
u23([H|T], R):- u23(T, RR), R is RR + abs(H).


% Zadání č. 24:
% Napište program resici úkol daný predikátem u24(LIN,VIN1,VIN2,VOUT), kde
% LIN je vstupný čiselný seznam, promenná VIN1 a VIN2 obsahuje čísla
% splňujúce podmienku VIN1>VIN2 a VOUT je proměnna, ve které se vraci první
% čislo seznamu LIN splňujúci podmínku VIN1>VOUT>VIN2. Pokud žiadne takové
% číslo neexistuje je predikát nepravdivý (vrací hodnotu false).  

% Testovací predikáty:                                      % VOUT        
u24_1:- u24([15,2,4,9,12,17],10,2,VOUT),write(VOUT).        % 4
u24_2:- u24([15,2,-14,9,12,17],10,2,VOUT),write(VOUT).      % 9
u24_3:- u24([-10,-20.8,-5.3,0,7],0,-10,VOUT),write(VOUT).   % -5.3
u24_r:- write('Zadej LIN: '),read(LIN),
        write('Zadej VIN1: '),read(VIN1),
        write('Zadej VIN2: '),read(VIN2),
        u24(LIN,VIN1,VIN2,LOUT),write(LOUT).

u24([H|_], Up, Down, H):- H > Down, H < Up, !.
u24([_|T], A, B, C):- u24(T, A, B, C).


% Zadani c. 25:
% Napiste program resici ukol dany predikatem u25(VIN), kde promenna VIN  
% obsahuje prirozene cislo. Predikat bude pravdivy, pokud vypise toto  
% cislo v binarni soustave. 

% Testovací predikáty:                                             
u25_1:- u25(113).                    % 1110001
u25_2:- u25(1024).                                      % 10000000000
u25_3:- u25(1).                                         % 1
u25_r:- write('Zadej VIN: '),read(VIN),u25(VIN).

u25(0):-!.
u25(C):- CC is C//2, u25(CC), Z is C mod 2, write(Z), !.


% Zadání č. 26:
% Napište program řešící úkol daný predikátem u26(VIN), kde proměnná VIN 
% obsahuje přirozené číslo. Predikát bude pravdivý, pokud vypíše číslo VIN 
% v hexadecimální soustavě. 

% Testovací predikáty:                                             
u26_1:- u26(113).                        % 71
u26_2:- u26(10).                        % A
u26_3:- u26(252).                                               % FC
u26_r:- write('Zadej VIN: '),read(VIN),u26(VIN).

hex_write(X):- X >= 0, X < 10, write(X), !.
hex_write(10):- write('A'),!.
hex_write(11):- write('B'),!.
hex_write(12):- write('C'),!.
hex_write(13):- write('D'),!.
hex_write(14):- write('E'),!.
hex_write(15):- write('F'),!.

u26_solution(0):-!.
u26_solution(C):-!, CC is C//16, u26_solution(CC), Z is C mod 16, hex_write(Z).

u26(0):-write(0),!.
u26(X):-u26_solution(X).


% Zadání č. 27:
% Napište program řešící úkol daný predikátem u27(LIN,LOUT), kde LIN je 
% vstupní seznam a LOUT je výstupní seznam obsahující seznam LIN rotovaný 
% o jedno místo doleva. 

% Testovací predikáty:                                                 % LOUT 
u27_1:- u27([a,b,c,d,e],LOUT),write(LOUT).        % [b,c,d,e,a]
u27_2:- u27([],LOUT),write(LOUT).            % [] 
u27_3:- u27([123],LOUT),write(LOUT).                  % [123]
u27_r:- write('Zadej LIN: '),read(LIN),
    u27(LIN,LOUT),write(LOUT).

u27(X, R):- rol(X, R).


% Zadání č. 28:
% Napište program øešící úkol daný predikátem u28(LIN,LOUT), kde LIN je 
% vstupní seznam a LOUT je výstupní seznam obsahující všechny prvky seznamu
% LIN rotované o jedno místo doprava. 

% Testovací predikáty:                                     % LOUT 
u28_1:- u28([a,b,c,d,e],LOUT),write(LOUT).    % [e, a, b, c, d]
u28_2:- u28([],LOUT),write(LOUT).                    % [] 
u28_3:- u28([123],LOUT),write(LOUT).              % [123]
u28_r:- write('Zadej LIN: '),read(LIN),
    u28(LIN,LOUT),write(LOUT).

pop_last([_],[]):-!.
pop_last([H|T], [H|TT]):- pop_last(T, TT).

last([H],H):-!.
last([_|T],R):-last(T,R).

u28([],[]):-!.
u28(X,[H|T]):-last(X,H), pop_last(X,T).


% Zadání č. 29:
% Napište program řešící úkol daný predikátem u29(LIN,VOUT), kde LIN je  
% vstupní číselný seznam obsahující čísla z intervalu <2,20> a VOUT je  
% proměnná, ve které se vrací počet prvočísel v seznamu LIN. 

% Testovací predikáty:                                             % LOUT 
u29_1:- u29([5,3,2,4,9,12,17],VOUT),write(VOUT).        % 4
u29_2:- u29([3,5,7,11,13,17],VOUT),write(VOUT).        % 6
u29_3:- u29([6,4,12],VOUT),write(VOUT).            % 0
u29_r:- write('Zadej LIN: '),read(LIN),
    u29(LIN,VOUT),write(VOUT).

u29([], 0).
u29([H|T], R):- prime(H), u29(T, RR), R is RR + 1, !.
u29([_|T], R):- u29(T, R).


% Zadání č. 30:
% Napište program řešící úkol daný predikátem u30(LIN,VOUT), kde LIN je  
% vstupní číselný seznam s nejméně jedním prvkem a VOUT je proměnná, ve 
% které se vrací průměrná hodnota absolutních hodnot čísel v seznamu LIN. 

% Testovací predikáty:                                         % VOUT 
u30_1:- u30([5,3,-18,2,-9,-13,17],VOUT),write(VOUT).        % 9.57...
u30_2:- u30([5,2],VOUT),write(VOUT).                % 3.5
u30_3:- u30([-18.3,-9.2,-13.1],VOUT),write(VOUT).        % 13.53...
u30_r:- write('Zadej LIN: '),read(LIN),
    u30(LIN,VOUT),write(VOUT).

u30(L, R):- u23(L, S), len(L, C), R is S / C.


% Zadání c. 31:
% Napiste program resici úkol daný predikátem u31(LIN,VOUT), kde LIN je vstupní 
% císelný seznam s nejméne jedním prvkem a VOUT je promenná, ve které se vrací 
% index prvního výskytu maximálního císla v seznamu LIN (indexování zacíná 
% jednickou). 

% Testovací predikáty:                                      % LOUT 
u31_1:- u31([5,3,-18,2,-9,-13,17,4],VOUT),write(VOUT).        % 7
u31_2:- u31([5,3.1,17,2,-9.4,-13,17,4], VOUT),write(VOUT).    % 3
u31_3:- u31([5,3.3],VOUT),write(VOUT).                % 1
u31_r:- write('Zadej LIN: '),read(LIN),
    u31(LIN,VOUT),write(VOUT).

index_of(P, [P|_], 1):- !.
index_of(P, [_|T], I):- index_of(P, T, II), I is II + 1. 

u31(L, R):- max(L, M), index_of(M, L, R).


% Zadání č. 32:
% Napište program řešící úkol daný predikátem u32(LIN1,LIN2), kde LIN1 a LIN2
% jsou vstupní číselné seznamy. Predikát je pravdivý (má hodnotu true), pokud
% počet nulových čísel seznamu LIN1 je větší než počet nulových čísel seznamu
% LIN2, jinak je predikát nepravdivý (má hodnotu false).

% Testovací predikáty:                                  
u32_1:- u32([5.1,0,-18,0,0,-13],[0,2.2,-9.47,-13]).     % true
u32_2:- u32([5.1,2,-18,0,1,-13],[0,2.2,-9.47,-13]).     % false
u32_3:- u32([5,2,-18,3,0,-13],[0,2,-9,-13,0]).          % false
u32_r:- write('Zadej LIN1: '),read(LIN1),
    write('Zadej LIN2: '),read(LIN2),
    u32(LIN1,LIN2).

zero_count([],0):-!.
zero_count([0|T], C):- zero_count(T,CC), C is CC+1,!.
zero_count([_|T], C):-zero_count(T,C), !.

u32(A,B):- zero_count(A,Ca), zero_count(B,Cb), Ca > Cb.


% Zadání č. 33:
% Napište program řešící úkol daný predikátem u33(LIN,LOUT), kde LIN je vstupní 
% celočíselný seznam a LOUT je výstupní seznam obsahující všechny prvky seznamu 
% LIN, ve kterém jsou všechna sudá čísla převedena na lichá odečtením jedničky. 

% Testovací predikáty:                                     % LOUT 
u33_1:- u33([5,-3,4,8,3,1,0,-2], LOUT),write(LOUT).    % [5,-3,3,7,3,1,-1,-3]
u33_2:- u33([10,3,1,7],LOUT),write(LOUT).        % [9,3,1,7]
u33_3:- u33([],LOUT),write(LOUT).            % []
u33_r:- write('Zadej LIN: '),read(LIN),
    u33(LIN,LOUT),write(LOUT).

u33([], []).
u33([H|T], [H|TT]):- odd(H), u33(T, TT).
u33([H|T], [HH|TT]):- even(H), HH is H - 1, u33(T, TT).


% Zadání č. 34:
% Napište program řešící úkol daný predikátem u34(LIN,LOUT), kde LIN je vstupní 
% celočíselný seznam a LOUT je výstupní seznam obsahující všechny prvky seznamu  
% LIN, ve kterém jsou všechna lichá čísla převedena na sudá vynásobením dvěma. 

% Testovací predikáty:                               % LOUT 
u34_1:- u34([5,-3,4,8,3,1,0,-2],LOUT),write(LOUT).    % [10,-6,4,8,6,2,0,-2]
u34_2:- u34([10,-3,1,7],LOUT),write(LOUT).        % [10,-6,2,14]
u34_3:- u34([],LOUT),write(LOUT).            % []
u34_r:- write('Zadej LIN: '),read(LIN),
u34(LIN,LOUT),write(LOUT).

u34([], []).
u34([H|T], [H|TT]):- even(H), u34(T, TT).
u34([H|T], [HH|TT]):- odd(H), HH is H * 2, u34(T, TT).


% Zadani c. 35:
% nikto nezverejnil toto zadanie. :reee:

u35_1:-skip_test.
u35_2:-skip_test.
u35_3:-skip_test.

% Zadani c. 36:
% Napiste program resici ukol dany prediketem u36(LIN,VOUT), kde LIN je  
% vstupny ciselny seznam s nejmene dvema prvky a VOUT je vystupny promenna, ve 
% ktere se vraci druha nejvetsi cislo seznamu LIN. Pokud je v seznamu LIN vice 
% stejnych nejvetsich cisel, vraci se v promenne VOUT toto cislo.

% Testovací predikáty:                                       % VOUT 
u36_1:- u36([4,-3,4,8,3,8,0,-20],VOUT),write(VOUT).            % 8
u36_2:- u36([4,-3,-4,-8,-2.9,-8,-20],VOUT),write(VOUT).        % -2.9
u36_3:- u36([4,-3,4,8.78,3,8,0,20],VOUT),write(VOUT).        % 8.78
u36_r:- write('Zadej LIN: '),read(LIN),
    u36(LIN,VOUT),write(VOUT).

second_max([M1|[M2]], M1, M2) :- M1>M2, !.
second_max([M1|[M2]], M2, M1) :- !.
second_max([H|T], H, M1):- second_max(T,M1,_), H > M1,!.
second_max([H|T], M1, H):- second_max(T,M1,M2), H > M2, !.
second_max([_|T], M1, M2):- second_max(T,M1,M2),!.

u36(L,C):-second_max(L,_,C).  % tato uloha sa urcite da vyriesit krajsie.


% Zadání č. 37:
% Napište program řešící úkol daný predikátem u37(LIN,VIN), kde LIN je vstupní
% celočíselný seznam a VIN je vstupní proměnná obsahující libovolné přirozené
% číslo. Predikát je pravdivý (má hodnotu true), pokud počet lichých čísel
% v seznamu LIN není větší než číslo v proměnné VIN, jinak je predikát
% nepravdivý (má hodnotu false).

% Testovací predikáty:                                  
u37_1:- u37([4,-3,7,8,3,7,0,-20],2).            % false
u37_2:- u37([4,-3,7,8,3,7,0,-20],5).            % true
u37_3:- u37([],2).                  % true
u37_r:- write('Zadej LIN: '),read(LIN),
   write('Zadej VIN: '),read(VIN),
   u37(LIN,VIN).

u37([], C):- C >= 0.
u37([H|T], C):- odd(H), CC is C - 1,!, u37(T, CC).
u37([_|T], C):- u37(T, C).


% Zadání č. 38:
% Napište program řešící úkol daný predikátem u38(LIN), kde LIN je vstupní 
% celočíselný seznam. Predikát je pravdivý (má hodnotu true), pokud 
% se v seznamu LIN opakuje nějaké liché číslo větší než nula, jinak je 
% predikát nepravdivý (má hodnotu false).

% Testovací predikáty:
u38_1:- u38([4,-3,1,8,3,-8,0]).                       % false.
u38_2:- u38([4,-3,3,8,3,8,0]).                            % true.
u38_3:- u38([]).                            % false.
u38_r:- write('Zadej LIN: '),read(LIN),u38(LIN).

contain([H|_], H):- odd(H), H > 0.
contain([_|T], X):- contain(T, X).

u38([H|T]):- contain(T, H); u38(T), !.


% Zadání č. 39:
% Napište program řešící úkol daný predikátem u39(LIN,VIN1,VIN2,VOUT),
% kde LIN je vstupní číselný seznam a VIN1 a VIN2 jsou vstupní proměnné
% obsahující přirozená čísla splňující podmínku VIN1>VIN2. Proměnná VOUT
% je výstupní proměnná, ve které se vrací maximální číslo ze seznamu LIN
% splňující podmínku VIN1>VOUT>VIN2, jinak se v proměnné VOUT vrací nula.

% Testovací predikáty:                               % VOUT
u39_1:- u39([4,-3,1,8,3,8,21],12,2,VOUT),write(VOUT).    % 8
u39_2:- u39([4.56],12,2,VOUT),write(VOUT).        % 4.56
u39_3:- u39([],12,2,VOUT),write(VOUT).            % 0
u39_r:- write('Zadej LIN: '),read(LIN),
    write('Zadej VIN1: '),read(VIN1),
    write('Zadej VIN2: '),read(VIN2),
    u39(LIN,VIN1,VIN2,VOUT),write(VOUT).

filter_range([], _, _, []).
filter_range([H|T], D, U, [H|TT]):- H < U , H > D, !, filter_range(T, D, U, TT).
filter_range([_|T], D, U, TT):- filter_range(T, D, U, TT).

u39(L, U, D, 0):- filter_range(L, D, U, F), F == [].
u39(L, U, D, M):- filter_range(L, D, U, F), max(F, M).


% Zadání č. 40:
% Napište program řešící úkol daný predikátem u40(LIN), kde LIN je vstupní 
% celočíselný seznam s nejméně dvěma čísly. Predikát je pravdivý (má hodnotu 
% true), pokud se v seznamu LIN pravidelně střídají lichá a sudá čísla, 
% jinak je nepravdivý (má hodnotu false).

% Testovací predikáty:
u40_1:- u40([4,-3,2,1,8,3,8,-1]).            % true
u40_2:- u40([4,-3,2,1,8,4,8,-1]).            % false
u40_3:- u40([-3,2]).                    % true
u40_r:- write('Zadej LIN: '),read(LIN),u40(LIN).

u40([_]).
u40([A,B|T]):- odd(A), even(B), u40([B|T]).
u40([A,B|T]):- even(A), odd(B), u40([B|T]).


% Zadání č. 41:
% Napište program řešící úkol daný predikátem u41(LIN), kde LIN je vstupní
% číselný seznam s nejméně jedním prvkem. Predikát je pravdivý (má hodnotu
% true), pokud je součet kladných čísel seznamu LIN větší než absolutní
% hodnota součtu záporných čísel seznamu LIN, jinak je predikát nepravdivý
% (má hodnotu false). Nula není ani kladné, ani záporné číslo.

% Testovací predikáty:
u41_1:- u41([5,27.5,-1,0,16.3,-40.2]).        % true  
u41_2:- u41([-10]).                    % false  
u41_3:- u41([5]).                    % true  
u41_r:- write('Zadej LIN: '),read(LIN),u41(LIN).

sum_positive_negative([], 0, 0).
sum_positive_negative([H|T], P, N):- H > 0, sum_positive_negative(T, PP, N), P is PP + H.
sum_positive_negative([H|T], P, N):- H < 0, sum_positive_negative(T, P, NN), N is NN + abs(H).
sum_positive_negative([0|T], P, N):- sum_positive_negative(T, P, N).

u41(L):- sum_positive_negative(L, P, N), P > N.


% Zadání č. 42:
% Napište program řešící úkol daný predikátem u42(VIN), kde proměnná VIN obsahuje 
% kladné celé číslo. Predikát bude pravdivý, pokud vypíše číslo VIN v osmičkové 
% soustavě. 

% Testovací predikáty:
u42_1:- u42(113).                   % 161
u42_2:- u42(1024).                                           % 2000
u42_3:- u42(1).                                                 % 1
u42_r:- write('Zadej VIN: '),read(VIN),u42(VIN).

u42(0):-!.
u42(C):- CC is C//8, u42(CC), Z is C mod 8, write(Z), !.



%  testing

skip_test:- write("Skip").

execute_test(T):- write(T), write(" res:"), nl, call(T),nl, write("True"), !.
execute_test(_):- nl, write("False").

execute_all_tests:-between(1, 42, U), between(1,3,T), 
    atomic_list_concat([u, U, '_',T], Atom),
    callable(Atom),
    execute_test(Atom).
