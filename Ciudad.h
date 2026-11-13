#ifndef CIUDAD_H
#define CIUDAD_H
#include <string>
using namespace std;

class Ciudad {
private:
	int id;
	string nombre;
	float x, y;   // posición en el mapa, para dibujarla
public:
	Ciudad();
	Ciudad(int id, string nombre, float x, float y);
	
	int getId();
	string getNombre();
	float getX();
	float getY();
	
	void setNombre(string nombre);
	void setPosicion(float x, float y);
};
#endif
