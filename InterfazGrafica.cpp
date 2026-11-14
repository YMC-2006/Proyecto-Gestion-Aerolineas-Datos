#include "InterfazGrafica.h"
using namespace std;

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cmath>

// Un solo campo recibe el teclado. Tab cambia al siguiente.
void editarCampos(string textos[], Rectangle cajas[], int cantidad, int& activo){
	if(cantidad <= 0){
		return;
	}
	
	// Seleccionar el campo donde se hizo clic.
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		activo = -1;
		Vector2 mouse = GetMousePosition();
		
		for(int i = 0; i < cantidad; i++){
			if(CheckCollisionPointRec(mouse, cajas[i])){
				activo = i;
			}
		}
	}
	
	// Cambiar de campo con Tab.
	if(IsKeyPressed(KEY_TAB)){
		activo++;
		
		if(activo >= cantidad){
			activo = 0;
		}
	}
	
	// Leer los caracteres escritos.
	int caracter = GetCharPressed();
	
	while(caracter > 0){
		bool hayCampoSeleccionado;
		bool esCaracterPermitido;
		
		if(activo >= 0 && activo < cantidad){
			hayCampoSeleccionado = true;
		}else{
			hayCampoSeleccionado = false;
		}
		
		// no acepto letras con tilde, espacios, numeros y en simbolos solo los necesarios
		if(caracter >= 32 && caracter <= 126){
			esCaracterPermitido = true;
		}else{
			esCaracterPermitido = false;
		}
		
		if(hayCampoSeleccionado && esCaracterPermitido){
			int cantidadCaracteres = textos[activo].length();
			
			if(cantidadCaracteres < 48){
				char letra = (char)caracter;
				textos[activo] = textos[activo] + letra;
			}
		}
		
		caracter = GetCharPressed();
	}
	
	// Borrar el ultimo caracter con retroceso
	if(activo >= 0 && activo < cantidad){
		if(IsKeyPressed(KEY_BACKSPACE)){
			int largo = textos[activo].length();
			
			if(largo > 0){
				textos[activo].erase(largo - 1, 1);
			}
		}
	}
	
	// Dibujar cada campo y su texto.
	for(int i = 0; i < cantidad; i++){
		Rectangle campo = cajas[i];
		string texto = textos[i];
		
		DrawRectangleRec(campo, WHITE);
		
		if(activo == i){
			DrawRectangleLinesEx(campo, 2, ORANGE);
		}else{
			DrawRectangleLinesEx(campo, 2, BLUE);
		}
		
		int margenHorizontal = 5;
		int margenVertical = 2;
		
		int x = campo.x + margenHorizontal;
		int y = campo.y + margenVertical;
		int ancho = campo.width - margenHorizontal * 2;
		int alto = campo.height - margenVertical * 2;
		
		// Evitar que el texto se salga del campo.
		BeginScissorMode(x, y, ancho, alto);
		DrawText(texto.c_str(), campo.x + 8, campo.y + 12, 20, DARKBLUE);
		EndScissorMode();
	}
}

 bool leerCoordenada(string texto, float& numero){
	char* fin;
	errno = 0;
	//strtof convierte el texto en un numero tipo float
	numero = strtof(texto.c_str(), &fin);
	
	// Si no se pudo convertir ningun caracter.
	if(fin == texto.c_str()){
		return false;
	}
	
	// Si quedaron letras u otros caracteres sin convertir.
	if(*fin != '\0'){
		return false;
	}
	
	// Si el numero esta fuera del rango permitido para un float.
	if(errno == ERANGE){
		return false;
	}
	
	// Si el resultado es infinito o NaN.
	if(!isfinite(numero)){
		return false;
	}
	return true;
}

	bool leerCapacidad(string texto, int& capacidad){
		if(texto == ""){
			return false;
		}
		
		// Comprobar que todos los caracteres sean digitos.
		for(int i = 0; i < (int)texto.length(); i++){
			if(texto[i] < '0' || texto[i] > '9'){
				return false;
			}
		}
		
		// Convertir el texto a un numero entero en base 10.
		errno = 0;
		char* fin;
		long numero = strtol(texto.c_str(), &fin, 10);
		
		// Comprobar que la conversion no excedio el rango de long.
		if(errno == ERANGE){
			return false;
		}
		
		// Comprobar que no quedo texto sin convertir.
		if(*fin != '\0'){
			return false;
		}
		
		// La capacidad debe ser mayor que cero.
		if(numero <= 0){
			return false;
		}
		
		// Comprobar que el numero cabe en un int.
		if(numero > INT_MAX){
			return false;
		}
		
		capacidad = (int)numero;
		return true;
	}
	

InterfazGrafica::InterfazGrafica(int ancho, int alto,const char* titulo){
	
	anchoPantalla = ancho;
	altoPantalla = alto;
	
	InitWindow(anchoPantalla, altoPantalla, titulo);
	InitAudioDevice();
	cargarAssets();

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
	DrawText("REGISTRAR AERONAVE", 310, 175, 30, BLUE);
	DrawText("Placa", 270, 250, 25, BLUE);
	DrawText("Capacidad", 550, 250, 25, BLUE);
	DrawText("Marca", 270, 360, 25, BLUE);
	DrawText("Modelo", 550, 360, 25, BLUE);
	Rectangle cajas[4] = {{270,285,240,45}, {550,285,240,45},{270,395,240,45}, {550,395,240,45}};
	editarCampos(camposAeronave, cajas, 4, campoAeronaveActivo);
	Rectangle volver = {270,510,200,50};
	Rectangle registrar = {550,510,240,50};
	DrawRectangleLinesEx(volver, 2, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);
	DrawText(mensajeAeronave.c_str(), 270, 580, 18, DARKBLUE);
	
	int cantidad = gestor.obtenerCantidadAeronaves();
	string mensaje = "Aeronaves registradas: " + to_string(cantidad) + " / 50";
	DrawText(mensaje.c_str(), 270, 610, 20, BLUE);
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), volver)) pantallaActual = ADMINISTRATIVO;
		else if(CheckCollisionPointRec(GetMousePosition(), registrar)){
			int capacidad;
			if(!leerCapacidad(camposAeronave[1], capacidad)){
				mensajeAeronave = "La capacidad debe ser un entero mayor que cero.";
			}else if(gestor.registrarAeronave(camposAeronave[0], capacidad,camposAeronave[2], camposAeronave[3])){
				mensajeAeronave = "Aeronave registrada correctamente.";
				//limpiamos los campos despues de registrar la nave para que queden limpios por si queremos registrar otro 
				for(int i = 0; i < 4; i++){
					camposAeronave[i] = "";
				}
			}else{
				mensajeAeronave = "Revise campos vacios, placa repetida o limite de 50.";
			}
		}
	}
}

void InterfazGrafica::mostrarPantallaCiudades(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("REGISTRAR CIUDAD / AEROPUERTO", 270, 175, 27, BLUE);
	DrawText("Nombre (el ID se asigna automaticamente)", 270, 245, 20, BLUE);
	DrawText("X del mapa (0 a 600)", 270, 350, 20, BLUE);
	DrawText("Y del mapa (0 a 400)", 550, 350, 20, BLUE);
	Rectangle cajas[3] = {{270,285,520,45}, {270,390,240,45}, {550,390,240,45}};
	editarCampos(camposCiudad, cajas, 3, campoCiudadActivo);
	DrawText("Coordenadas dentro del mapa, no de toda la ventana.", 270, 460, 18, BLUE);
	Rectangle volver = {270,510,200,50};
	Rectangle registrar = {550,510,240,50};
	DrawRectangleLinesEx(volver, 2, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);
	DrawText(mensajeCiudad.c_str(), 270, 580, 18, DARKBLUE);
	
	
	int cantidad = gestor.obtenerCantidadCiudades();
	string mensaje = "Ciudades registradas: " + to_string(cantidad) + " / 50";
	DrawText(mensaje.c_str(), 270, 610, 20, BLUE);
	
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, volver)){
			pantallaActual = ADMINISTRATIVO;
		}else if(CheckCollisionPointRec(mouse, registrar)){
			float x, y;
			
			bool xValida = leerCoordenada(camposCiudad[1], x);
			bool yValida = leerCoordenada(camposCiudad[2], y);
			
			if(xValida == false || yValida == false){
				mensajeCiudad = "Las coordenadas deben ser numeros validos.";
			}else if(x < 0 || x > 600){
				mensajeCiudad = "X debe estar entre 0 y 600.";
			}else if(y < 0 || y > 400){
				mensajeCiudad = "Y debe estar entre 0 y 400.";
			}else{
				bool registrada = gestor.registrarCiudad(camposCiudad[0], x, y);
				
				if(registrada){
					mensajeCiudad = "Ciudad registrada correctamente.";
					
					// Limpiar los campos del formulario.
					for(int i = 0; i < 3; i++){
						camposCiudad[i] = "";
					}
				}else{
					mensajeCiudad = "Revise nombre vacio, repetido o limite de 50 ciudades.";
				}
			}
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
