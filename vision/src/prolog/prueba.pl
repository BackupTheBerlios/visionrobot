%DECLARACION DEL MODULO
/**
* Este m�dulo carga a todos los dem�s y es el �nico que debe ser llamado por los
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

/**
* main(+DirTrabajo): establece el directorio indicado como directorio de trabajo
y
* genera una progresion de acordes. Hay que expandirlo con m�s argumentos
* @param +DirTrabajo: formato como 'c:/hlocal'
* */
main(DirTrabajo) :- working_directory(_, DirTrabajo).


/**
* mainArgumentos: este predicado esta pensado para ser compilado en un exe
siguiendo este
* procedimiento:
* -iniciar SWI-Prolog
* -consultar solamente este modulo
* -llamar al objetivo:
biblio_genaro_ES:haz_ejecutable('ruta_relativa/mainArgs.exe',principal:mainArgumentos)
* Una vez construido el ejecutable mainArgs.exe su directorio de trabajo al
ejecutarlo ser� el directorio
* desde donde se le llame, asi que ser�a recomendable situarlo en C:/hlocal o el
directorio que se use para
* la comunicacion de archivos que corresponda. para que luego Haskell pueda
encontrar los archivos
* Por consola esto se debe llamar sin poner "-" delante de los argumentos, por
ejemplo con ./mainArgs.exe d:/cvsrepo 8 0
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

* haz_ejecutable(+NombreArchivo,+Objetivo): salva el estado actual del programa
en un ejecutable independiente
* que tendr� como objetivo el atomo especificado
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

