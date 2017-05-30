/*
 * pruebas_alumno.c
 *
 *  Created on: 29 de may. de 2017
 *      Author: algoritmos2
 */
#include "heap.h"
#include "testing.h"
#include <stdlib.h>
#include <string.h>

int intcmp (const void* a, const void* b) {
	int* inta = (int*)a;
	int* intb = (int*)b;
	return *inta - *intb;
}

void pruebas_heap_vacio(void) {
	heap_t* heap = heap_crear(intcmp);
	print_test("Heap creado con exito", heap != NULL);
	if (heap == NULL) return;
	print_test("Cantidad es cero", heap_cantidad(heap) == 0);
	print_test("Heap esta vacio", heap_esta_vacio(heap));
	print_test("Ver maximo es NULL", heap_ver_max(heap) == NULL);
	print_test("Desencolar devuelve NULL", heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}



void pruebas_heap_alumno(void) {
	pruebas_heap_vacio();
}
