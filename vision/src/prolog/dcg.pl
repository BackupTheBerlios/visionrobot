/*
  Visi�n por Computador
  DCG para procesamiento de lenguaje natural en el robot
  Archivo: dcg.pl
  Autor: Carlos Le�n
  Agradecimientos: a Juan Rodr�guez, por "quita_blancos_str", reconocimiento de n�meros, y correcciones y a�adidos en general.
  Versi�n: 1.0  
*/
:- module(dcg, [camaron/2]).

camaron(StringEntrada, Resultado) :-
	parser(StringEntrada, Resultado, _).

parser(StringEntrada, Resultado, Resto) :-
	quita_blancos_str(StringEntrada, StringSinBlancos),
	phrase(sintactico(Resultado), StringSinBlancos, Resto).

quita_blancos_str([],[]).
quita_blancos_str([32|Se],Ss) :-
	!,
	quita_blancos_str(Se, Ss).
quita_blancos_str([C|Se],[C|Ss]) :-
	quita_blancos_str(Se, Ss).


sintactico(S) -->
    expresion(S).

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
      integer(N).

integer(I) -->
       digit(D0),
       digits(D),
       { number_chars(I, [D0|D])
       }.

digits([D|T]) -->
       digit(D), !,
       digits(T).
digits([]) -->
       [].

digit(D) -->
       [D],
       { code_type(D, digit)
       }.

suma -->
    "+".

resta -->
    "-".

multiplicacion -->
    "*".

division -->
    "/".

parentesis_abierto -->
    "(".

parentesis_cerrado -->
    ")".
