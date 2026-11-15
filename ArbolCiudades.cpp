#include "ArbolCiudades.h"

ArbolCiudades::ArbolCiudades(){ limpiar(); }

void ArbolCiudades::limpiar(){
	cantidad = 0;
	raiz = -1;
}

string ArbolCiudades::clave(string nombre){
	while(!nombre.empty() && nombre[0] == ' ') nombre.erase(0, 1);
	while(!nombre.empty() && nombre[nombre.length() - 1] == ' ') nombre.erase(nombre.length() - 1, 1);
	for(int i = 0; i < (int)nombre.length(); i++){
		if(nombre[i] >= 'A' && nombre[i] <= 'Z') nombre[i] += 'a' - 'A';
	}
	return nombre;
}

bool ArbolCiudades::insertar(Ciudad ciudad){
	string nombre = clave(ciudad.getNombre());
	if(cantidad >= 50 || nombre.empty()) return false;
	int actual = raiz;
	int padre = -1;
	bool izquierda = false;
	while(actual != -1){
		string comparado = clave(nodos[actual].ciudad.getNombre());
		if(nombre == comparado) return false;
		padre = actual;
		izquierda = nombre < comparado;
		if(izquierda) actual = nodos[actual].izquierdo;
		else actual = nodos[actual].derecho;
	}
	nodos[cantidad].ciudad = ciudad;
	nodos[cantidad].izquierdo = -1;
	nodos[cantidad].derecho = -1;
	if(padre == -1) raiz = cantidad;
	else if(izquierda) nodos[padre].izquierdo = cantidad;
	else nodos[padre].derecho = cantidad;
	cantidad++;
	return true;
}

bool ArbolCiudades::buscar(string nombre, Ciudad& resultado){
	nombre = clave(nombre);
	int actual = raiz;
	while(actual != -1){
		string comparado = clave(nodos[actual].ciudad.getNombre());
		if(nombre == comparado){
			resultado = nodos[actual].ciudad;
			return true;
		}
		if(nombre < comparado) actual = nodos[actual].izquierdo;
		else actual = nodos[actual].derecho;
	}
	return false;
}

int ArbolCiudades::obtenerRaiz(){ return raiz; }

bool ArbolCiudades::obtenerNodo(int indice, NodoCiudad& resultado){
	if(indice < 0 || indice >= cantidad) return false;
	resultado = nodos[indice];
	return true;
}

void ArbolCiudades::recorrer(int indice, int tipo, Ciudad resultado[], int& total){
	if(indice == -1) return;
	if(tipo == 1) resultado[total++] = nodos[indice].ciudad;
	recorrer(nodos[indice].izquierdo, tipo, resultado, total);
	if(tipo == 0) resultado[total++] = nodos[indice].ciudad;
	recorrer(nodos[indice].derecho, tipo, resultado, total);
	if(tipo == 2) resultado[total++] = nodos[indice].ciudad;
}

int ArbolCiudades::obtenerRecorrido(int tipo, Ciudad resultado[]){
	int total = 0;
	if(tipo >= 0 && tipo <= 2) recorrer(raiz, tipo, resultado, total);
	return total;
}
