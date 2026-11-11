#ifndef INTERFAZGRAFICA_H
#define INTERFAZGRAFICA_H
#include "raylib.h"
#include "Pantalla.h"


class InterfazGrafica {
	
private:
	
	int anchoPantalla, altoPantalla;
	Pantalla pantallaActual = PRINCIPAL;
	Texture2D fondo;
	
	
	Texture2D btnCliente; 
	Texture2D btnAdministrador;
	
	
	Music musica;
	bool musicaActiva = false;
	
	
	void cargarAssets();
	void liberarAssets();
	
	
	void mostrarPantallaPrincipal();
	void mostrarPantallaCliente();
	void mostrarPantallaAdministrador();
	
public:
	InterfazGrafica(int ancho, int alto,const char* titulo);
	~InterfazGrafica(); // destructor :p
	void empezar();
	
};

#endif
