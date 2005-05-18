%DECLARACION DEL MODULO
/**
* Este módulo carga a todos los demás y es el único que debe ser llamado por los
otros
* lenguajes y programas que integran el proyecto
* */
:- module(principal,[main/1,mainArgumentos/0]).

%BIBLIOTECAS
%:- use_module(library(system)).
:- ensure_loaded(library(system)).
:- ensure_loaded(library(lists)).

:-consult(dcg).

%ARCHIVOS PROPIOS CONSULTADOS


main(DirTrabajo) :- working_directory(_, DirTrabajo).


/**
* mainArgumentos: este predicado esta pensado para ser compilado en un exe
*siguiendo este
* procedimiento:
* -iniciar SWI-Prolog
* -consultar solamente este modulo
* -llamar al objetivo:
* haz_ejecutable('ruta_relativa/mainArgs.exe',principal:mainArgumentos)
* Una vez construido el ejecutable mainArgs.exe su directorio de trabajo al
ejecutarlo será el directorio
* desde donde se le llame, asi que sería recomendable situarlo en C:/hlocal o el
* directorio que se use. 
* Por consola esto se debe llamar sin poner "-" delante de los argumentos, por
ejemplo con ./mainArgs.exe d:/cvsrepo (5+3)/6
* En windows hay que poner todos los archivos .dll que hay en la carpeta bin del
directorio donde
* este instalado SWI pq si no funcionan los exes
* En windows no cargar el guitracer antes de hacer el exe o no funcionara
*/
mainArgumentos :- current_prolog_flag(argv, [_,DirTrabajo,Suma1, Suma2])
	,atom_number(Suma1,X),atom_number(Suma2,Y) 
	,working_directory(_, DirTrabajo)
	,suma(X,Y) 
      ,halt(0).

/* haz_ejecutable(+NombreArchivo,+Objetivo): salva el estado actual del programa
en un ejecutable independiente
* que tendrá como objetivo el atomo especificado
* En windows hay que poner todos los archivos .dll que hay en la carpeta bin del
directorio donde
* este instalado SWI pq si no funcionan los exes
* En windows no cargar el guitracer antes de hacer el exe o no funcionara
* -http://gollem.science.uva.nl/SWI-Prolog/Manual/runtime.html <javascript:ol('http://gollem.science.uva.nl/SWI-Prolog/Manual/runtime.html');>
* -ver que pasa con las dll
*/
haz_ejecutable(NombreArchivo,Objetivo) :-
    qsave_program(NombreArchivo, [goal(Objetivo), stand_alone(true)]).

%suma(X,Y):-open('salida.txt',write,Stream), R is X + Y, write(Stream,R),close(Stream).

