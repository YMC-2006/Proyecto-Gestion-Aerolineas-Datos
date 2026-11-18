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
#include "ArbolExpansion.h"
#include "Animacion.h"

class InterfazGrafica {
	
private:
	GestorAdministrativo gestorAdmin;
	GestorCliente gestorCliente;
	string camposCiudad[5]; // Nombre, X, Y, nombre actual y nuevo nombre.
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
	string camposPasajero[3]; // Identificacion, nombre y codigo de vuelo.
	int campoPasajeroActivo = -1;
	string mensajePasajero;
	string campoBuscarPasajero[2]; // Identificacion y vuelo.
	int campoBuscarPasajeroActivo = -1;
	string mensajeAVL;
	string pasajeroEncontrado;
	string vueloEncontrado;
	int recorridoAVL = 0;
	int paginaAVL = 0;
	int moverAVLX = 0;
	int moverAVLY = 0;
	string campoBuscarCiudad[1];
	int campoBuscarCiudadActivo = -1;
	string mensajeArbol;
	int ciudadEncontrada = -1;
	int recorridoArbol = 0;
	int paginaArbol = 0;
	int moverArbolX = 0;
	int moverArbolY = 0;
	
	
	Criterio criterioBusqueda;
	string mensajeCliente;
	ResultadoRuta resultadoRuta;
	ResultadoRuta rutaMaleta;
	bool verMaleta;
	//Maleta
	bool mismaRuta(ResultadoRuta& a, ResultadoRuta& b);
	string textoRuta(ResultadoRuta& ruta);
	void dibujarRutaEnMapa(ResultadoRuta& ruta, Color color, float grosor);
	
	
	
	// fondos
	int anchoPantalla, altoPantalla;
	Pantalla pantallaActual = PRINCIPAL;
	Texture2D fondo;
	Texture2D fondoRegistros;
	Texture2D fondoCreditos;
	

	
	// Combos para la vista del cliente
	ListaDesplegable comboOrigen;
	ListaDesplegable comboDestino;
	
	
	// animaciones
	Texture2D texAvion;
	Texture2D texMaleta;
	Animacion animAvion;
	Animacion animMaleta;
	
	int puntosDeRuta(ResultadoRuta& ruta, Vector2 puntos[]);
	void dibujarViajero(ResultadoRuta& ruta, Animacion& animacion, Texture2D textura, bool rotar, float desfaseY);
	
	
	
	void cargarAssets();
	void liberarAssets();
	
	
	void mostrarPantallaPrincipal();
	void mostrarPantallaCliente();
	void mostrarPantallaAdministrador();
	void mostrarPantallaCreditos();
	ArbolExpansion algoritmosExpansion;
	ResultadoExpansion resultadoExpansion;
	bool expansionPorCosto = true;
	int recorridoExpansion = 0;
	int paginaExpansion = 0;
	float moverExpansionX = 0;
	float moverExpansionY = 0;
	void calcularExpansion();
	void mostrarPantallaExpansion();
	void ubicarExpansion(int actual, int padre, bool visitados[], int nivel, int& columna, Vector2 posiciones[]);
	
	void mostrarPantallaAeronaves();
	void mostrarPantallaCiudades();
	void mostrarPantallaRutas();
	void mostrarPantallaVuelos();
	void mostrarPantallaTripulacion();
	void mostrarPantallaArbolCiudades();
	void mostrarPantallaPasajeros();
	void mostrarPantallaArbolPasajeros();
	void ubicarNodosPasajero(ArbolPasajeros* arbol, int indice, int nivel, int& posicion, int x[], int y[]);
	void dibujarNodosPasajero(ArbolPasajeros* arbol, int indice, int x[], int y[]);
	void ubicarNodosCiudad(ArbolCiudades* arbol, int indice, int nivel, int& posicion, int x[], int y[]);
	void dibujarNodosCiudad(ArbolCiudades* arbol, int indice, int x[], int y[]);
	
	
	// funciones de la vista del CLIENTE
	
	Rectangle mapaCliente;
	
	void cargarCombosCliente();
	void dibujarMapaCliente();
	void dibujarRutasMapa();
	void dibujarCiudadesMapa();
	Vector2 posicionEnMapa(Ciudad& ciudad);
	void dibujarBotonOpcion(Rectangle boton, const char* texto, bool seleccionado);
	void buscarRutaCliente();
	void dibujarPanelResultado();
	
	
	// ALGORITMO DE BELLMAN FORD
	static const int FILAS_DESTINOS = 10;   // cuantas filas se ven a la vez
	
	bool mostrandoDestinos;
	int origenExplorado;
	double distExplorar[Grafo::N];
	int previoExplorar[Grafo::N];
	int ordenDestinos[Grafo::N];
	int cantidadDestinos;
	int scrollearDestinos;             // para la rueda del mouse
	ResultadoRuta rutaExplorada;
	
	void limpiarResultadosCliente();
	void explorarDestinosCliente();
	void dibujarPanelDestinos();
	Rectangle filaDestino(int posicion);
	
	
	// reservar
	// RESERVAR VUELOS
	static const int MAX_TRAMOS = 8;
	
	ListaDesplegable listasTramos[MAX_TRAMOS];
	string codigosTramo[MAX_TRAMOS][50];     // codigo de cada opcion de cada lista
	int cantidadVuelosTramo[MAX_TRAMOS];
	int cantidadTramos;
	
	string camposReserva[2];                 // identificacion y nombre
	int campoReservaActivo = -1;
	string mensajeReserva;
	bool reservaExitosa;
	
	void prepararReserva();
	void mostrarPantallaReservar();
	void confirmarReserva();
	
public:
	InterfazGrafica(int ancho, int alto,const char* titulo);
	~InterfazGrafica(); // destructor :p
	void empezar();
	
};

#endif
