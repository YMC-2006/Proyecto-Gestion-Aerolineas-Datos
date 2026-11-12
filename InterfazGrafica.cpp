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
	UnloadTexture(fondoRegistros);
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

	Rectangle btnAeronaves = {380, 260, 300, 60};
	Rectangle btnCiudades = {380, 350, 300, 60};
	Rectangle btnRutas = {380, 440, 300, 60};
	Rectangle btnVolver = {430, 540, 200, 50};
	DrawRectangleRec(btnAeronaves, BLUE);
	DrawRectangleRec(btnCiudades, BLUE);
	DrawRectangleRec(btnRutas, BLUE);
	DrawRectangleLines(430, 540, 200, 50, BLUE);
	
	DrawText("AERONAVES", 434, 275, 30, WHITE);
	DrawText("CIUDADES", 445, 365, 30, WHITE);
	DrawText("RUTAS", 479, 455, 30, WHITE);
	DrawText("VOLVER", 475, 555, 25, BLUE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		
		Vector2 mouse = GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnAeronaves)){
			pantallaActual = AERONAVES;
		}else if(CheckCollisionPointRec(mouse, btnCiudades)){
			pantallaActual = CIUDADES;
		}else if(CheckCollisionPointRec(mouse, btnRutas)){
			pantallaActual = RUTAS;
		}else if(CheckCollisionPointRec(mouse, btnVolver)){
			pantallaActual = PRINCIPAL;
		}
	}
}

void InterfazGrafica::mostrarPantallaAeronaves(){
	
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	
	//formulario de aeronaves
	DrawText("REGISTRAR AERONAVE", 310, 175, 30, BLUE);
	DrawText("Placas", 270, 250, 25, BLUE);
	DrawRectangleLines(270, 285, 240, 45, BLUE);
	DrawText("Capacidad", 550, 250, 25, BLUE);
	DrawRectangleLines(550, 285, 240, 45, BLUE);
	DrawText("Marca", 270, 360, 25, BLUE);
	DrawRectangleLines(270, 395, 240, 45, BLUE);
	DrawText("Modelo", 550, 360, 25, BLUE);
	DrawRectangleLines(550, 395, 240, 45, BLUE);

	//botones
	Rectangle btnVolver = {270, 510, 200, 50};
	Rectangle btnRegistrar = {550, 510, 240, 50};
	DrawRectangleLines(270, 510, 200, 50, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(btnRegistrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), btnVolver)){
			pantallaActual = ADMINISTRATIVO;
		}
	}
}

void InterfazGrafica::mostrarPantallaCiudades(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	
	//formulario de ciudad
	DrawText("REGISTRAR CIUDAD", 330, 175, 30, BLUE);
	DrawText("ID de la ciudad", 300, 270, 25, BLUE);
	DrawRectangleLines(300, 305, 460, 45, BLUE);
	DrawText("Nombre de la ciudad", 300, 380, 25, BLUE);
	DrawRectangleLines(300, 415, 460, 45, BLUE);

	Rectangle btnVolver = {270, 510, 200, 50};
	Rectangle btnRegistrar = {550, 510, 240, 50};
	DrawRectangleLines(270, 510, 200, 50, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(btnRegistrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);


	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), btnVolver)){
			pantallaActual = ADMINISTRATIVO;
		}
	}
}

void InterfazGrafica::mostrarPantallaRutas(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	
	//formulario de rutas
	DrawText("REGISTRAR RUTA", 355, 175, 30, BLUE);
	
	//duda, eso es por ids o por nombres normales?
	DrawText("ciudad de origen", 270, 250, 20, BLUE);
	DrawRectangleLines(270, 285, 240, 45, BLUE);
	DrawText("Ciudad de destino", 550, 250, 20, BLUE);
	DrawRectangleLines(550, 285, 240, 45, BLUE);
	DrawText("Costo", 270, 360, 25, BLUE);
	DrawRectangleLines(270, 395, 240, 45, BLUE);
	DrawText("Duracion en minutos", 550, 360, 20, BLUE);
	DrawRectangleLines(550, 395, 240, 45, BLUE);

	Rectangle btnVolver = {270, 510, 200, 50};
	Rectangle btnRegistrar = {550, 510, 240, 50};
	DrawRectangleLines(270, 510, 200, 50, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(btnRegistrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), btnVolver)){
			pantallaActual = ADMINISTRATIVO;
		}
	}
}

void InterfazGrafica::empezar(){
	
	while (!WindowShouldClose()){
		BeginDrawing();
		ClearBackground(RAYWHITE);
		
		if(pantallaActual == PRINCIPAL){
			mostrarPantallaPrincipal();
		}
		else if(pantallaActual == ADMINISTRATIVO){
			mostrarPantallaAdministrador();
		}
		else if(pantallaActual == AERONAVES){
			mostrarPantallaAeronaves();
		}
		else if(pantallaActual == CIUDADES){
			mostrarPantallaCiudades();
		}
		else if(pantallaActual == RUTAS){
			mostrarPantallaRutas();
		}
		
		
		EndDrawing();
	}

	CloseWindow();

}
