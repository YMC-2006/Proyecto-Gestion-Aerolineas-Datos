#include "InterfazGrafica.h"
using namespace std;


InterfazGrafica::InterfazGrafica(int ancho, int alto,const char* titulo){
	
	anchoPantalla = ancho;
	altoPantalla = alto;
	
	InitWindow(anchoPantalla, altoPantalla, titulo);
	InitAudioDevice();
	cargarAssets();
	musica.looping = true;
	
	btnCliente = { 700, 500, 180, 50 };
	btnAdministrador = { 900, 500, 180, 50 };
	
}

InterfazGrafica::~InterfazGrafica(){

}

void InterfazGrafica::cargarAssets(){
	fondo = LoadTexture("assets/fondoPantallaPrincipal.png");
}

void InterfazGrafica::liberarAssets(){
	UnloadTexture(fondo);
}

void InterfazGrafica::mostrarPantallaPrincipal(){
	DrawTexture(fondo, 0, 0, WHITE);
	
	Rectangle btnCliente = {300, 400, 260, 60};
	DrawRectangleRec(btnCliente, WHITE);
	DrawText("Cliente", 380, 420, 30, BLUE);
	
	Rectangle btnAdministrador = {600, 400, 260, 60};
	DrawRectangleRec(btnAdministrador, WHITE);
	DrawText("Administrador", 630, 420, 30, BLUE);
	
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
