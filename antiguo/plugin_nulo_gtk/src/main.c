/* Created by Anjuta version 1.2.2 */
/*	This file will not be overwritten */

#include <stdio.h>
#include <string.h>

char buffer[128];

int ciclo()
{
	printf("Ciclo con datos \"%s\".\n", buffer);
	return 0;
}

int set_datos(const void * datos) {
	printf("Set_Datos: %s.\n", (char *) datos);
	strcpy(buffer, datos);
	return 0;

}

void * get_datos() {
	printf("Get_Datos.\n");
	return buffer;
}

int iniciar() {
	printf("Iniciar.\n");
	return 0;

}

int propiedades() {
	printf("Propiedades.\n");
	return 0;

}

int cerrar(){
	printf("Cerrar.\n");
	return 0;

}
