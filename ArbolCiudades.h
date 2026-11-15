#ifndef ARBOLCIUDADES_H
#define ARBOLCIUDADES_H
#include "Ciudad.h"

struct NodoCiudad {
	Ciudad ciudad;
	int izquierdo;
	int derecho;
};

// ABB propio, ordenado por nombre. -1 indica que no hay hijo.
class ArbolCiudades {
private:
	NodoCiudad nodos[50];
	int cantidad;
	int raiz;
	string clave(string nombre);
	void recorrer(int indice, int tipo, Ciudad resultado[], int& total);
public:
	ArbolCiudades();
	void limpiar();
	bool insertar(Ciudad ciudad);
	bool buscar(string nombre, Ciudad& resultado);
	int obtenerRaiz();
	bool obtenerNodo(int indice, NodoCiudad& resultado);
	// 0: inorden, 1: preorden, 2: postorden.
	int obtenerRecorrido(int tipo, Ciudad resultado[]);
};
#endif
