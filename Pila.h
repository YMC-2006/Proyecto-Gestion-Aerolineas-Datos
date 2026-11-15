#ifndef PILA_H
#define PILA_H

class Pila {
private:
	struct Nodo {
		int dato;
		Nodo* siguiente;
	};
	
	Nodo* tope;
	
public:
	Pila();
	~Pila();
	
	void push(int dato);
	int pop();
	bool vacia();
};

#endif
