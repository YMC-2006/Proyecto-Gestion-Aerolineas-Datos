#ifndef GESTORADMINISTRATIVO_H
#define GESTORADMINISTRATIVO_H

#include "Grafo.h"
#include "Aeronave.h"
using namespace std;

class GestorAdministrativo {
private:
	Grafo grafo;
	Aeronave aeronaves[50];
	int cantidadAeronaves;
	int siguienteIdCiudad;

public:
	GestorAdministrativo();
	~GestorAdministrativo();

	bool registrarCiudad(int id, string nombre);
	bool registrarCiudad(string nombre, float x, float y);
	int obtenerCantidadCiudades();
	bool obtenerCiudadPorIndice(int indice, Ciudad& resultado);
	bool registrarAeronave(string placa, int capacidad, string marca, string modelo);
	int obtenerCantidadAeronaves();
	bool obtenerAeronavePorIndice(int indice, Aeronave& resultado);
	bool modificarCiudad(int id, string nombre);
	bool eliminarCiudad(int id);

	bool registrarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool eliminarRuta(int idOrigen, int idDestino);
};

#endif
