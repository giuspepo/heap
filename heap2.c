#include "heap.h"
#include <stdlib.h>
#include <string.h>

#define TAM_INICIAL 23
#define FACT_REDIM 2

struct heap{
	void** datos; //Datos es un vector de void*, es de tipo void**
	size_t cant;
	size_t tam;
	cmp_func_t cmp;
};
//Esta es mi idea: El heap guarda un arreglo de datos que termina siempre en NULL.
//y cant guarda la posicion de ese ultimo NULL.
//De esa manera, si el heap esta vacio (cant == 0), ver_max puede aun usar heap->datos[0].

bool heap_redimensionar(heap_t* heap, size_t nuevo_tam){
	void** datos = realloc( heap->datos, nuevo_tam * sizeof(void*));
	if ( datos == NULL) return false;

	heap->datos = datos;
	heap->tam = nuevo_tam;
	return true;
}

//Hace un swap de valores dentro de un vector, dadas dos posiciones.
void swap(void** vector, size_t a, size_t b) {
	void* aux = vector[a];
	vector[a] = vector[b];
	vector[b] = aux;
}

//Devuelve el hijo de una posicion con el valor maximo, dentro de un heap.
//Pre: el doble se encuentra dentro del rango. [2*pos < heap->cant]
size_t hijo_max(heap_t* heap, size_t pos) {
	//Si los hijos estan fuera del rango, devuelve cant como error.
	size_t izq = 2*pos+1;
	size_t der = 2*pos+2;
	if (izq >= heap->cant) return heap->cant;
	if (der == heap->cant) return izq;
	int c = heap->cmp(heap->datos[izq], heap->datos[der]);
	if (c > 0) return izq;
	else return der;
}

void upheap(heap_t* heap){
	size_t pos = heap->cant - 1;
	size_t padre = (pos - 1)/2;
	//Mid esta en caso que la division no castee correctamente.
	while(pos > 0 &&  heap->cmp(heap->datos[padre], heap->datos[pos] ) < 0){
		swap(heap->datos, padre, pos);
		pos = padre;
		padre = (pos - 1)/2;
	}
}

void downheap(heap_t* heap){
	size_t pos = 0;
	//while ( 2*pos < heap->cant && heap->datos[2*pos+1] != NULL && heap->cmp( heap->datos[pos], heap->datos[2*pos+1]) < 0 ){
	while (2*pos + 1 < heap->cant) {
		size_t hijo = hijo_max(heap, pos);
		if (heap->cmp(heap->datos[pos], heap->datos[hijo]) > 0) break;
		swap(heap->datos, pos, hijo);
		pos = hijo;
	}
}

//************** PRIMITIVAS DEL HEAP ***********
heap_t *heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc( sizeof(heap_t));
	if ( !heap ) return NULL;
	heap->datos = malloc( sizeof(void*) * TAM_INICIAL);
	if ( !heap->datos ){
		free( heap );
		return NULL;
	}
	heap->datos[0] = NULL; //Inicializamos con el valor NULL
	heap->tam = TAM_INICIAL;
	heap->cant = 0;
	heap->cmp = cmp;
	return heap;
}

//Ya encontre tu error.
heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap =  malloc( sizeof (heap_t) );
	if (!heap) return NULL;

	heap->datos = malloc(sizeof(void*) * (n+1));
	if (!heap->datos) {
		free(heap);
		return NULL;
	}
	heap->tam = n+1;
	heap->cant = 0;
	for (int i = 0; i < n; i++){
		heap->datos[heap->cant] = arreglo[i];
		heap->cant++;
		upheap(heap);
	}

	heap->datos[n] = NULL;

	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if ( destruir_elemento ){
		for (int i = 0; i < heap->cant; i++){
			destruir_elemento(heap->datos[i]);
		}
	}
	free( heap->datos );
	free( heap );
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap){
	return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem){
	if ( heap->cant == heap->tam -1  ){
		if ( !heap_redimensionar ( heap, heap->tam * FACT_REDIM))
			return false;
	}

	heap->datos[heap->cant] = elem;
	heap->cant++;
	heap->datos[heap->cant] = NULL;
	upheap( heap );
	return true;
}

void *heap_ver_max(const heap_t *heap){
	return heap->datos[0];
}

void *heap_desencolar(heap_t *heap){
	if ( heap_esta_vacio( heap) ) return NULL;

	void* dato = heap->datos[0];

	heap->cant--;
	heap->datos[0] = heap->datos[heap->cant];
	heap->datos[heap->cant] = NULL;

	size_t fraccion = heap->tam /FACT_REDIM;
	//Reduce a una fraccion el tamano del vector si se alcanzo un minimo adecuado
	if (heap->cant  < fraccion) {
		heap_redimensionar( heap, fraccion);
	}

	downheap( heap );
	return dato;
}
