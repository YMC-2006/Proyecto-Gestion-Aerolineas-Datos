#include "GestorCliente.h"
#include "Pila.h"
#include "Grafo.h"
#include "Criterio.h"
#include "ResultadoRuta.h"


GestorCliente::GestorCliente(){
	grafo = nullptr;
	admin = nullptr;
}

void GestorCliente::setGrafo(Grafo* grafo){
	this->grafo = grafo;
}

void GestorCliente::setAdmin(GestorAdministrativo* admin){
	this->admin = admin;
}


// Algoritmo de Dijkstra busca el camino de menor costo o menor tiempo entre dos ciudades.
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



// ======================= Algoritmo de Bellman-Ford

// calcula la mejor ruta desde "origen" hacia TODAS las ciudades.
// Llena dist[] (total para llegar a cada ciudad) y previo[] (desde donde se llego).
// Devuelve false si el origen no es valido o si hay un ciclo negativo.
bool GestorCliente::bellmanFord(int origen, Criterio criterio, double dist[], int previo[]){
	if(grafo == nullptr){
		return false;
	}
	
	int cantidad = grafo->obtenerCantidadCiudades();
	if(origen < 0 || origen >= cantidad){
		return false;
	}
	// Al inicio solo se sabe llegar al origen
	for(int i = 0; i < cantidad; i++){
		dist[i] = INFINITO;
		previo[i] = -1;
	}
	
	dist[origen] = 0;
	
	// revisar vuelta tras vuelta
	for(int vuelta = 1; vuelta <= cantidad - 1; vuelta++){
		
		bool huboCambio = false;
		
		for(int i = 0; i < cantidad; i++){
			
			// si todavia no se sabe llegar a i, no se puede salir de ahi
			if(dist[i] == INFINITO){
				continue;
			}
			
			for(int j = 0; j < cantidad; j++){
				
				if(i == j){
					continue;
				}
				
				double peso = grafo->obtenerPeso(i, j, criterio);
				
				if(peso == -1){
					continue;   // no hay ruta de i a j
				}
				
				if(dist[i] + peso < dist[j]){
					dist[j] = dist[i] + peso;
					previo[j] = i;
					huboCambio = true;
				}
			}
		}
		
		// si en una vuelta no cambio nada, ya estan todas listas
		if(!huboCambio){
			break;
		}
	}
	
	// una vuelta mas si algo todavia mejora, hay un ciclo negativo
	for(int i = 0; i < cantidad; i++){
		
		if(dist[i] == INFINITO){
			continue;
		}
		
		for(int j = 0; j < cantidad; j++){
			
			if(i == j){
				continue;
			}
			
			double peso = grafo->obtenerPeso(i, j, criterio);
			
			if(peso != -1 && dist[i] + peso < dist[j]){
				return false;
			}
		}
	}
	
	return true;
}


// Con el arreglo previo[] arma el camino de origen a destino
// (sirve con el resultado de Bellman-Ford)
bool GestorCliente::armarCamino(int origen, int destino, int previo[], double dist[], ResultadoRuta& resultado){
	
	resultado.largo = 0;
	resultado.costoTotal = 0;
	resultado.duracionTotal = 0;
	resultado.existe = false;
	
	if(dist[destino] == INFINITO){
		return false;
	}
	
	// de destino hacia atras, la pila lo voltea
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
	
	// sumar costo y tiempo de cada tramo
	for(int k = 0; k < resultado.largo - 1; k++){
		int desde = resultado.camino[k];
		int hasta = resultado.camino[k + 1];
		
		resultado.costoTotal += grafo->obtenerPeso(desde, hasta, POR_COSTO);
		resultado.duracionTotal += (int)grafo->obtenerPeso(desde, hasta, POR_TIEMPO);
	}
	
	resultado.existe = true;
	return true;
}


int GestorCliente::ordenarDestinos(int origen, double dist[], int orden[]){
	int cantidad = grafo->obtenerCantidadCiudades();
	int total = 0;
	

	for(int i = 0; i < cantidad; i++){
		if(i != origen){
			orden[total] = i;
			total++;
		}
	}
	
	// insercion cada ciudad se mueve hacia atras hasta su lugar
	for(int i = 1; i < total; i++){
		int actual = orden[i];
		int j = i - 1;
		
		while(j >= 0 && dist[orden[j]] > dist[actual]){
			orden[j + 1] = orden[j];
			j--;
		}
		
		orden[j + 1] = actual;
	}
	
	return total;
}

// para reservacionnn
// Pasa un texto a minusculas (para comparar placas y codigos sin importar mayusculas)
string aMinusculas(string texto){
	for(int i = 0; i < (int)texto.length(); i++){
		if(texto[i] >= 'A' && texto[i] <= 'Z'){
			texto[i] = texto[i] - 'A' + 'a';
		}
	}
	return texto;
}

// Llena "resultado" con los vuelos que cubren el tramo idOrigen -> idDestino.
// Devuelve cuantos encontro.
int GestorCliente::buscarVuelosDeTramo(int idOrigen, int idDestino, Vuelo resultado[]){
	int encontrados = 0;
	
	if(admin == nullptr){
		return 0;
	}
		
	for(int i = 0; i < admin->obtenerCantidadVuelos(); i++){
		Vuelo vuelo;
	
		if(admin->obtenerVueloPorIndice(i, vuelo)){
			if(vuelo.idOrigen == idOrigen && vuelo.idDestino == idDestino){
				resultado[encontrados] = vuelo;
				encontrados++;
			}
		}
	}
	
	return encontrados;
}
	
	
	// Busca la aeronave que tiene esa placa
	bool GestorCliente::obtenerAeronave(string placa, Aeronave& resultado){
		if(admin == nullptr){
			return false;
		}
		
		for(int i = 0; i < admin->obtenerCantidadAeronaves(); i++){
			Aeronave aeronave;
			
			if(admin->obtenerAeronavePorIndice(i, aeronave)){
				if(aMinusculas(aeronave.placa) == aMinusculas(placa)){
					resultado = aeronave;
					return true;
				}
			}
		}
		
		return false;
	}
	
	
	// Capacidad de la aeronave menos los pasajeros ya asignados al vuelo.
	// Los pasajeros se cuentan recorriendo el AVL.
	int GestorCliente::cuposDisponibles(Vuelo& vuelo){
		Aeronave aeronave;
		
		if(!obtenerAeronave(vuelo.placaAeronave, aeronave)){
			return 0;
		}
		
		Pasajero lista[50];
		int total = admin->obtenerArbolPasajeros()->obtenerRecorrido(0, lista);
		
		int ocupados = 0;
		
		for(int i = 0; i < total; i++){
			if(aMinusculas(lista[i].codigoVuelo) == aMinusculas(vuelo.codigo)){
				ocupados++;
			}
		}
		
		int cupos = aeronave.capacidad - ocupados;
		
		if(cupos < 0){
			cupos = 0;
		}
		
		return cupos;
	}
	
	
	// Registra al pasajero en todos los vuelos del viaje.
	// Primero REVISA todos los tramos y solo si todos estan bien los REGISTRA,
	// asi el pasajero nunca queda registrado a medias.
	bool GestorCliente::reservarViaje(string identificacion, string nombre, string codigos[], int cantidad, string& mensaje){
		
		if(admin == nullptr){
			mensaje = "Error interno: no hay gestor administrativo.";
			return false;
		}
		
		if(identificacion == "" || nombre == ""){
			mensaje = "Escriba su identificacion y su nombre.";
			return false;
		}
		
		if(cantidad <= 0){
			mensaje = "No hay vuelos para reservar.";
			return false;
		}
		
		ArbolPasajeros* arbol = admin->obtenerArbolPasajeros();
		
		// el AVL guarda maximo 50 asignaciones
		if(arbol->obtenerCantidad() + cantidad > 50){
			mensaje = "Ya no hay espacio para mas reservas (maximo 50).";
			return false;
		}
		
		// si la identificacion ya existe, el nombre tiene que coincidir
		Pasajero lista[50];
		int total = arbol->obtenerRecorrido(0, lista);
		
		for(int i = 0; i < total; i++){
			if(lista[i].identificacion == identificacion && aMinusculas(lista[i].nombre) != aMinusculas(nombre)){
				mensaje = "Esa identificacion ya esta registrada como " + lista[i].nombre + ".";
				return false;
			}
		}
		
		// 1. revisar TODOS los tramos
		for(int k = 0; k < cantidad; k++){
			Vuelo vuelo;
			
			if(!admin->obtenerVueloPorCodigo(codigos[k], vuelo)){
				mensaje = "El vuelo " + codigos[k] + " ya no existe.";
				return false;
			}
			
			Pasajero existente;
			
			if(admin->buscarPasajero(identificacion, codigos[k], existente)){
				mensaje = "Ya esta asignado al vuelo " + codigos[k] + ".";
				return false;
			}
			
			if(cuposDisponibles(vuelo) <= 0){
				mensaje = "El vuelo " + codigos[k] + " esta lleno.";
				return false;
			}
		}
		
		// 2. registrar en todos
		for(int k = 0; k < cantidad; k++){
			if(!admin->registrarPasajero(identificacion, nombre, codigos[k])){
				mensaje = "No se pudo registrar en el vuelo " + codigos[k] + ".";
				return false;
			}
		}
		
		mensaje = "Viaje reservado en " + to_string(cantidad) + " vuelo(s).";
		return true;
}


