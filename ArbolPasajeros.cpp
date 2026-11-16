#include "ArbolPasajeros.h"

ArbolPasajeros::ArbolPasajeros(){
	cantidad = 0;
	raiz = -1;
}

// Clave compuesta: identificacion y luego codigo de vuelo.
int ArbolPasajeros::comparar(Pasajero a, Pasajero b){
	if(a.identificacion < b.identificacion) return -1;
	if(a.identificacion > b.identificacion) return 1;
	if(a.codigoVuelo < b.codigoVuelo) return -1;
	if(a.codigoVuelo > b.codigoVuelo) return 1;
	return 0;
}

int ArbolPasajeros::altura(int indice){
	if(indice == -1) return 0;
	return nodos[indice].altura;
}

void ArbolPasajeros::actualizarAltura(int indice){
	int izquierda = altura(nodos[indice].izquierdo);
	int derecha = altura(nodos[indice].derecho);
	if(izquierda > derecha) nodos[indice].altura = izquierda + 1;
	else nodos[indice].altura = derecha + 1;
}

int ArbolPasajeros::balance(int indice){
	return altura(nodos[indice].izquierdo) - altura(nodos[indice].derecho);
}

int ArbolPasajeros::rotarDerecha(int indice){
	int hijo = nodos[indice].izquierdo;
	nodos[indice].izquierdo = nodos[hijo].derecho;
	nodos[hijo].derecho = indice;
	actualizarAltura(indice);
	actualizarAltura(hijo);
	return hijo;
}

int ArbolPasajeros::rotarIzquierda(int indice){
	int hijo = nodos[indice].derecho;
	nodos[indice].derecho = nodos[hijo].izquierdo;
	nodos[hijo].izquierdo = indice;
	actualizarAltura(indice);
	actualizarAltura(hijo);
	return hijo;
}

int ArbolPasajeros::insertarNodo(int indice, int nuevo){
	if(indice == -1) return nuevo;
	if(comparar(nodos[nuevo].pasajero, nodos[indice].pasajero) < 0){
		nodos[indice].izquierdo = insertarNodo(nodos[indice].izquierdo, nuevo);
	}else{
		nodos[indice].derecho = insertarNodo(nodos[indice].derecho, nuevo);
	}
	actualizarAltura(indice);
	if(balance(indice) > 1){
		if(balance(nodos[indice].izquierdo) < 0){
			nodos[indice].izquierdo = rotarIzquierda(nodos[indice].izquierdo);
		}
		return rotarDerecha(indice);
	}
	if(balance(indice) < -1){
		if(balance(nodos[indice].derecho) > 0){
			nodos[indice].derecho = rotarDerecha(nodos[indice].derecho);
		}
		return rotarIzquierda(indice);
	}
	return indice;
}

bool ArbolPasajeros::insertar(Pasajero pasajero){
	Pasajero existente;
	if(cantidad >= 50 || pasajero.identificacion.empty() || pasajero.nombre.empty() || pasajero.codigoVuelo.empty()) return false;
	if(buscar(pasajero.identificacion, pasajero.codigoVuelo, existente)) return false;
	nodos[cantidad].pasajero = pasajero;
	nodos[cantidad].izquierdo = -1;
	nodos[cantidad].derecho = -1;
	nodos[cantidad].altura = 1;
	raiz = insertarNodo(raiz, cantidad);
	cantidad++;
	return true;
}

bool ArbolPasajeros::buscar(string identificacion, string codigoVuelo, Pasajero& resultado){
	Pasajero clave;
	clave.identificacion = identificacion;
	clave.codigoVuelo = codigoVuelo;
	int actual = raiz;
	while(actual != -1){
		int orden = comparar(clave, nodos[actual].pasajero);
		if(orden == 0){ resultado = nodos[actual].pasajero; return true; }
		if(orden < 0) actual = nodos[actual].izquierdo;
		else actual = nodos[actual].derecho;
	}
	return false;
}

int ArbolPasajeros::obtenerCantidad(){ return cantidad; }
int ArbolPasajeros::obtenerRaiz(){ return raiz; }

int ArbolPasajeros::contarPorVuelo(string codigoVuelo){
	int total = 0;
	for(int i = 0; i < cantidad; i++){
		if(nodos[i].pasajero.codigoVuelo == codigoVuelo) total++;
	}
	return total;
}

bool ArbolPasajeros::obtenerNodo(int indice, NodoPasajero& resultado){
	if(indice < 0 || indice >= cantidad) return false;
	resultado = nodos[indice];
	return true;
}

void ArbolPasajeros::recorrer(int indice, int tipo, Pasajero resultado[], int& total){
	if(indice == -1) return;
	if(tipo == 1) resultado[total++] = nodos[indice].pasajero;
	recorrer(nodos[indice].izquierdo, tipo, resultado, total);
	if(tipo == 0) resultado[total++] = nodos[indice].pasajero;
	recorrer(nodos[indice].derecho, tipo, resultado, total);
	if(tipo == 2) resultado[total++] = nodos[indice].pasajero;
}

int ArbolPasajeros::obtenerRecorrido(int tipo, Pasajero resultado[]){
	int total = 0;
	if(tipo >= 0 && tipo <= 2) recorrer(raiz, tipo, resultado, total);
	return total;
}
