#include "GestorAdministrativo.h"

#include <cctype>
#include <cmath>

//auxiliares para manejo de texto escritos
string limpiarTexto(string texto){
	int inicio = 0;
	int fin = (int)texto.length() - 1;
	string limpio = "";
	
	// Saltar espacios del inicio y del final
	while(inicio <= fin && texto[inicio] == ' '){
		inicio++;
	}
	while(fin >= inicio && texto[fin] == ' '){
		fin--;
	}
	
	// Copiar lo que queda, incluyendo los espacios interiores.
	for(int i = inicio; i <= fin; i++){
		limpio += texto[i];
	}
	
	return limpio;
}

string minusculas(string texto){
	for(int i = 0; i < (int)texto.length(); i++){
		if(texto[i] >= 'A' && texto[i] <= 'Z'){
			texto[i] = tolower(texto[i]);
		}
	}		
	return texto;
}
	
	
//propios de la clase
	
GestorAdministrativo::GestorAdministrativo(){
	cantidadAeronaves = 0;
	siguienteIdCiudad = 1;
}

GestorAdministrativo::~GestorAdministrativo(){}

bool GestorAdministrativo::registrarCiudad(int id, string nombre){
	nombre = limpiarTexto(nombre);
	Ciudad ciudad;
	for(int i = 0; i < grafo.obtenerCantidadCiudades(); i++){
		grafo.obtenerCiudadPorIndice(i, ciudad); 
		//si la ciudad ya estaba
		if(minusculas(ciudad.getNombre()) == minusculas(nombre)){
			return false;
		}
	}
	Ciudad nueva(id, nombre, 0, 0);
	return grafo.agregarCiudad(nueva);
}

bool GestorAdministrativo::registrarCiudad(string nombre, float x, float y){
	nombre = limpiarTexto(nombre);
	
	if(nombre == ""){
		return false;
	}
	
	// Comprobar que las coordenadas sean numeros finitos.
	if(!isfinite(x) || !isfinite(y)){
		return false;
	}
	
	// Comprobar que el punto en el mapa
	if(x < 0 || x > 600 || y < 0 || y > 400){
		return false;
	}
	
	// Buscar si ya hay una ciudad con ese nombre.
	Ciudad ciudad;
	
	for(int i = 0; i < grafo.obtenerCantidadCiudades(); i++){
		grafo.obtenerCiudadPorIndice(i, ciudad);
		
		if(minusculas(ciudad.getNombre()) == minusculas(nombre)){
			return false;
		}
	}
	
	// Buscar un ID disponible.
	while(grafo.existeCiudad(siguienteIdCiudad)){
		siguienteIdCiudad++;
	}
	
	Ciudad nueva(siguienteIdCiudad, nombre, x, y);
	bool registrada = grafo.agregarCiudad(nueva);
	
	if(registrada == false){
		return false;
	}
	
	siguienteIdCiudad++;
	return true;
}

int GestorAdministrativo::obtenerCantidadCiudades(){
	return grafo.obtenerCantidadCiudades();
}

bool GestorAdministrativo::obtenerCiudadPorIndice(int indice, Ciudad& resultado){
	return grafo.obtenerCiudadPorIndice(indice, resultado);
}

bool GestorAdministrativo::registrarAeronave(string placa, int capacidad, string marca, string modelo){
	placa = limpiarTexto(placa);
	placa = minusculas(placa);
	marca = limpiarTexto(marca);
	modelo = limpiarTexto(modelo);
	
	if(cantidadAeronaves >= 50){
		return false;
	}
	
	if(placa == "" || marca == "" || modelo == ""){
		return false;
	}
	
	if(capacidad <= 0){
		return false;
	}
	
	// Comprobar que la placa no este registrada.
	for(int i = 0; i < cantidadAeronaves; i++){
		if(minusculas(aeronaves[i].placa) == placa){
			return false;
		}
	}
	
	Aeronave nueva;
	nueva.placa = placa;
	nueva.capacidad = capacidad;
	nueva.marca = marca;
	nueva.modelo = modelo;
	
	aeronaves[cantidadAeronaves] = nueva;
	cantidadAeronaves++;
	
	return true;
}

int GestorAdministrativo::obtenerCantidadAeronaves(){
	return cantidadAeronaves;
}

bool GestorAdministrativo::obtenerAeronavePorIndice(int indice, Aeronave& resultado){
	if(indice < 0 || indice >= cantidadAeronaves){
		return false;
	}
	resultado = aeronaves[indice];
	return true;
}
