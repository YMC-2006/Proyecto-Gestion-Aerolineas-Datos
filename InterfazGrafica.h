#ifndef INTERFAZGRAFICA_H
#define INTERFAZGRAFICA_H
#include "raylib.h"
#include "Pantalla.h"
#include "ListaDesplegable.h"
#include "GestorAdministrativo.h"
#include "Grafo.h"
#include "Criterio.h"
#include "GestorCliente.h"
#include "ResultadoRuta.h"

class InterfazGrafica {
	
private:
	GestorAdministrativo gestorAdmin;
	GestorCliente gestorCliente;
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
	string camposTripulante[4]; // Identificacion, nombre, cargo y placa.
	int campoTripulanteActivo = -1;
	string mensajeTripulante;
	string campoBuscarCiudad[1];
	int campoBuscarCiudadActivo = -1;
	string mensajeArbol;
	int ciudadEncontrada = -1;
	int recorridoArbol = 0;
	int paginaArbol = 0;
	int moverArbolX = 0;
	int moverArbolY = 0;
	Grafo grafo;
	Criterio criterioBusqueda;
	string mensajeCliente;
	ResultadoRuta resultadoRuta;
	
	
	
	
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
	void mostrarPantallaTripulacion();
	void mostrarPantallaArbolCiudades();
	void ubicarNodosCiudad(ArbolCiudades* arbol, int indice, int nivel, int& posicion, int x[], int y[]);
	void dibujarNodosCiudad(ArbolCiudades* arbol, int indice, int x[], int y[]);
	
	
	// funciones de la vista del CLIENTE
	void cargarCombosCliente();
	void dibujarMapaCliente();
	void dibujarRutasMapa();
	void dibujarCiudadesMapa();
	Vector2 posicionEnMapa(Ciudad& ciudad);
	void dibujarBotonOpcion(Rectangle boton, const char* texto, bool seleccionado);
	void buscarRutaCliente();
	
	void dibujarRutaResultado();
	void dibujarPanelResultado();
	
public:
	InterfazGrafica(int ancho, int alto,const char* titulo);
	~InterfazGrafica(); // destructor :p
	void empezar();
	
};

#endif
