camaron(E, S) :-
	lexico(E, L),
	sintactico(S, L, []).

sintactico(S) -->
	expresion(S).

% Transforma una cadena de caracteres en un lista

token([[N] | CS], [N], CS) :-
	number(N).

token([[+] | CS], [+], CS).

token([[-] | CS], [-], CS).

token([[/] | CS], [/], CS).

token([[*] | CS], [*], CS).

haz_token([], []).

haz_token(C, [[ ] | TS]) :-
	!,
	haz_token(C, TS).

haz_token(C, [T | TS]) :-
	token(C, T, CS),
	haz_token(CS, TS).

lexico(E, L) :-
	haz_token(E, L).

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
	'('.

parentesis_cerrado -->
	')'.

