/* Created by Anjuta version 1.2.2 */
/*	This file will not be overwritten */

#include <stdio.h>
#include <string.h>

char buffer[128];

int ciclo()
{
	printf("ciclo con datos \"%s\".\n", buffer);
	return 0;
}

int set_datos(const void * datos) {
	printf("set_datos recibe: %s.\n", (char *) datos);
	strcpy(buffer, datos);
	return 0;

}

void * get_datos() {
	printf("get_datos devuelve: %s.\n", buffer);
	return buffer;
}

int iniciar() {
	printf("iniciar.\n");
	return 0;

}

int propiedades() {
	printf("propiedades.\n");
	return 0;

}

int cerrar(){
	printf("cerrar.\n");
	return 0;

}
