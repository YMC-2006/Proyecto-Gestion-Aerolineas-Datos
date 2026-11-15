#include "Pila.h"

Pila::Pila(){
	tope = nullptr;
}

// libera todos los nodos que queden
Pila::~Pila(){
	while(!vacia()){
		pop();
	}
}
	
void Pila::push(int dato){
	Nodo* nuevo = new Nodo;
	nuevo->dato = dato;
	nuevo->siguiente = tope;
	
	tope = nuevo;
}
	
// saca el de arriba y lo devuelve si esta vacia devuelve -1
int Pila::pop(){
	if(vacia()){
		return -1;
	}
	
	Nodo* aux = tope;
	int dato = aux->dato;
	
	tope = tope->siguiente;
	delete aux;

	return dato;
}
	
bool Pila::vacia(){
	if(tope == nullptr){
		return true;
	}
	return false;
	
}
