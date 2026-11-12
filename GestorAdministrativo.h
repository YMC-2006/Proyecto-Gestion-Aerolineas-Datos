#ifndef GESTORADMINISTRATIVO_H
#define GESTORADMINISTRATIVO_H

#include "Grafo.h"
using namespace std;

class GestorAdministrativo {
private:
	Grafo grafo;

public:
	GestorAdministrativo();
	~GestorAdministrativo();

	bool registrarCiudad(int id, string nombre);
	bool modificarCiudad(int id, string nombre);
	bool eliminarCiudad(int id);

	bool registrarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool modificarRuta(int idOrigen, int idDestino, double costo, int duracionMinutos);
	bool eliminarRuta(int idOrigen, int idDestino);
};

#endif
