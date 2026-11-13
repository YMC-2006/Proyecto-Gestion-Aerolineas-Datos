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
	
	comboOrigen = ListaDesplegable(700, 150, 250, 40);
	comboDestino = ListaDesplegable(1000, 150, 250, 40);
	
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
	
	Rectangle btnCliente = {350, 400, 260, 60};
	DrawRectangleRec(btnCliente, WHITE);
	DrawText("Cliente", 400, 420, 30, BLUE);
	
	Rectangle btnAdministrador = {650, 400, 260, 60};
	DrawRectangleRec(btnAdministrador, WHITE);
	DrawText("Administrador", 680, 420, 30, BLUE);
	
	
	DrawText("GESTION DE VUELOS", 300, 100, 60, WHITE);
	
	//detectar clicks
	//si detecto el boton izquierdo del mouse
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse=GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnAdministrador)){
			pantallaActual=ADMINISTRATIVO;
		}
	}
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, btnCliente)){
			cargarCombosCliente();
			pantallaActual = CLIENTE;
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


// ============================= CLIENTE YULI :P ====================================
// Llena las listas de origen y destino.
// Se llama una vez, al entrar a la pantalla del cliente.
void InterfazGrafica::cargarCombosCliente(){
	comboOrigen.limpiar();
	comboDestino.limpiar();
	
	// DATOS DE PRUEBA (cambiar por las ciudades del grafo cuando esté listo)
	string ciudadesPrueba[6] = {"San Jose", "Limon", "Liberia", "Puntarenas", "Cartago", "Perez Zeledon"};
	
	for(int i = 0; i < 6; i++){
		comboOrigen.agregarOpcion(ciudadesPrueba[i]);
		comboDestino.agregarOpcion(ciudadesPrueba[i]);
	}
}



void InterfazGrafica::mostrarPantallaCliente(){
	
	ClearBackground(WHITE);
	
	Rectangle encabezado = {0, 0, 1300, 70};
	DrawRectangleRec(encabezado, BLUE);
	DrawText("Seleccion de viaje", 500, 10, 40, WHITE);
	

	Rectangle btnVolver = {10, 10, 200, 50};
	DrawRectangleLines(10, 10, 150, 50, WHITE);
	DrawText("VOLVER", 25, 25, 25, WHITE);
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, btnVolver)){
			pantallaActual = PRINCIPAL;
		}
	}
	
	Rectangle machoteMapa = {50, 100, 600,400};
	DrawRectangleRounded(machoteMapa, 0.1, 0, BLUE);
	DrawText("MAPA", 70, 120, 40, WHITE);
	
	DrawCircle(100, 250, 10, WHITE);
	
	
	DrawText("BUSCAR POR: ", 900, 300, 20, BLACK);
	DrawText("COSTO", 800, 350, 20, BLACK);
	DrawText("TIEMPO", 1090, 350, 20, BLACK);
	
	DrawText("DESDE", 700, 120, 20, BLACK);
	DrawText("HASTA",1000, 120, 20, BLACK);
	
	// esto es porque solo uno puede estar abierto a la vez
	if(!comboDestino.estaAbierta()){
		comboOrigen.actualizar();
	}
	if(!comboOrigen.estaAbierta()){
		comboDestino.actualizar();
	}
	
	
	comboDestino.dibujar();
	comboOrigen.dibujar();

	
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
		}else if(pantallaActual == CLIENTE){
			mostrarPantallaCliente();
		}
		
		
		EndDrawing();
	}

	CloseWindow();

}
