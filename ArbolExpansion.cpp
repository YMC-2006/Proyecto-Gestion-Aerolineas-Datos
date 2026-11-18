#include "ArbolExpansion.h"

double ArbolExpansion::pesoNoDirigido(Grafo& grafo, int a, int b, Criterio criterio){
    double ida = grafo.obtenerPeso(a, b, criterio);
    double vuelta = grafo.obtenerPeso(b, a, criterio);
    if(ida < 0) return vuelta;
    if(vuelta < 0) return ida;
    return ida < vuelta ? ida : vuelta;
}

void ArbolExpansion::agregar(ResultadoExpansion& resultado, int a, int b, double peso){
    ConexionExpansion conexion = {a, b, peso};
    resultado.conexiones[resultado.cantidadConexiones++] = conexion;
    resultado.total += peso;
}

ResultadoExpansion ArbolExpansion::prim(Grafo& grafo){
    ResultadoExpansion resultado;
    resultado.cantidadCiudades = grafo.obtenerCantidadCiudades();
    resultado.criterio = POR_COSTO;
    bool visitado[Grafo::N] = {};
    double mejor[Grafo::N];
    int previo[Grafo::N];
    for(int i = 0; i < resultado.cantidadCiudades; i++){
        mejor[i] = -1;
        previo[i] = -1;
    }
    for(int paso = 0; paso < resultado.cantidadCiudades; paso++){
        int actual = -1;
        for(int i = 0; i < resultado.cantidadCiudades; i++){
            if(!visitado[i] && mejor[i] >= 0 && (actual == -1 || mejor[i] < mejor[actual])) actual = i;
        }
        if(actual == -1){
            // Ningun vertice pendiente es alcanzable: iniciar otra componente.
            for(int i = 0; i < resultado.cantidadCiudades; i++){
                if(!visitado[i]){ actual = i; break; }
            }
            resultado.componentes++;
        }
        visitado[actual] = true;
        if(previo[actual] != -1) agregar(resultado, previo[actual], actual, mejor[actual]);
        for(int vecino = 0; vecino < resultado.cantidadCiudades; vecino++){
            if(visitado[vecino]) continue;
            double peso = pesoNoDirigido(grafo, actual, vecino, POR_COSTO);
            if(peso >= 0 && (mejor[vecino] < 0 || peso < mejor[vecino])){
                mejor[vecino] = peso;
                previo[vecino] = actual;
            }
        }
    }
    return resultado;
}

// MergeSort propio; no se utiliza std::sort.
void ArbolExpansion::ordenar(ConexionExpansion aristas[], ConexionExpansion auxiliar[], int inicio, int fin){
    if(fin - inicio <= 1) return;
    int mitad = (inicio + fin) / 2;
    ordenar(aristas, auxiliar, inicio, mitad);
    ordenar(aristas, auxiliar, mitad, fin);
    int i = inicio, j = mitad, k = inicio;
    while(i < mitad && j < fin){
        if(aristas[i].peso <= aristas[j].peso) auxiliar[k++] = aristas[i++];
        else auxiliar[k++] = aristas[j++];
    }
    while(i < mitad) auxiliar[k++] = aristas[i++];
    while(j < fin) auxiliar[k++] = aristas[j++];
    for(int p = inicio; p < fin; p++) aristas[p] = auxiliar[p];
}

int ArbolExpansion::representante(int padres[], int nodo){
    if(padres[nodo] != nodo) padres[nodo] = representante(padres, padres[nodo]);
    return padres[nodo];
}

ResultadoExpansion ArbolExpansion::kruskal(Grafo& grafo){
    ResultadoExpansion resultado;
    resultado.cantidadCiudades = grafo.obtenerCantidadCiudades();
    resultado.componentes = resultado.cantidadCiudades;
    resultado.criterio = POR_TIEMPO;
    ConexionExpansion aristas[MAX_ARISTAS], auxiliar[MAX_ARISTAS];
    int cantidad = 0;
    for(int i = 0; i < resultado.cantidadCiudades; i++){
        for(int j = i + 1; j < resultado.cantidadCiudades; j++){
            double peso = pesoNoDirigido(grafo, i, j, POR_TIEMPO);
            if(peso >= 0){ ConexionExpansion conexion = {i, j, peso}; aristas[cantidad++] = conexion; }
        }
    }
    ordenar(aristas, auxiliar, 0, cantidad);
    int padres[Grafo::N], rangos[Grafo::N] = {};
    for(int i = 0; i < resultado.cantidadCiudades; i++) padres[i] = i;
    for(int i = 0; i < cantidad; i++){
        int a = representante(padres, aristas[i].origen);
        int b = representante(padres, aristas[i].destino);
        if(a == b) continue;
        agregar(resultado, aristas[i].origen, aristas[i].destino, aristas[i].peso);
        // Union-Find propio con union por rango y compresion de caminos.
        if(rangos[a] < rangos[b]) padres[a] = b;
        else{
            padres[b] = a;
            if(rangos[a] == rangos[b]) rangos[a]++;
        }
        resultado.componentes--;
    }
    return resultado;
}

void ArbolExpansion::recorrer(ResultadoExpansion& resultado, int actual, bool visitados[], int tipo, int orden[], int& cantidad){
    visitados[actual] = true;
    if(tipo == 0) orden[cantidad++] = actual;
    // Vecinos por indice para que el recorrido sea reproducible.
    for(int vecino = 0; vecino < resultado.cantidadCiudades; vecino++){
        if(visitados[vecino]) continue;
        for(int k = 0; k < resultado.cantidadConexiones; k++){
            ConexionExpansion& c = resultado.conexiones[k];
            if((c.origen == actual && c.destino == vecino) || (c.destino == actual && c.origen == vecino)){
                recorrer(resultado, vecino, visitados, tipo, orden, cantidad);
                break;
            }
        }
    }
    if(tipo == 1) orden[cantidad++] = actual;
}

int ArbolExpansion::obtenerRecorrido(ResultadoExpansion& resultado, int tipo, int orden[]){
    if(tipo < 0 || tipo > 2) return 0;
    bool visitados[Grafo::N] = {};
    int cantidad = 0;
    for(int raiz = 0; raiz < resultado.cantidadCiudades; raiz++){
        if(visitados[raiz]) continue;
        if(tipo != 2){ recorrer(resultado, raiz, visitados, tipo, orden, cantidad); continue; }
        // Cola propia acotada por el maximo de ciudades.
        int cola[Grafo::N], inicio = 0, fin = 0;
        cola[fin++] = raiz;
        visitados[raiz] = true;
        while(inicio < fin){
            int actual = cola[inicio++];
            orden[cantidad++] = actual;
            for(int vecino = 0; vecino < resultado.cantidadCiudades; vecino++){
                if(visitados[vecino]) continue;
                for(int k = 0; k < resultado.cantidadConexiones; k++){
                    ConexionExpansion& c = resultado.conexiones[k];
                    if((c.origen == actual && c.destino == vecino) || (c.destino == actual && c.origen == vecino)){
                        visitados[vecino] = true;
                        cola[fin++] = vecino;
                        break;
                    }
                }
            }
        }
    }
    return cantidad;
}
