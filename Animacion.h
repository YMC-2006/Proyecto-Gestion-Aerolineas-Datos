#ifndef ANIMACION_H
#define ANIMACION_H
#include "raylib.h"


class Animacion {
private:
	float recorrido;       // pixeles avanzados desde el primer punto
	float velocidad;       // pixeles por segundo
	float espera;          // segundos que lleva quieto en el destino
	float tiempoEspera;    // cuanto espera en el destino antes de repetir
	
	float largoTotal(Vector2 puntos[], int cantidad);
	
public:
	Animacion();
	Animacion(float velocidad);
	
	void reiniciar();
	void actualizar(Vector2 puntos[], int cantidad);
	bool obtenerPosicion(Vector2 puntos[], int cantidad, Vector2& posicion, float& angulo);
};

#endif
