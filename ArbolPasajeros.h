#ifndef ARBOLPASAJEROS_H
#define ARBOLPASAJEROS_H
#include "Pasajero.h"

struct NodoPasajero {
	Pasajero pasajero;
	int izquierdo;
	int derecho;
	int altura;
};

class ArbolPasajeros {
private:
	NodoPasajero nodos[50];
	int cantidad;
	int raiz;
	int comparar(Pasajero a, Pasajero b);
	int altura(int indice);
	void actualizarAltura(int indice);
	int balance(int indice);
	int rotarDerecha(int indice);
	int rotarIzquierda(int indice);
	int insertarNodo(int indice, int nuevo);
	void recorrer(int indice, int tipo, Pasajero resultado[], int& total);
public:
	ArbolPasajeros();
	bool insertar(Pasajero pasajero);
	bool buscar(string identificacion, string codigoVuelo, Pasajero& resultado);
	int obtenerCantidad();
	int contarPorVuelo(string codigoVuelo);
	int obtenerRaiz();
	bool obtenerNodo(int indice, NodoPasajero& resultado);
	int obtenerRecorrido(int tipo, Pasajero resultado[]);
};
#endif
