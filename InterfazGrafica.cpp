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
	fondoRegistros=LoadTexture("assets/vistaConCuadritoParaRegistros.png");
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
	
	//detectar clicks
	//si detecto el boton izquierdo del mouse
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse=GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnAdministrador)){
			pantallaActual=ADMINISTRATIVO;
		}
	}

}

void InterfazGrafica::mostrarPantallaAdministrador(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);

	DrawText("ADMINISTRACION", 365, 176, 40, BLUE);

	Rectangle btnAeronaves = {380, 290, 300, 60};
	Rectangle btnRutas = {380, 390, 300, 60};
	DrawRectangleRec(btnAeronaves, BLUE);
	DrawRectangleRec(btnRutas, BLUE);
	
	DrawText("AERONAVES", 434, 305, 30, WHITE);
	DrawText("RUTAS", 479, 405, 30, WHITE);
}


void InterfazGrafica::empezar(){
	
	while (!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		if(pantallaActual == PRINCIPAL){
			mostrarPantallaPrincipal();
		}
		if(pantallaActual == ADMINISTRATIVO){
			mostrarPantallaAdministrador();
		}
		
		
		EndDrawing();
	}
	
	CloseWindow();
	

}
