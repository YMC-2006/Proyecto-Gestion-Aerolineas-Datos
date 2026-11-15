#ifndef GESTORCLIENTE_H
#define GESTORCLIENTE_H
#include "Criterio.h"
#include "ResultadoRuta.h"
using namespace std;


class GestorCliente {
private:
	Grafo* grafo;     // apunta al MISMO grafo del admin
	
public:
	GestorCliente();
	void setGrafo(Grafo* grafo);
	bool buscarMejorRuta(int origen, int destino, Criterio criterio, ResultadoRuta& resultado);
	

};
#endif
