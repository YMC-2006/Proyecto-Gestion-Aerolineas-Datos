#ifndef GESTORCLIENTE_H
#define GESTORCLIENTE_H

#include "Grafo.h"
#include "Criterio.h"
#include "ResultadoRuta.h"
#include "GestorAdministrativo.h"

// valor que significa "no se puede llegar"
const double INFINITO = 999999999.0;

class GestorCliente {
private:
	Grafo* grafo;                    // el mismo grafo del admin, solo para acceder a sus propiedades
	GestorAdministrativo* admin;     // para vuelos, aeronaves y pasajeros
	
public:
	GestorCliente();
	
	void setGrafo(Grafo* grafo);
	void setAdmin(GestorAdministrativo* admin);
	
	// rutas
	bool buscarMejorRuta(int origen, int destino, Criterio criterio, ResultadoRuta& resultado);
	bool bellmanFord(int origen, Criterio criterio, double dist[], int previo[]);
	bool armarCamino(int origen, int destino, int previo[], double dist[], ResultadoRuta& resultado);
	int ordenarDestinos(int origen, double dist[], int orden[]);
	
	// reservas
	int buscarVuelosDeTramo(int idOrigen, int idDestino, Vuelo resultado[]);
	bool obtenerAeronave(string placa, Aeronave& resultado);
	int cuposDisponibles(Vuelo& vuelo);
	bool reservarViaje(string identificacion, string nombre, string codigos[], int cantidad, string& mensaje);
};

#endif
