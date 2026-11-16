#ifndef GESTORADMINISTRATIVO_H
#define GESTORADMINISTRATIVO_H

#include "Grafo.h"
#include "Aeronave.h"
#include "Vuelo.h"
#include "Tripulante.h"
#include "ArbolCiudades.h"
#include "ArbolPasajeros.h"
using namespace std;

class GestorAdministrativo {
private:
	Grafo grafo;
	Aeronave aeronaves[50];
	int cantidadAeronaves;
	int siguienteIdCiudad;
	Vuelo vuelos[50];
	int cantidadVuelos;
	Tripulante tripulantes[50];
	int cantidadTripulantes;
	ArbolCiudades arbolCiudades;
	ArbolPasajeros arbolPasajeros;

public:
	GestorAdministrativo();
	~GestorAdministrativo();

	bool registrarCiudad(int id, string nombre);
	bool registrarCiudad(string nombre, float x, float y);
	int obtenerCantidadCiudades();
	bool obtenerCiudadPorIndice(int indice, Ciudad& resultado);
	bool obtenerCiudadPorNombre(string nombre, Ciudad& resultado);
	bool registrarAeronave(string placa, int capacidad, string marca, string modelo);
	int obtenerCantidadAeronaves();
	bool obtenerAeronavePorIndice(int indice, Aeronave& resultado);
	bool modificarCiudad(int id, string nombre);
	bool eliminarCiudad(int id);
	
	// funcion que ocupo en el cliente :p
	Grafo* getGrafo();

	bool registrarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool eliminarRuta(int idOrigen, int idDestino);
	bool registrarVuelo(string codigo, string placa, int idOrigen, int idDestino);
	int obtenerCantidadVuelos();
	bool obtenerVueloPorIndice(int indice, Vuelo& resultado);
	bool obtenerVueloPorCodigo(string codigo, Vuelo& resultado);
	bool registrarTripulante(string identificacion, string nombre, string cargo, string placa);
	int obtenerCantidadTripulantes();
	bool obtenerTripulantePorIndice(int indice, Tripulante& resultado);
	bool obtenerTripulantePorIdentificacion(string identificacion, Tripulante& resultado);
	ArbolCiudades* obtenerArbolCiudades();
	bool registrarPasajero(string identificacion, string nombre, string codigoVuelo);
	bool buscarPasajero(string identificacion, string codigoVuelo, Pasajero& resultado);
	ArbolPasajeros* obtenerArbolPasajeros();
};

#endif
