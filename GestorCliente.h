#ifndef GESTORCLIENTE_H
#define GESTORCLIENTE_H
#include "Criterio.h"
#include "ResultadoRuta.h"
#include "Grafo.h"
#include "Criterio.h"
const double INFINITO = 999999999.0;


class GestorCliente {
private:
	Grafo* grafo;     // apunta al MISMO grafo del admin
	
public:
	GestorCliente();
	void setGrafo(Grafo* grafo);
	bool buscarMejorRuta(int origen, int destino, Criterio criterio, ResultadoRuta& resultado);
	
	bool bellmanFord(int origen, Criterio criterio, double dist[], int previo[]);
	bool armarCamino(int origen, int destino, int previo[], double dist[], ResultadoRuta& resultado);
	
	int ordenarDestinos(int origen, double dist[], int orden[]);
};
#endif
