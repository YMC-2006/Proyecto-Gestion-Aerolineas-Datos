#ifndef ARBOLEXPANSION_H
#define ARBOLEXPANSION_H
#include "Grafo.h"

// Extremos expresados como indices del grafo, no como IDs de ciudad.
struct ConexionExpansion {
    int origen;
    int destino;
    double peso;
};

struct ResultadoExpansion {
    ConexionExpansion conexiones[Grafo::N];
    int cantidadConexiones = 0;
    int cantidadCiudades = 0;
    int componentes = 0;
    double total = 0;
    Criterio criterio = POR_COSTO;
};

class ArbolExpansion {
private:
    static const int MAX_ARISTAS = Grafo::N * (Grafo::N - 1) / 2;
    double pesoNoDirigido(Grafo& grafo, int a, int b, Criterio criterio);
    void agregar(ResultadoExpansion& resultado, int a, int b, double peso);
    void ordenar(ConexionExpansion aristas[], ConexionExpansion auxiliar[], int inicio, int fin);
    int representante(int padres[], int nodo);
    void recorrer(ResultadoExpansion& resultado, int actual, bool visitados[], int tipo, int orden[], int& cantidad);
public:
    // En una red desconectada se obtiene un bosque de expansion minima.
    ResultadoExpansion prim(Grafo& grafo);
    ResultadoExpansion kruskal(Grafo& grafo);
    // 0: preorden; 1: postorden; 2: niveles. Incluye todas las componentes.
    int obtenerRecorrido(ResultadoExpansion& resultado, int tipo, int orden[]);
};
#endif
