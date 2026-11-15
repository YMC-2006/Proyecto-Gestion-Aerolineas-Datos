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
	cantidadVuelos = 0;
	cantidadTripulantes = 0;
	siguienteIdCiudad = 1;
}

GestorAdministrativo::~GestorAdministrativo(){}

bool GestorAdministrativo::registrarCiudad(int id, string nombre){
	nombre = limpiarTexto(nombre);
	if(nombre.empty()) return false;
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


// metodo que yuli ocupaa
Grafo* GestorAdministrativo::getGrafo(){
	return &grafo;
}

bool GestorAdministrativo::obtenerCiudadPorNombre(string nombre, Ciudad& resultado){
	nombre = minusculas(limpiarTexto(nombre));
	for(int i = 0; i < grafo.obtenerCantidadCiudades(); i++){
		Ciudad ciudad;
		grafo.obtenerCiudadPorIndice(i, ciudad);
		if(minusculas(ciudad.getNombre()) == nombre){
			resultado = ciudad;
			return true;
		}
	}
	return false;
}

bool GestorAdministrativo::modificarCiudad(int id, string nombre){
	nombre = limpiarTexto(nombre);
	Ciudad repetida;
	if(obtenerCiudadPorNombre(nombre, repetida) && repetida.getId() != id) return false;
	return grafo.modificarCiudad(id, nombre);
}

bool GestorAdministrativo::eliminarCiudad(int id){
	for(int i = 0; i < cantidadVuelos; i++){
		if(vuelos[i].idOrigen == id || vuelos[i].idDestino == id) return false;
	}
	return grafo.eliminarCiudad(id);
}

bool GestorAdministrativo::registrarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos){
	return grafo.agregarRuta(idOrigen, idDestino, costo, duracionMinutos);
}

bool GestorAdministrativo::modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos){
	return grafo.modificarRuta(idOrigen, idDestino, costo, duracionMinutos);
}

bool GestorAdministrativo::eliminarRuta(int idOrigen, int idDestino){
	for(int i = 0; i < cantidadVuelos; i++){
		if(vuelos[i].idOrigen == idOrigen && vuelos[i].idDestino == idDestino) return false;
	}
	return grafo.eliminarRuta(idOrigen, idDestino);
}

bool GestorAdministrativo::registrarVuelo(string codigo, string placa, int idOrigen, int idDestino){
	codigo = minusculas(limpiarTexto(codigo));
	placa = minusculas(limpiarTexto(placa));
	if(cantidadVuelos >= 50 || codigo.empty()) return false;
	if(!grafo.existeRuta(idOrigen, idDestino)) return false;
	for(int i = 0; i < cantidadVuelos; i++){
		if(vuelos[i].codigo == codigo) return false;
	}
	bool existeAeronave = false;
	for(int i = 0; i < cantidadAeronaves; i++){
		if(aeronaves[i].placa == placa) existeAeronave = true;
	}
	if(!existeAeronave) return false;
	Vuelo nuevo;
	nuevo.codigo = codigo;
	nuevo.placaAeronave = placa;
	nuevo.idOrigen = idOrigen;
	nuevo.idDestino = idDestino;
	vuelos[cantidadVuelos] = nuevo;
	cantidadVuelos++;
	return true;
}

int GestorAdministrativo::obtenerCantidadVuelos(){
	return cantidadVuelos;
}

bool GestorAdministrativo::registrarTripulante(string identificacion, string nombre, string cargo, string placa){
	identificacion = minusculas(limpiarTexto(identificacion));
	nombre = limpiarTexto(nombre);
	cargo = limpiarTexto(cargo);
	placa = minusculas(limpiarTexto(placa));
	if(cantidadTripulantes >= 50) return false;
	if(identificacion.empty() || nombre.empty() || cargo.empty()) return false;
	for(int i = 0; i < cantidadTripulantes; i++){
		if(tripulantes[i].identificacion == identificacion) return false;
	}
	bool existeAeronave = false;
	for(int i = 0; i < cantidadAeronaves; i++){
		if(aeronaves[i].placa == placa) existeAeronave = true;
	}
	if(!existeAeronave) return false;
	Tripulante nuevo;
	nuevo.identificacion = identificacion;
	nuevo.nombre = nombre;
	nuevo.cargo = cargo;
	nuevo.placaAeronave = placa;
	tripulantes[cantidadTripulantes] = nuevo;
	cantidadTripulantes++;
	return true;
}

int GestorAdministrativo::obtenerCantidadTripulantes(){
	return cantidadTripulantes;
}

bool GestorAdministrativo::obtenerTripulantePorIndice(int indice, Tripulante& resultado){
	if(indice < 0 || indice >= cantidadTripulantes) return false;
	resultado = tripulantes[indice];
	return true;
}

bool GestorAdministrativo::obtenerTripulantePorIdentificacion(string identificacion, Tripulante& resultado){
	identificacion = minusculas(limpiarTexto(identificacion));
	for(int i = 0; i < cantidadTripulantes; i++){
		if(tripulantes[i].identificacion == identificacion){
			resultado = tripulantes[i];
			return true;
		}
	}
	return false;
}

bool GestorAdministrativo::obtenerVueloPorIndice(int indice, Vuelo& resultado){
	if(indice < 0 || indice >= cantidadVuelos) return false;
	resultado = vuelos[indice];
	return true;
}

bool GestorAdministrativo::obtenerVueloPorCodigo(string codigo, Vuelo& resultado){
	codigo = minusculas(limpiarTexto(codigo));
	for(int i = 0; i < cantidadVuelos; i++){
		if(vuelos[i].codigo == codigo){
			resultado = vuelos[i];
			return true;
		}
	}
	return false;
}
