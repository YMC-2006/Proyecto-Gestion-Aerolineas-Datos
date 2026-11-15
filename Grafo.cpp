#include "Grafo.h"
#include <cmath>

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
// Las rutas se guardan en las matrices de costo y duracion.


bool Grafo::existeRuta(int idOrigen, int idDestino){
	int i = buscarIndiceCiudad(idOrigen);
	int j = buscarIndiceCiudad(idDestino);
	
	// si alguna de las dos ciudades no existe, no hay ruta
	if(i == -1 || j == -1){
		return false;
	}
	
	if(costos[i][j] == -1){
		return false;
	}
	
	return true;
}

bool Grafo::modificarCiudad(int id, string& nombre){
	int indice = buscarIndiceCiudad(id);
	if(indice == -1 || nombre.empty()) return false;
	for(int i = 0; i < cantidadCiudades; i++){
		if(i != indice && ciudades[i].getNombre() == nombre) return false;
	}
	ciudades[indice].setNombre(nombre);
	return true;
}

bool Grafo::eliminarCiudad(int id){
	int indice = buscarIndiceCiudad(id);
	if(indice == -1) return false;
	// Quitar la fila y la columna de la ciudad junto con sus conexiones.
	for(int i = indice; i < cantidadCiudades - 1; i++){
		ciudades[i] = ciudades[i + 1];
		for(int j = 0; j < cantidadCiudades; j++){
			costos[i][j] = costos[i + 1][j];
			duraciones[i][j] = duraciones[i + 1][j];
		}
	}
	for(int j = indice; j < cantidadCiudades - 1; j++){
		for(int i = 0; i < cantidadCiudades; i++){
			costos[i][j] = costos[i][j + 1];
			duraciones[i][j] = duraciones[i][j + 1];
		}
	}
	cantidadCiudades--;
	ciudades[cantidadCiudades] = Ciudad();
	for(int i = 0; i < N; i++){
		costos[cantidadCiudades][i] = -1;
		costos[i][cantidadCiudades] = -1;
		duraciones[cantidadCiudades][i] = -1;
		duraciones[i][cantidadCiudades] = -1;
	}
	return true;
}

bool Grafo::agregarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos){
	int i = buscarIndiceCiudad(idOrigen);
	int j = buscarIndiceCiudad(idDestino);
	if(i == -1 || j == -1 || i == j) return false;
	if(!isfinite(costo) || costo < 0 || duracionMinutos <= 0) return false;
	if(existeRuta(idOrigen, idDestino)) return false;
	// Solo se registra la ida. El regreso puede tener otros valores.
	costos[i][j] = costo;
	duraciones[i][j] = duracionMinutos;
	return true;
}

bool Grafo::modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos){
	if(!existeRuta(idOrigen, idDestino)) return false;
	if(!isfinite(costo) || costo < 0 || duracionMinutos <= 0) return false;
	int i = buscarIndiceCiudad(idOrigen);
	int j = buscarIndiceCiudad(idDestino);
	costos[i][j] = costo;
	duraciones[i][j] = duracionMinutos;
	return true;
}

bool Grafo::eliminarRuta(int idOrigen, int idDestino){
	if(!existeRuta(idOrigen, idDestino)) return false;
	int i = buscarIndiceCiudad(idOrigen);
	int j = buscarIndiceCiudad(idDestino);
	costos[i][j] = -1;
	duraciones[i][j] = -1;
	return true;
}

double Grafo::obtenerCosto(int idOrigen, int idDestino){
	if(!existeRuta(idOrigen, idDestino)) return -1;
	return costos[buscarIndiceCiudad(idOrigen)][buscarIndiceCiudad(idDestino)];
}

int Grafo::obtenerDuracion(int idOrigen, int idDestino){
	if(!existeRuta(idOrigen, idDestino)) return -1;
	return duraciones[buscarIndiceCiudad(idOrigen)][buscarIndiceCiudad(idDestino)];
}

// Devuelve el costo o la duracion de la ruta directa entre dos indices.
// Devuelve -1 si no hay ruta.
double Grafo::obtenerPeso(int indiceOrigen, int indiceDestino, Criterio criterio){
	if(indiceOrigen < 0 || indiceOrigen >= cantidadCiudades){
		return -1;
	}
	if(indiceDestino < 0 || indiceDestino >= cantidadCiudades){
		return -1;
	}
	
	if(criterio == POR_COSTO){
		return costos[indiceOrigen][indiceDestino];
	}
	else{
		return duraciones[indiceOrigen][indiceDestino];
	}
}





