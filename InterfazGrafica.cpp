#include "InterfazGrafica.h"
using namespace std;


InterfazGrafica::InterfazGrafica(int ancho, int alto,const char* titulo){
	
	anchoPantalla = ancho;
	altoPantalla = alto;
	
	InitWindow(anchoPantalla, altoPantalla, titulo);
	InitAudioDevice();
	//cargarAssets();
	musica.looping = true;
	
	
	
}

InterfazGrafica::~InterfazGrafica(){

}

void InterfazGrafica::mostrarPantallaPrincipal(){
	DrawTexture(fondo, 0, 0, WHITE);
	
	
}


void InterfazGrafica::empezar(){
	
	while (!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		if(pantallaActual == PRINCIPAL){
			mostrarPantallaPrincipal();
		}
		
		
		EndDrawing();
	}
	
	CloseWindow();
	

}
