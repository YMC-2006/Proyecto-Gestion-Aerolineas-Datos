#ifndef GRAFO_H
#define GRAFO_H

#include "Ciudad.h"
#include "Arista.h"

using namespace std;
class Grafo {
public:
	static const int N = 50; // Tamano fijo de los arreglos.

private:
	Ciudad ciudades[N];
	
	//es el precio de viajar de un lugar a otro (la fila y la columna)
	double costos[N][N];
	//lo mismo pero con el tiempo en MINUTOSSSS
	int duraciones[N][N];
	int cantidadCiudades;
	int buscarIndiceCiudad(int id);

public:
	Grafo();
	~Grafo();
	bool agregarCiudad(Ciudad& ciudad);
	bool modificarCiudad(int id, string& nombre);
	bool eliminarCiudad(int id);
	bool existeCiudad(int id);
	bool obtenerCiudad(int id, Ciudad& resultado);
	int obtenerCantidadCiudades();

	bool agregarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool eliminarRuta(int idOrigen, int idDestino);
	bool existeRuta(int idOrigen, int idDestino);
	double obtenerCosto(int idOrigen, int idDestino);
	int obtenerDuracion(int idOrigen, int idDestino);
};

#endif
