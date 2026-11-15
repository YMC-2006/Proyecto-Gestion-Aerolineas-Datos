#ifndef RESULTADORUTA_H
#define RESULTADORUTA_H
#include "Grafo.h"

// Lo que devuelve Dijkstra
struct ResultadoRuta {
	int camino[Grafo::N];   // indices de las ciudades, de origen a destino
	int largo;              // cuantas ciudades tiene el camino
	double costoTotal;
	int duracionTotal;
	bool existe;           
};

#endif
