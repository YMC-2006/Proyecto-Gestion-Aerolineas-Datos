#ifndef INTERFAZGRAFICA_H
#define INTERFAZGRAFICA_H
#include "raylib.h"
#include "Pantalla.h"
#include "ListaDesplegable.h"
#include "GestorAdministrativo.h"


class InterfazGrafica {
	
private:
	GestorAdministrativo gestor;
	string camposCiudad[3]; // Nombre, X, Y.
	string camposAeronave[4]; // Placa, capacidad, marca, modelo.
	int campoCiudadActivo = -1;
	int campoAeronaveActivo = -1;
	string mensajeCiudad;
	string mensajeAeronave;
	
	// fondos
	int anchoPantalla, altoPantalla;
	Pantalla pantallaActual = PRINCIPAL;
	Texture2D fondo;
	Texture2D fondoRegistros;
	
	
	
	// Botones del sistema
	Texture2D btnCliente; 
	Texture2D btnAdministrador;
	
	// Combos para la vista del cliente
	ListaDesplegable comboOrigen;
	ListaDesplegable comboDestino;
	
	
	// Texto
	
	
	Music musica;
	bool musicaActiva = false;
	
	
	void cargarAssets();
	void liberarAssets();
	
	
	void mostrarPantallaPrincipal();
	void mostrarPantallaCliente();
	void mostrarPantallaAdministrador();
	void mostrarPantallaAeronaves();
	void mostrarPantallaCiudades();
	void mostrarPantallaRutas();
	
	
	// funciones de la vista del CLIENTE
	void cargarCombosCliente();
	
public:
	InterfazGrafica(int ancho, int alto,const char* titulo);
	~InterfazGrafica(); // destructor :p
	void empezar();
	
};

#endif
