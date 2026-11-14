#ifndef GESTORCLIENTE_H
#define GESTORCLIENTE_H
using namespace std;


class GestorCliente {
private:
	Grafo* grafo;     // apunta al MISMO grafo del admin
	
public:
	GestorCliente();
	void setGrafo(Grafo* grafo);
	

};
#endif
