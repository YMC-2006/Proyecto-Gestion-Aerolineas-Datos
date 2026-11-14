#include "Grafo.h"

Grafo::Grafo(){
	cantidadCiudades = 0;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			costos[i][j] = -1;
			duraciones[i][j] = -1;
		}
	}
}

Grafo::~Grafo(){}

int Grafo::buscarIndiceCiudad(int id){
	for(int i = 0; i < cantidadCiudades; i++){
		if(ciudades[i].getId() == id) return i;
	}
	return -1;
}

bool Grafo::existeCiudad(int id){
	return buscarIndiceCiudad(id) != -1;
}

bool Grafo::agregarCiudad(Ciudad& ciudad){
	if(cantidadCiudades == N || ciudad.getId() < 0 ||
	   ciudad.getNombre().empty() || existeCiudad(ciudad.getId())){
		return false;
	}
	for(int i = 0; i < cantidadCiudades; i++){
		if(ciudades[i].getNombre() == ciudad.getNombre()){
			return false;
		}
	}
	ciudades[cantidadCiudades] = ciudad;
	cantidadCiudades++;
	return true;
}

bool Grafo::obtenerCiudad(int id, Ciudad& resultado){
	int indice = buscarIndiceCiudad(id);
	if(indice == -1){
		return false;
	}
	resultado = ciudades[indice];
	return true;
}

int Grafo::obtenerCantidadCiudades(){
	return cantidadCiudades;
}

bool Grafo::obtenerCiudadPorIndice(int indice, Ciudad& resultado){
	if(indice < 0 || indice >= cantidadCiudades){
		return false;
	}
	resultado = ciudades[indice];
	return true;
}
//faltan los metodos de modificar, eliminar y rutas
