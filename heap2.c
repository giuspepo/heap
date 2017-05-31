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
//De esa manera, si el heap esta vacio (cant == 0), ver_max puede aun usar heap-datos[0].

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

void upheap(heap_t* heap){
	size_t pos = heap->cant - 1;
	size_t mid = (pos - 1)/2;
	//Mid esta en caso que la division no castee correctamente.
	while(pos > 0 &&  heap->cmp(heap->datos[mid], heap->datos[pos] ) < 0){
		swap(heap->datos, mid, pos);
		pos = mid;
		mid = (pos - 1)/2;
	}
}

void downheap(heap_t* heap){
	size_t pos = 0;
	while ( 2*pos < heap->cant && heap->datos[2*pos+1] != NULL && heap->cmp( heap->datos[pos], heap->datos[2*pos+1]) < 0 ){
		swap(heap->datos, 2*pos+1, pos);
		pos = 2*pos+1;
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

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heap_t* heap =  malloc( sizeof (heap_t) * n);

	for (int i = 0; i < n; i++){
		// wtf!
		heap_encolar(heap, arreglo[i]);
	}

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

	//Reduce a una fraccion el tamano del vector si se alcanzo un minimo adecuado
	if ((heap->cant + 1)* FACT_REDIM < heap->tam) {
		size_t frac = heap->tam /FACT_REDIM;
		heap_redimensionar( heap, frac);
	}

	downheap( heap );
	return dato;
}
