#include "Ciudad.h"

// Constructor vacio -> lo necesita el arreglo Ciudad ciudades[N] del Grafo.
// El id -1 significa que esa casilla todavia no tiene una ciudad.
Ciudad::Ciudad(){
	id = -1;
	nombre = "";
	x = 0;
	y = 0;
}

Ciudad::Ciudad(int id, string nombre, float x, float y){
	this->id = id;
	this->nombre = nombre;
	this->x = x;
	this->y = y;
}

int Ciudad::getId(){
	return id;
}

string Ciudad::getNombre(){
	return nombre;
}

float Ciudad::getX(){
	return x;
}

float Ciudad::getY(){
	return y;
}

void Ciudad::setNombre(string nombre){
	this->nombre = nombre;
}

void Ciudad::setPosicion(float x, float y){
	this->x = x;
	this->y = y;
}
