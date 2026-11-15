#include "InterfazGrafica.h"
using namespace std;

#include <cstdlib>
#include <cerrno>
#include <climits>
#include <cmath>



InterfazGrafica::InterfazGrafica(int ancho, int alto,const char* titulo){
	
	anchoPantalla = ancho;
	altoPantalla = alto;
	
	InitWindow(anchoPantalla, altoPantalla, titulo);
	InitAudioDevice();
	cargarAssets();
	
	comboOrigen = ListaDesplegable(700, 150, 250, 40);
	comboDestino = ListaDesplegable(1000, 150, 250, 40);
	
	mapaCliente = {50, 100, 600, 400};
	criterioBusqueda = POR_COSTO;
	mensajeCliente = "";
	
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

	Rectangle btnAeronaves = {380, 240, 300, 60};
	Rectangle btnCiudades = {380, 320, 300, 60};
	Rectangle btnRutas = {380, 400, 300, 60};
	Rectangle btnVuelos = {380, 480, 300, 60};
	Rectangle btnVolver = {430, 570, 200, 50};
	DrawRectangleRec(btnAeronaves, BLUE);
	DrawRectangleRec(btnCiudades, BLUE);
	DrawRectangleRec(btnRutas, BLUE);
	DrawRectangleRec(btnVuelos, BLUE);
	DrawRectangleLines(430, 570, 200, 50, BLUE);
	
	DrawText("AERONAVES", 434, 255, 30, WHITE);
	DrawText("CIUDADES", 445, 335, 30, WHITE);
	DrawText("RUTAS", 479, 415, 30, WHITE);
	DrawText("VUELOS", 465, 495, 30, WHITE);
	DrawText("VOLVER", 475, 585, 25, BLUE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		
		Vector2 mouse = GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnAeronaves)){
			pantallaActual = AERONAVES;
		}else if(CheckCollisionPointRec(mouse, btnCiudades)){
			pantallaActual = CIUDADES;
		}else if(CheckCollisionPointRec(mouse, btnRutas)){
			pantallaActual = RUTAS;
		}else if(CheckCollisionPointRec(mouse, btnVuelos)){
			pantallaActual = VUELOS;
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
	
	int cantidad = gestorAdmin.obtenerCantidadAeronaves();
	string mensaje = "Aeronaves registradas: " + to_string(cantidad) + " / 50";
	DrawText(mensaje.c_str(), 270, 610, 20, BLUE);
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), volver)) pantallaActual = ADMINISTRATIVO;
		else if(CheckCollisionPointRec(GetMousePosition(), registrar)){
			int capacidad;
			if(!leerCapacidad(camposAeronave[1], capacidad)){
				mensajeAeronave = "La capacidad debe ser un entero mayor que cero.";
			}else if(gestorAdmin.registrarAeronave(camposAeronave[0], capacidad,camposAeronave[2], camposAeronave[3])){
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
	
	
	int cantidad = gestorAdmin.obtenerCantidadCiudades();
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
				bool registrada = gestorAdmin.registrarCiudad(camposCiudad[0], x, y);
				
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


bool leerCosto(string texto, double& costo){
	char* fin;
	errno = 0;
	costo = strtod(texto.c_str(), &fin);
	if(fin == texto.c_str() || *fin != '\0') return false;
	if(errno == ERANGE || !isfinite(costo) || costo < 0) return false;
	return true;
}

void InterfazGrafica::mostrarPantallaRutas(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("GESTION DE RUTAS", 330, 175, 30, BLUE);
	DrawText("Ciudad de origen", 270, 250, 20, BLUE);
	DrawText("Ciudad de destino", 550, 250, 20, BLUE);
	DrawText("Costo", 270, 360, 25, BLUE);
	DrawText("Duracion en minutos", 550, 360, 20, BLUE);
	Rectangle cajas[4] = {{270,285,240,45}, {550,285,240,45}, {270,395,240,45}, {550,395,240,45}};
	editarCampos(camposRuta, cajas, 4, campoRutaActivo);
	DrawText("Use nombres de ciudades ya registradas. Solo se guarda la ida.", 270, 460, 16, BLUE);

	Rectangle btnVolver = {270,575,170,45};
	Rectangle btnRegistrar = {270,500,160,50};
	Rectangle btnModificar = {450,500,160,50};
	Rectangle btnEliminar = {630,500,160,50};
	DrawRectangleLines(270, 575, 170, 45, BLUE);
	DrawText("VOLVER", 300, 587, 22, BLUE);
	DrawRectangleRec(btnRegistrar, BLUE);
	DrawText("REGISTRAR", 285, 517, 20, WHITE);
	DrawRectangleRec(btnModificar, BLUE);
	DrawText("MODIFICAR", 462, 517, 20, WHITE);
	DrawRectangleLines(630, 500, 160, 50, BLUE);
	DrawText("ELIMINAR", 650, 517, 20, BLUE);
	DrawText(mensajeRuta.c_str(), 270, 640, 16, DARKBLUE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, btnVolver)){
			pantallaActual = ADMINISTRATIVO;
		}else if(CheckCollisionPointRec(mouse, btnRegistrar) ||
		         CheckCollisionPointRec(mouse, btnModificar) ||
		         CheckCollisionPointRec(mouse, btnEliminar)){
			Ciudad origen, destino;
			double costo;
			int minutos;
			if(!gestorAdmin.obtenerCiudadPorNombre(camposRuta[0], origen) ||
			   !gestorAdmin.obtenerCiudadPorNombre(camposRuta[1], destino)){
				mensajeRuta = "Las dos ciudades deben estar registradas.";
			}else if(origen.getId() == destino.getId()){
				mensajeRuta = "El origen y el destino deben ser diferentes.";
			}else if(CheckCollisionPointRec(mouse, btnEliminar)){
				// Para eliminar solo se necesitan las dos ciudades.
				if(gestorAdmin.eliminarRuta(origen.getId(), destino.getId())){
					mensajeRuta = "Ruta eliminada. El regreso no se modifica.";
				}else{
					mensajeRuta = "Ruta inexistente o asignada a un vuelo.";
				}
			}else if(!leerCosto(camposRuta[2], costo)){
				mensajeRuta = "Costo invalido. Use punto decimal y un valor no negativo.";
			}else if(!leerCapacidad(camposRuta[3], minutos)){
				mensajeRuta = "La duracion debe ser un entero mayor que cero.";
			}else if(CheckCollisionPointRec(mouse, btnModificar)){
				if(gestorAdmin.modificarRuta(origen.getId(), destino.getId(), costo, minutos)){
					mensajeRuta = "Ruta modificada correctamente.";
				}else{
					mensajeRuta = "No existe esa ruta. Primero debe registrarla.";
				}
			}else if(gestorAdmin.registrarRuta(origen.getId(), destino.getId(), costo, minutos)){
				mensajeRuta = "Ruta registrada correctamente.";
				for(int i = 0; i < 4; i++) camposRuta[i] = "";
			}else{
				mensajeRuta = "Ya existe una ruta desde ese origen hacia ese destino.";
			}
		}
	}
}

// ============================= CLIENTE YULI :P ====================================
// Llena las listas de origen y destino.
// Se llama una vez, al entrar a la pantalla del cliente.

void InterfazGrafica::mostrarPantallaVuelos(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("REGISTRAR VUELO", 340, 175, 30, BLUE);
	DrawText("Codigo del vuelo", 270, 250, 22, BLUE);
	DrawText("Placa de aeronave", 550, 250, 22, BLUE);
	DrawText("Ciudad de origen", 270, 360, 22, BLUE);
	DrawText("Ciudad de destino", 550, 360, 22, BLUE);
	Rectangle cajas[4] = {{270,285,240,45}, {550,285,240,45}, {270,395,240,45}, {550,395,240,45}};
	editarCampos(camposVuelo, cajas, 4, campoVueloActivo);
	DrawText("Primero registre la aeronave y la ruta de ida.", 270, 460, 18, BLUE);
	Rectangle volver = {270,510,200,50};
	Rectangle registrar = {550,510,240,50};
	DrawRectangleLines(270, 510, 200, 50, BLUE);
	DrawText("VOLVER", 315, 525, 25, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 590, 525, 25, WHITE);
	DrawText(mensajeVuelo.c_str(), 270, 580, 16, DARKBLUE);
	string cantidad = "Vuelos registrados: " + to_string(gestorAdmin.obtenerCantidadVuelos()) + " / 50";
	DrawText(cantidad.c_str(), 270, 610, 20, BLUE);

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, volver)){
			pantallaActual = ADMINISTRATIVO;
		}else if(CheckCollisionPointRec(mouse, registrar)){
			Ciudad origen, destino;
			if(!gestorAdmin.obtenerCiudadPorNombre(camposVuelo[2], origen) ||
			   !gestorAdmin.obtenerCiudadPorNombre(camposVuelo[3], destino)){
				mensajeVuelo = "Las dos ciudades deben estar registradas.";
			}else if(!gestorAdmin.getGrafo()->existeRuta(origen.getId(), destino.getId())){
				mensajeVuelo = "Primero registre una ruta en esa direccion.";
			}else if(gestorAdmin.registrarVuelo(camposVuelo[0], camposVuelo[1], origen.getId(), destino.getId())){
				mensajeVuelo = "Vuelo registrado correctamente.";
				for(int i = 0; i < 4; i++) camposVuelo[i] = "";
			}else{
				mensajeVuelo = "Revise codigo vacio/repetido, placa o limite de 50.";
			}
		}
	}
}

void InterfazGrafica::cargarCombosCliente(){
	comboOrigen.limpiar();
	comboDestino.limpiar();
	
	
	int cantidad = gestorAdmin.obtenerCantidadCiudades();
	for(int i = 0; i < cantidad; i++){
		Ciudad c;
		if(gestorAdmin.obtenerCiudadPorIndice(i, c)){
			comboOrigen.agregarOpcion(c.getNombre());
			comboDestino.agregarOpcion(c.getNombre());
		}
	}
}


void InterfazGrafica::mostrarPantallaCliente(){
	
	ClearBackground(WHITE);

	Rectangle encabezado = {0, 0, 1300, 70};
	DrawRectangleRec(encabezado, BLUE);
	DrawText("Seleccion de viaje", 500, 10, 40, WHITE);
	
	Rectangle btnVolver = {10, 10, 150, 50};
	DrawRectangleLinesEx(btnVolver, 2, WHITE);
	DrawText("VOLVER", 25, 25, 25, WHITE);
	
	// mapa con las ciudades y rutas registradas
	dibujarMapaCliente();
	
	// panel de busqueda
	DrawText("DESDE", 700, 120, 20, BLACK);
	DrawText("HASTA", 1000, 120, 20, BLACK);
	
	Rectangle btnCosto = {700, 330, 250, 45};
	Rectangle btnTiempo = {1000, 330, 250, 45};
	Rectangle btnBuscar = {800, 410, 350, 55};
	
	dibujarBotonOpcion(btnCosto, "MAS BARATA", criterioBusqueda == POR_COSTO);
	dibujarBotonOpcion(btnTiempo, "MAS RAPIDA", criterioBusqueda == POR_TIEMPO);
	
	DrawRectangleRec(btnBuscar, DARKBLUE);
	DrawText("BUSCAR RUTA", btnBuscar.x + 80, btnBuscar.y + 15, 26, WHITE);
	
	DrawText(mensajeCliente.c_str(), 700, 490, 20, RED);
	
	if(!comboDestino.estaAbierta()){
		comboOrigen.actualizar();
	}
	if(!comboOrigen.estaAbierta()){
		comboDestino.actualizar();
	}
	

	// clics de los botones
	bool hayListaAbierta = comboOrigen.estaAbierta() || comboDestino.estaAbierta();
	
	if(!hayListaAbierta && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnVolver)){
			pantallaActual = PRINCIPAL;
		}
		else if(CheckCollisionPointRec(mouse, btnCosto)){
			criterioBusqueda = POR_COSTO;
		}
		else if(CheckCollisionPointRec(mouse, btnTiempo)){
			criterioBusqueda = POR_TIEMPO;
		}
		else if(CheckCollisionPointRec(mouse, btnBuscar)){
			buscarRutaCliente();
		}
	}
	
	
	// las listas van de ultimo para que queden encima de todo
	comboDestino.dibujar();
	comboOrigen.dibujar();
}

// Las coordenadas de la ciudad est�n guardadas en un mapa de 600 x 400.
// Esta funci�n las pasa a la posici�n real dentro del mapa del cliente.
Vector2 InterfazGrafica::posicionEnMapa(Ciudad& ciudad){
	float escalaX = mapaCliente.width / 600.0f;
	float escalaY = mapaCliente.height / 400.0f;
	
	Vector2 posicion;
	posicion.x = mapaCliente.x + ciudad.getX() * escalaX;
	posicion.y = mapaCliente.y + ciudad.getY() * escalaY;
	
	return posicion;
}


// Dibuja una l�nea por cada ruta registrada en el grafo
void InterfazGrafica::dibujarRutasMapa(){
	Grafo* grafo = gestorAdmin.getGrafo();
	int cantidad = grafo->obtenerCantidadCiudades();
	
	for(int i = 0; i < cantidad; i++){
		for(int j = 0; j < cantidad; j++){
			
			if(i == j){
				continue;
			}
			
			Ciudad origen;
			Ciudad destino;
			grafo->obtenerCiudadPorIndice(i, origen);
			grafo->obtenerCiudadPorIndice(j, destino);
			
			if(grafo->existeRuta(origen.getId(), destino.getId())){
				Vector2 inicio = posicionEnMapa(origen);
				Vector2 fin = posicionEnMapa(destino);
				
				DrawLineEx(inicio, fin, 2, LIGHTGRAY);
			}
		}
	}
}


// Dibuja cada ciudad como un c�rculo con su nombre
void InterfazGrafica::dibujarCiudadesMapa(){
	Grafo* grafo = gestorAdmin.getGrafo();
	int cantidad = grafo->obtenerCantidadCiudades();
	
	for(int i = 0; i < cantidad; i++){
		Ciudad ciudad;
		grafo->obtenerCiudadPorIndice(i, ciudad);
		
		Vector2 posicion = posicionEnMapa(ciudad);
		
		// color seg�n si es origen, destino o ninguna
		Color color = DARKBLUE;
		
		if(i == comboOrigen.getSeleccionada()){
			color = GREEN;
		}
		else if(i == comboDestino.getSeleccionada()){
			color = RED;
		}
		
		DrawCircleV(posicion, 9, color);
		DrawCircleLines(posicion.x, posicion.y, 9, BLACK);
		
		DrawText(ciudad.getNombre().c_str(), posicion.x + 12, posicion.y - 8, 18, BLACK);
	}
}

// juntamos todo aca dibujamos el rectangulo del mapa y las lineas
void InterfazGrafica::dibujarMapaCliente(){
	DrawRectangleRounded(mapaCliente, 0.05, 0, WHITE);
	DrawRectangleRoundedLines(mapaCliente, 0.05, 0, 2, DARKBLUE);
	
	dibujarRutasMapa();
	dibujarCiudadesMapa();
}


void InterfazGrafica::dibujarBotonOpcion(Rectangle boton, const char* texto, bool seleccionado){
	if(seleccionado){
		DrawRectangleRec(boton, BLUE);
		DrawText(texto, boton.x + 20, boton.y + 12, 22, WHITE);
	}
	else{
		DrawRectangleRec(boton, WHITE);
		DrawRectangleLinesEx(boton, 2, BLUE);
		DrawText(texto, boton.x + 20, boton.y + 12, 22, BLUE);
	}
}
	
	
// Revisa la seleccion del cliente y busca la ruta
void InterfazGrafica::buscarRutaCliente(){
	int origen = comboOrigen.getSeleccionada();
	int destino = comboDestino.getSeleccionada();
	
	if(origen == -1 || destino == -1){
		mensajeCliente = "Seleccione el origen y el destino.";
		return;
	}
	
	if(origen == destino){
		mensajeCliente = "El origen y el destino deben ser distintos.";
		return;
	}
	
	
	mensajeCliente = "";
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
		}else if(pantallaActual == VUELOS){
			mostrarPantallaVuelos();
		}else if(pantallaActual == CLIENTE){
			mostrarPantallaCliente();
		}
		
		
		EndDrawing();
	}

	CloseWindow();

}
