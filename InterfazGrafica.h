#ifndef INTERFAZGRAFICA_H
#define INTERFAZGRAFICA_H
#include "raylib.h"
#include "Pantalla.h"
#include "ListaDesplegable.h"
#include "GestorAdministrativo.h"
#include "Grafo.h"
#include "Criterio.h"


class InterfazGrafica {
	
private:
	GestorAdministrativo gestorAdmin;
	GestorAdministrativo gestorCliente;
	string camposCiudad[3]; // Nombre, X, Y.
	string camposAeronave[4]; // Placa, capacidad, marca, modelo.
	int campoCiudadActivo = -1;
	int campoAeronaveActivo = -1;
	string mensajeCiudad;
	string mensajeAeronave;
	string camposRuta[4]; // Nombre origen, nombre destino, costo, minutos.
	int campoRutaActivo = -1;
	string mensajeRuta;
	string camposVuelo[4]; // Codigo, placa, nombre origen y nombre destino.
	int campoVueloActivo = -1;
	string mensajeVuelo;
	Grafo grafo;
	Criterio criterioBusqueda;
	string mensajeCliente;
	
	
	
	
	
	// fondos
	int anchoPantalla, altoPantalla;
	Pantalla pantallaActual = PRINCIPAL;
	Texture2D fondo;
	Texture2D fondoRegistros;
	Rectangle mapaCliente;
	
	
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
	void mostrarPantallaVuelos();
	
	
	// funciones de la vista del CLIENTE
	void cargarCombosCliente();
	void dibujarMapaCliente();
	void dibujarRutasMapa();
	void dibujarCiudadesMapa();
	Vector2 posicionEnMapa(Ciudad& ciudad);
	void dibujarBotonOpcion(Rectangle boton, const char* texto, bool seleccionado);
	void buscarRutaCliente();
	
public:
	InterfazGrafica(int ancho, int alto,const char* titulo);
	~InterfazGrafica(); // destructor :p
	void empezar();
	
};

#endif
