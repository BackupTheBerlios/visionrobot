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
	term(X), [+], expresion(Y), {Z is X + Y}.
expresion(Z) -->
	term(X), [-], expresion(Y), {Z is X - Y}.
expresion(Z) -->
	term(X).

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
	[(], expresion(N), [)].
factor(N) -->
	[X].



% Diccionario
determinante -->
	[el].
nombre -->
	[panda].

verbo -->
	[comer].
