camaron(E, S) :-
	frase(E, []).

camaron(E, S) :-
	expresion(S, E, []).

% Lenguaje natural	
frase -->
	gn,
	gv.

gn -->
	determinante,
	nombre.

gv -->
	verbo,
	gn.


% Expresiones aritmeticas
expresion(Z) --> 
	termino(X), [+], expresion(Y), {Z is X + Y}.
expresion(Z) -->
	termino(X), [-], expresion(Y), {Z is X - Y}.
expresion(Z) -->
	termino(X).

termino(Z) -->
	factor(X), [*], termino(Y), {Z is X * Y}.
termino(Z) -->
	factor(X), [/], termino(Y), {Z is X / Y}.
termino(Z) -->
	factor(Z).

factor(N) -->
	[+], factor(N).
factor(N) -->
	[-], factor(M), {N is - M}.
factor(N) -->
	[N], {member(N,[0,1,2,3,4,5,6,7,8,9])}.



% Diccionario
determinante -->
	[el].
nombre -->
	[panda].

verbo -->
	[comer].
