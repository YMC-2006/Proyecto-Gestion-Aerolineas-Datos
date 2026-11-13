#include "ListaDesplegable.h"

ListaDesplegable::ListaDesplegable(){
	caja = {0, 0, 200, 40};
	cantidadOpciones = 0;
	seleccionada = -1;
	abierta = false;
}

ListaDesplegable::ListaDesplegable(float x, float y, float ancho, float alto){
	caja = {x, y, ancho, alto};
	cantidadOpciones = 0;
	seleccionada = -1;
	abierta = false;
}

void ListaDesplegable::agregarOpcion(string texto){
	if(cantidadOpciones < 50){
		opciones[cantidadOpciones] = texto;
		cantidadOpciones++;
	}
}

void ListaDesplegable::limpiar(){
	cantidadOpciones = 0;
	seleccionada = -1;
	abierta = false;
}

int ListaDesplegable::getSeleccionada(){
	return seleccionada;
}

bool ListaDesplegable::estaAbierta(){
	return abierta;
}

// Revisa los clics abrir la lista o escoger una op
void ListaDesplegable::actualizar(){
	if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		return;
	}
	
	Vector2 mouse = GetMousePosition();
	
	if(abierta){
		// ver si toco alguna opción
		for(int i = 0; i < cantidadOpciones; i++){
			Rectangle opcion = {caja.x, caja.y + caja.height * (i + 1), caja.width, caja.height};
			
			if(CheckCollisionPointRec(mouse, opcion)){
				seleccionada = i;
			}
		}
		abierta = false;   // se cierra al hacer clic en cualquier lado
	}
	else if(CheckCollisionPointRec(mouse, caja)){
		abierta = true;
	}
}

void ListaDesplegable::dibujar(){
	// caja principal
	DrawRectangleRec(caja, WHITE);
	DrawRectangleLinesEx(caja, 2, DARKBLUE);
	
	if(seleccionada == -1){
		DrawText("Seleccione...", caja.x + 10, caja.y + 10, 20, GRAY);
	}
	else{
		DrawText(opciones[seleccionada].c_str(), caja.x + 10, caja.y + 10, 20, BLACK);
	}
	
	DrawText("v", caja.x + caja.width - 25, caja.y + 10, 20, DARKBLUE);
	
	// opciones (solo si esta abierta)
	if(abierta){
		Vector2 mouse = GetMousePosition();
		
		for(int i = 0; i < cantidadOpciones; i++){
			Rectangle opcion = {caja.x, caja.y + caja.height * (i + 1), caja.width, caja.height};
			
			if(CheckCollisionPointRec(mouse, opcion)){
				DrawRectangleRec(opcion, SKYBLUE);    // resalta donde esta el mouse
			}
			else{
				DrawRectangleRec(opcion, WHITE);
			}
			
			DrawRectangleLinesEx(opcion, 1, LIGHTGRAY);
			DrawText(opciones[i].c_str(), opcion.x + 10, opcion.y + 10, 20, BLACK);
		}
	}
}
