#ifndef PASAJERO_H
#define PASAJERO_H
#include <string>
using namespace std;

// Un registro representa la asignacion de un pasajero a un vuelo.
struct Pasajero {
	string identificacion;
	string nombre;
	string codigoVuelo;
};
#endif
