camaron(E, S) :-
	expresion(S, E, []).

camaron(E, S) :-
	frase(S, E, []).

% Lenguaje natural	
frase(S) -->
	gn(S),
	gv.

gn(S) -->
	determinante,
	nombre.

gv(S) -->
	verbo,
	gn.

% Transforma una cadena de caracteres en un lista

lire1Token([CAR|LASCII] , [] , [CAR|LASCII]) :-

    member(CAR , [32,13,9,10,58, 43, 45, 42, 47, 59 ]) . %CAR = separateur 

lire1Token([CAR|LASCII] ,[CAR| TOKEN ], LRESTE) :-

    not(member(CAR , [32,13,9,10,58, 43, 45, 42, 47, 59 ])) ,

    lire1Token(LASCII ,TOKEN , LRESTE).

lexer([] , []).

lexer([CAR|LASCII] , ListeTOKEN) :-
	member(CAR , [32,13,9,10]) , ! , 
	lexer(LASCII , ListeTOKEN). 

lexer([CAR|LASCII] , [TOKEN|ListeTOKEN]) :-
	member(CAR , [ 43, 45, 42, 47, 59]) , ! , 
	name(TOKEN , [CAR]),
	lexer(LASCII , ListeTOKEN). 

lexer([58,61|LASCII] , [':='|ListeTOKEN]) :- ! , 

	lexer(LASCII , ListeTOKEN). 

lexer([CAR|LASCII] , [TOKEN|ListeTOKEN]) :-
	not(member(CAR , [32,13,9,10])) , ! ,
	lire1Token([CAR|LASCII] , LCarTOKEN , LRESTE),
	name(TOKEN , LCarTOKEN),
	lexer(LRESTE , ListeTOKEN). 

% Expresiones aritmeticas
expresion(Z) --> 
	termino(X), suma, expresion(Y), {Z is X + Y}.
expresion(Z) -->
	termino(X), resta, expresion(Y), {Z is X - Y}.
expresion(Z) -->
	termino(Z).

termino(Z) -->
	factor(X), multiplicacion, termino(Y), {Z is X * Y}.
termino(Z) -->
	factor(X), division, termino(Y), {Z is X / Y}.
termino(Z) -->
	factor(Z).

factor(N) -->
	suma, factor(N).
factor(N) -->
	resta, factor(M), {N is - M}.
factor(N) -->
	parentesis_abierto, expresion(N), parentesis_cerrado.
factor(N) -->
	[N], {number(N)}.

suma -->
	[+].

resta -->
	[-].

multiplicacion -->
	[*].

division -->
	[/].

parentesis_abierto -->
	"(".

parentesis_cerrado -->
	")".
	

% Diccionario
determinante -->
	[el].
nombre -->
	[panda].

verbo -->
	[comer].
