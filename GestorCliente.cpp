#include "GestorCliente.h"
#include "Pila.h"

const double INFINITO = 999999999.0;

GestorCliente::GestorCliente(){
	grafo = nullptr;
}

void GestorCliente::setGrafo(Grafo* grafo){
	this->grafo = grafo;
}


// Algoritmo de Dijkstra busca el camino de menor costo o menor tiempo entre dos ciudades.
// origen y destino son INDICES (los mismos de las listas desplegables).
bool GestorCliente::buscarMejorRuta(int origen, int destino, Criterio criterio, ResultadoRuta& resultado){
	
	resultado.largo = 0;
	resultado.costoTotal = 0;
	resultado.duracionTotal = 0;
	resultado.existe = false;
	
	if(grafo == nullptr){
		return false;
	}
	
	int cantidad = grafo->obtenerCantidadCiudades();
	
	if(origen < 0 || origen >= cantidad || destino < 0 || destino >= cantidad){
		return false;
	}
	
	double dist[Grafo::N];
	bool visitado[Grafo::N];
	int previo[Grafo::N];
	
	// Al inicio no se sabe llegar a ninguna ciudad
	for(int i = 0; i < cantidad; i++){
		dist[i] = INFINITO;
		visitado[i] = false;
		previo[i] = -1;
	}
	
	dist[origen] = 0;
	
	// En cada vuelta se fija una ciudad
	for(int paso = 0; paso < cantidad; paso++){
		
		// buscar la ciudad no visitada con menor distancia
		int actual = -1;
		double menor = INFINITO;
		
		for(int i = 0; i < cantidad; i++){
			if(!visitado[i] && dist[i] < menor){
				menor = dist[i];
				actual = i;
			}
		}
		
		// si no quedo ninguna alcanzable, se termina
		if(actual == -1){
			break;
		}
		
		visitado[actual] = true;
		
		// ya se llego al destino, su distancia es definitiva
		if(actual == destino){
			break;
		}
		
		// relajar -> revisar si pasando por "actual" se llega mas barato a sus vecinos
		for(int vecino = 0; vecino < cantidad; vecino++){
			
			if(visitado[vecino]){
				continue;
			}
			
			double peso = grafo->obtenerPeso(actual, vecino, criterio);
			
			if(peso == -1){
				continue;   // no hay ruta directa
			}
			
			double nuevaDistancia = dist[actual] + peso;
			
			if(nuevaDistancia < dist[vecino]){
				dist[vecino] = nuevaDistancia;
				previo[vecino] = actual;
			}
		}
	}
	
	// Si el destino quedo en infinito, no hay forma de llegar
	if(dist[destino] == INFINITO){
		return false;
	}
	
	// Reconstruir el camino -> de destino hacia atras, y la pila lo voltea
	Pila pila;
	int ciudad = destino;
	
	while(ciudad != -1){
		pila.push(ciudad);
		ciudad = previo[ciudad];
	}
	
	while(!pila.vacia()){
		resultado.camino[resultado.largo] = pila.pop();
		resultado.largo++;
	}
	
	// Sumar costo y tiempo de cada tramo
	for(int k = 0; k < resultado.largo - 1; k++){
		int desde = resultado.camino[k];
		int hasta = resultado.camino[k + 1];
		
		resultado.costoTotal += grafo->obtenerPeso(desde, hasta, POR_COSTO);
		resultado.duracionTotal += (int)grafo->obtenerPeso(desde, hasta, POR_TIEMPO);
	}
	
	resultado.existe = true;
	return true;
}
