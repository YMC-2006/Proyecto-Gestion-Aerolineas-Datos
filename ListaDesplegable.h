#ifndef LISTADESPLEGABLE_H
#define LISTADESPLEGABLE_H

#include "raylib.h"
#include <string>
using namespace std;

class ListaDesplegable {
private:
	Rectangle caja;
	string opciones[50];
	int cantidadOpciones;
	int seleccionada;     // -1 = no ha escogido nada
	bool abierta;
	
public:
	ListaDesplegable();
	ListaDesplegable(float x, float y, float ancho, float alto);
	
	void agregarOpcion(string texto);
	void limpiar();
	
	int getSeleccionada();
	bool estaAbierta();
	
	void actualizar();
	void dibujar();
};

#endif
