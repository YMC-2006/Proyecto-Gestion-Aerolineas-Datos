#include "Animacion.h"
#include <cmath>

// Distancia entre dos puntos
float distanciaEntre(Vector2 a, Vector2 b){
	float dx = b.x - a.x;
	float dy = b.y - a.y;
	
	return sqrtf(dx * dx + dy * dy);
}
	
	Animacion::Animacion(){
		velocidad = 120;
		tiempoEspera = 1.0f;
		reiniciar();
	}
	
	Animacion::Animacion(float velocidad){
		this->velocidad = velocidad;
		tiempoEspera = 1.0f;
		reiniciar();
	}
	
	// Vuelve a poner la animacion en el origen
	void Animacion::reiniciar(){
		recorrido = 0;
		espera = 0;
	}
	
	
	float Animacion::largoTotal(Vector2 puntos[], int cantidad){
		float total = 0;
		
		for(int k = 0; k < cantidad - 1; k++){
			total += distanciaEntre(puntos[k], puntos[k + 1]);
		}
		
		return total;
	}

	void Animacion::actualizar(Vector2 puntos[], int cantidad){
		float total = largoTotal(puntos, cantidad);
		
		if(total <= 0){
			return;
		}
		
		float segundos = GetFrameTime();
		
		if(recorrido < total){
			recorrido += velocidad * segundos;
			
			if(recorrido > total){
				recorrido = total;    // que no se pase del destino
			}
		}
		else{
			espera += segundos;
			
			if(espera >= tiempoEspera){
				reiniciar();
			}
		}
	}
	
	// Calcula donde va y hacia donde mira, segun lo recorrido
	bool Animacion::obtenerPosicion(Vector2 puntos[], int cantidad, Vector2& posicion, float& angulo){
		if(cantidad < 2){
			return false;
		}
		
		float restante = recorrido;
		
		for(int k = 0; k < cantidad - 1; k++){
			Vector2 inicio = puntos[k];
			Vector2 fin = puntos[k + 1];
			
			float largo = distanciaEntre(inicio, fin);
			bool esUltimoTramo = (k == cantidad - 2);
			
			// si lo que falta cabe en este tramo, aqui esta
			if(restante <= largo || esUltimoTramo){
				

				float t = 0;
				
				if(largo > 0){
					t = restante / largo;
				}
				if(t > 1){
					t = 1;
				}
				
				// interpolacion lineal
				posicion.x = inicio.x + (fin.x - inicio.x) * t;
				posicion.y = inicio.y + (fin.y - inicio.y) * t;
				
				// angulo del tramo, en grados
				angulo = atan2f(fin.y - inicio.y, fin.x - inicio.x) * RAD2DEG;
				
				return true;
			}
			

			restante -= largo;
		}
		
		return false;
	}
