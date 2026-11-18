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

	gestorCliente.setGrafo(gestorAdmin.getGrafo());
	
	origenExplorado = -1;
	cantidadDestinos = 0;
	scrollearDestinos = 0;
	limpiarResultadosCliente();
}

InterfazGrafica::~InterfazGrafica(){

}

void InterfazGrafica::cargarAssets(){
	fondo = LoadTexture("assets/fondoPantallaPrincipal.png");
	fondoRegistros=LoadTexture("assets/vistaConCuadritoParaRegistros.png");
	fondoCreditos = LoadTexture("assets/fondoCreditos.png");
}

void InterfazGrafica::liberarAssets(){
	UnloadTexture(fondo);
	UnloadTexture(fondoRegistros);
	UnloadTexture(fondoCreditos);
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

	Vector2 mouse;
	
	Rectangle btnCliente = {350, 400, 260, 60};
	DrawRectangleRec(btnCliente, WHITE);
	DrawText("Cliente", 400, 420, 30, BLUE);

	Rectangle btnAdministrador = {650, 400, 260, 60};
	DrawRectangleRec(btnAdministrador, WHITE);
	DrawText("Administrador", 680, 420, 30, BLUE);
	
	Rectangle btnCreditos = {30, 20, 130, 35};
	DrawRectangleRec(btnCreditos, WHITE);
	DrawText("Creditos", 45, 28, 20, BLUE);


	DrawText("GESTION DE VUELOS", 300, 100, 60, WHITE);

	//detectar clicks
	//si detecto el boton izquierdo del mouse
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		mouse=GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnCreditos)){
			pantallaActual = CREDITOS;
		}
	}
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		mouse=GetMousePosition();

		if(CheckCollisionPointRec(mouse, btnAdministrador)){
			pantallaActual = ADMINISTRATIVO;
		}
	}

	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, btnCliente)){
			cargarCombosCliente();
			pantallaActual = CLIENTE;
		}
	}

}

void InterfazGrafica::mostrarPantallaAdministrador(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("ADMINISTRACION", 365, 176, 40, BLUE);
	Rectangle btnAeronaves = {270,260,240,60};
	Rectangle btnCiudades = {550,260,240,60};
	Rectangle btnRutas = {270,350,240,60};
	Rectangle btnVuelos = {550,350,240,60};
	Rectangle btnTripulacion = {270,440,240,60};
	Rectangle btnPasajeros = {550,440,240,60};
	Rectangle btnVolver = {430,550,200,50};
	DrawRectangleRec(btnAeronaves, BLUE);
	DrawRectangleRec(btnCiudades, BLUE);
	DrawRectangleRec(btnRutas, BLUE);
	DrawRectangleRec(btnVuelos, BLUE);
	DrawRectangleRec(btnTripulacion, BLUE);
	DrawRectangleRec(btnPasajeros, BLUE);
	DrawRectangleLines(430, 550, 200, 50, BLUE);
	DrawText("AERONAVES", 290, 280, 28, WHITE);
	DrawText("CIUDADES", 585, 280, 28, WHITE);
	DrawText("RUTAS", 335, 370, 28, WHITE);
	DrawText("VUELOS", 615, 370, 28, WHITE);
	DrawText("TRIPULACION", 285, 460, 28, WHITE);
	DrawText("PASAJEROS", 580, 460, 28, WHITE);
	DrawText("VOLVER", 475, 565, 25, BLUE);
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, btnAeronaves)) pantallaActual = AERONAVES;
		else if(CheckCollisionPointRec(mouse, btnCiudades)) pantallaActual = CIUDADES;
		else if(CheckCollisionPointRec(mouse, btnRutas)) pantallaActual = RUTAS;
		else if(CheckCollisionPointRec(mouse, btnVuelos)) pantallaActual = VUELOS;
		else if(CheckCollisionPointRec(mouse, btnTripulacion)) pantallaActual = TRIPULACION;
		else if(CheckCollisionPointRec(mouse, btnPasajeros)) pantallaActual = PASAJEROS;
		else if(CheckCollisionPointRec(mouse, btnVolver)) pantallaActual = PRINCIPAL;
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
	Rectangle cajas[5] = {{270,285,520,45}, {270,390,240,45}, {550,390,240,45}, {270,670,240,40}, {550,670,320,40}};
	editarCampos(camposCiudad, cajas, 5, campoCiudadActivo);
	DrawText("Coordenadas dentro del mapa, no de toda la ventana.", 270, 460, 18, BLUE);
	Rectangle volver = {270,510,160,50};
	Rectangle registrar = {450,510,160,50};
	Rectangle verArbol = {630,510,160,50};
	DrawRectangleLines(270, 510, 160, 50, BLUE);
	DrawText("VOLVER", 300, 527, 20, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 465, 527, 20, WHITE);
	DrawRectangleLines(630, 510, 160, 50, BLUE);
	DrawText("VER ARBOL", 645, 527, 20, BLUE);
	DrawText(mensajeCiudad.c_str(), 270, 580, 18, DARKBLUE);


	int cantidad = gestorAdmin.obtenerCantidadCiudades();
	string mensaje = "Ciudades registradas: " + to_string(cantidad) + " / 50";
	DrawText(mensaje.c_str(), 270, 610, 20, BLUE);


	DrawText("Nombre actual", 270, 645, 18, BLUE);
	DrawText("Nuevo nombre (solo para renombrar)", 550, 645, 16, BLUE);
	Rectangle renombrar = {270,725,240,45};
	Rectangle eliminar = {550,725,240,45};
	DrawRectangleRec(renombrar, BLUE);
	DrawText("RENOMBRAR", 295, 738, 20, WHITE);
	DrawRectangleRec(eliminar, MAROON);
	DrawText("ELIMINAR", 590, 738, 20, WHITE);


	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();

		if(CheckCollisionPointRec(mouse, volver)){
			pantallaActual = ADMINISTRATIVO;
		}else if(CheckCollisionPointRec(mouse, verArbol)){
			pantallaActual = ABB_CIUDADES;
		}else if(CheckCollisionPointRec(mouse, renombrar) || CheckCollisionPointRec(mouse, eliminar)){
			Ciudad ciudad;
			if(!gestorAdmin.obtenerCiudadPorNombre(camposCiudad[3], ciudad)){
				mensajeCiudad = "No se encontro la ciudad indicada.";
			}else{
				bool cambiar = CheckCollisionPointRec(mouse, renombrar);
				bool realizada = cambiar
					? gestorAdmin.modificarCiudad(ciudad.getId(), camposCiudad[4])
					: gestorAdmin.eliminarCiudad(ciudad.getId());
				if(realizada){
					mensajeCiudad = cambiar ? "Ciudad renombrada." : "Ciudad y sus rutas eliminadas.";
					camposCiudad[3] = "";
					camposCiudad[4] = "";
					ciudadEncontrada = -1;
					mensajeArbol = "";
					resultadoRuta.existe = false;
					rutaMaleta.existe = false;
					verMaleta = false;
				}else{
					mensajeCiudad = cambiar ? "Nombre vacio o repetido." : "No se puede eliminar: ciudad asignada a un vuelo.";
				}
			}
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




void InterfazGrafica::mostrarPantallaTripulacion(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("REGISTRAR TRIPULANTE", 295, 175, 30, BLUE);
	DrawText("Identificacion", 270, 250, 22, BLUE);
	DrawText("Nombre", 550, 250, 22, BLUE);
	DrawText("Cargo", 270, 360, 22, BLUE);
	DrawText("Placa de aeronave", 550, 360, 22, BLUE);
	Rectangle cajas[4] = {{270,285,240,45}, {550,285,240,45}, {270,395,240,45}, {550,395,240,45}};
	editarCampos(camposTripulante, cajas, 4, campoTripulanteActivo);
	DrawText("Cargo: por ejemplo piloto, copiloto o auxiliar.", 270, 455, 18, BLUE);
	DrawText("La aeronave debe estar registrada.", 270, 480, 18, BLUE);
	Rectangle volver = {270,520,200,50};
	Rectangle registrar = {550,520,240,50};
	DrawRectangleLines(270, 520, 200, 50, BLUE);
	DrawText("VOLVER", 315, 535, 25, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 590, 535, 25, WHITE);
	DrawText(mensajeTripulante.c_str(), 270, 590, 16, DARKBLUE);
	string cantidad = "Tripulantes registrados: " + to_string(gestorAdmin.obtenerCantidadTripulantes()) + " / 50";
	DrawText(cantidad.c_str(), 270, 620, 20, BLUE);
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, volver)){
			pantallaActual = ADMINISTRATIVO;
		}else if(CheckCollisionPointRec(mouse, registrar)){
			Tripulante existente;
			if(gestorAdmin.obtenerTripulantePorIdentificacion(camposTripulante[0], existente)){
				mensajeTripulante = "Ya existe un tripulante con esa identificacion.";
			}else if(gestorAdmin.registrarTripulante(camposTripulante[0], camposTripulante[1], camposTripulante[2], camposTripulante[3])){
				mensajeTripulante = "Tripulante registrado y asignado a la aeronave.";
				for(int i = 0; i < 4; i++) camposTripulante[i] = "";
			}else{
				mensajeTripulante = "Revise campos vacios, placa existente o limite de 50.";
			}
		}
	}
}

// El inorden da una columna a cada nodo; el nivel da su fila.
// Estas posiciones solo son para el dibujo del arbol, no para los botones.
void InterfazGrafica::ubicarNodosCiudad(ArbolCiudades* arbol, int indice, int nivel, int& posicion, int x[], int y[]){
	NodoCiudad nodo;
	if(!arbol->obtenerNodo(indice, nodo)) return;
	ubicarNodosCiudad(arbol, nodo.izquierdo, nivel + 1, posicion, x, y);
	x[indice] = 120 + posicion * 140;
	y[indice] = 290 + nivel * 80;
	posicion++;
	ubicarNodosCiudad(arbol, nodo.derecho, nivel + 1, posicion, x, y);
}

void InterfazGrafica::dibujarNodosCiudad(ArbolCiudades* arbol, int indice, int x[], int y[]){
	NodoCiudad nodo;
	if(!arbol->obtenerNodo(indice, nodo)) return;
	int px = x[indice] - moverArbolX;
	int py = y[indice] - moverArbolY;
	if(nodo.izquierdo != -1){
		DrawLine(px, py, x[nodo.izquierdo] - moverArbolX, y[nodo.izquierdo] - moverArbolY, BLUE);
	}
	if(nodo.derecho != -1){
		DrawLine(px, py, x[nodo.derecho] - moverArbolX, y[nodo.derecho] - moverArbolY, BLUE);
	}
	dibujarNodosCiudad(arbol, nodo.izquierdo, x, y);
	dibujarNodosCiudad(arbol, nodo.derecho, x, y);
	Color color = BLUE;
	if(nodo.ciudad.getId() == ciudadEncontrada) color = DARKGREEN;
	DrawRectangle(px - 60, py - 18, 120, 36, color);
	string etiqueta = to_string(nodo.ciudad.getId()) + " " + nodo.ciudad.getNombre();
	if(etiqueta.length() > 16) etiqueta = etiqueta.substr(0, 13) + "...";
	DrawText(etiqueta.c_str(), px - 55, py - 6, 12, WHITE);
}

void InterfazGrafica::mostrarPantallaArbolCiudades(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawRectangle(30, 110, 1140, 610, WHITE);
	DrawText("ABB DE CIUDADES", 390, 50, 32, WHITE);
	Rectangle caja[1] = {{60,130,350,45}};
	editarCampos(campoBuscarCiudad, caja, 1, campoBuscarCiudadActivo);
	Rectangle buscar = {430,130,140,45};
	Rectangle inorden = {60,200,180,40};
	Rectangle preorden = {260,200,180,40};
	Rectangle postorden = {460,200,180,40};
	Rectangle anterior = {840,655,130,40};
	Rectangle siguiente = {990,655,140,40};
	Rectangle volver = {50,760,180,45};
	DrawRectangleRec(buscar, BLUE);
	DrawText("BUSCAR", 448, 142, 22, WHITE);
	DrawRectangleRec(inorden, BLUE);
	DrawText("INORDEN", 85, 210, 20, WHITE);
	DrawRectangleRec(preorden, BLUE);
	DrawText("PREORDEN", 280, 210, 20, WHITE);
	DrawRectangleRec(postorden, BLUE);
	DrawText("POSTORDEN", 475, 210, 20, WHITE);
	DrawRectangleLines(840, 655, 130, 40, BLUE);
	DrawText("ANTERIOR", 850, 667, 18, BLUE);
	DrawRectangleLines(990, 655, 140, 40, BLUE);
	DrawText("SIGUIENTE", 1000, 667, 18, BLUE);
	DrawRectangleLines(50, 760, 180, 45, BLUE);
	DrawText("VOLVER", 80, 773, 22, BLUE);
	DrawText("Flechas: mover dibujo | Inicio: volver al origen", 60, 695, 18, BLUE);
	ArbolCiudades* arbol = gestorAdmin.obtenerArbolCiudades();
	Ciudad recorrido[50];
	int total = arbol->obtenerRecorrido(recorridoArbol, recorrido);
	if(paginaArbol * 8 >= total) paginaArbol = 0;
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, volver)) pantallaActual = CIUDADES;
		else if(CheckCollisionPointRec(mouse, buscar)){
			Ciudad encontrada;
			if(arbol->buscar(campoBuscarCiudad[0], encontrada)){
				ciudadEncontrada = encontrada.getId();
				mensajeArbol = "Encontrada: " + encontrada.getNombre() + " (ID " + to_string(ciudadEncontrada) + ")";
			}else{
				ciudadEncontrada = -1;
				mensajeArbol = "No se encontro esa ciudad.";
			}
		}else if(CheckCollisionPointRec(mouse, inorden)){ recorridoArbol = 0; paginaArbol = 0; }
		else if(CheckCollisionPointRec(mouse, preorden)){ recorridoArbol = 1; paginaArbol = 0; }
		else if(CheckCollisionPointRec(mouse, postorden)){ recorridoArbol = 2; paginaArbol = 0; }
		else if(CheckCollisionPointRec(mouse, anterior) && paginaArbol > 0) paginaArbol--;
		else if(CheckCollisionPointRec(mouse, siguiente) && (paginaArbol + 1) * 8 < total) paginaArbol++;
	}
	// No mover el dibujo mientras se escribe el nombre de una ciudad.
	if(campoBuscarCiudadActivo == -1){
		if(IsKeyPressed(KEY_RIGHT) && moverArbolX < 7000) moverArbolX += 80;
		if(IsKeyPressed(KEY_LEFT) && moverArbolX > 0) moverArbolX -= 80;
		if(IsKeyPressed(KEY_DOWN) && moverArbolY < 4000) moverArbolY += 80;
		if(IsKeyPressed(KEY_UP) && moverArbolY > 0) moverArbolY -= 80;
		if(IsKeyPressed(KEY_HOME)){ moverArbolX = 0; moverArbolY = 0; }
	}
	int x[50], y[50];
	int posicion = 0;
	ubicarNodosCiudad(arbol, arbol->obtenerRaiz(), 0, posicion, x, y);
	BeginScissorMode(50, 250, 750, 430);
	dibujarNodosCiudad(arbol, arbol->obtenerRaiz(), x, y);
	if(total == 0) DrawText("Registre ciudades para formar el arbol.", 70, 300, 20, BLUE);
	EndScissorMode();
	// Volver a obtener el recorrido por si se cambio el boton en este cuadro.
	arbol->obtenerRecorrido(recorridoArbol, recorrido);
	DrawText("RECORRIDO", 840, 250, 22, BLUE);
	const char* nombres[3] = {"Inorden", "Preorden", "Postorden"};
	DrawText(nombres[recorridoArbol], 840, 280, 20, DARKBLUE);
	BeginScissorMode(830, 310, 310, 335);
	for(int i = 0; i < 8 && paginaArbol * 8 + i < total; i++){
		int indice = paginaArbol * 8 + i;
		string texto = to_string(indice + 1) + ". " + recorrido[indice].getNombre();
		DrawText(texto.c_str(), 840, 320 + i * 38, 18, DARKBLUE);
	}
	EndScissorMode();
	BeginScissorMode(590, 125, 560, 60);
	DrawText(mensajeArbol.c_str(), 595, 142, 18, DARKBLUE);
	EndScissorMode();
}

void InterfazGrafica::ubicarNodosPasajero(ArbolPasajeros* arbol, int indice, int nivel, int& posicion, int x[], int y[]){
	NodoPasajero nodo;
	if(!arbol->obtenerNodo(indice, nodo)) return;
	ubicarNodosPasajero(arbol, nodo.izquierdo, nivel + 1, posicion, x, y);
	x[indice] = 120 + posicion * 140;
	y[indice] = 290 + nivel * 80;
	posicion++;
	ubicarNodosPasajero(arbol, nodo.derecho, nivel + 1, posicion, x, y);
}

void InterfazGrafica::dibujarNodosPasajero(ArbolPasajeros* arbol, int indice, int x[], int y[]){
	NodoPasajero nodo;
	if(!arbol->obtenerNodo(indice, nodo)) return;
	int px = x[indice] - moverAVLX;
	int py = y[indice] - moverAVLY;
	if(nodo.izquierdo != -1){
		DrawLine(px, py, x[nodo.izquierdo] - moverAVLX, y[nodo.izquierdo] - moverAVLY, BLUE);
	}
	if(nodo.derecho != -1){
		DrawLine(px, py, x[nodo.derecho] - moverAVLX, y[nodo.derecho] - moverAVLY, BLUE);
	}
	dibujarNodosPasajero(arbol, nodo.izquierdo, x, y);
	dibujarNodosPasajero(arbol, nodo.derecho, x, y);
	Color color = BLUE;
	if(nodo.pasajero.identificacion == pasajeroEncontrado && nodo.pasajero.codigoVuelo == vueloEncontrado) color = DARKGREEN;
	DrawRectangle(px - 60, py - 18, 120, 36, color);
	string etiqueta = nodo.pasajero.identificacion + " / " + nodo.pasajero.codigoVuelo;
	if(etiqueta.length() > 16) etiqueta = etiqueta.substr(0, 13) + "...";
	DrawText(etiqueta.c_str(), px - 55, py - 6, 12, WHITE);
}

void InterfazGrafica::mostrarPantallaArbolPasajeros(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawRectangle(30, 110, 1140, 610, WHITE);
	DrawText("AVL DE PASAJEROS", 390, 50, 32, WHITE);
	DrawText("Identificacion", 60, 110, 16, BLUE);
	DrawText("Codigo de vuelo", 270, 110, 16, BLUE);
	Rectangle caja[2] = {{60,130,190,45}, {270,130,140,45}};
	editarCampos(campoBuscarPasajero, caja, 2, campoBuscarPasajeroActivo);
	Rectangle buscar = {430,130,140,45};
	Rectangle inorden = {60,200,180,40};
	Rectangle preorden = {260,200,180,40};
	Rectangle postorden = {460,200,180,40};
	Rectangle anterior = {840,655,130,40};
	Rectangle siguiente = {990,655,140,40};
	Rectangle volver = {50,760,180,45};
	DrawRectangleRec(buscar, BLUE);
	DrawText("BUSCAR", 448, 142, 22, WHITE);
	DrawRectangleRec(inorden, BLUE);
	DrawText("INORDEN", 85, 210, 20, WHITE);
	DrawRectangleRec(preorden, BLUE);
	DrawText("PREORDEN", 280, 210, 20, WHITE);
	DrawRectangleRec(postorden, BLUE);
	DrawText("POSTORDEN", 475, 210, 20, WHITE);
	DrawRectangleLines(840, 655, 130, 40, BLUE);
	DrawText("ANTERIOR", 850, 667, 18, BLUE);
	DrawRectangleLines(990, 655, 140, 40, BLUE);
	DrawText("SIGUIENTE", 1000, 667, 18, BLUE);
	DrawRectangleLines(50, 760, 180, 45, BLUE);
	DrawText("VOLVER", 80, 773, 22, BLUE);
	DrawText("Flechas: mover dibujo | Inicio: volver al origen", 60, 695, 18, BLUE);
	ArbolPasajeros* arbol = gestorAdmin.obtenerArbolPasajeros();
	Pasajero recorrido[50];
	int total = arbol->obtenerRecorrido(recorridoAVL, recorrido);
	if(paginaAVL * 8 >= total) paginaAVL = 0;
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, volver)) pantallaActual = PASAJEROS;
		else if(CheckCollisionPointRec(mouse, buscar)){
			Pasajero encontrado;
			if(gestorAdmin.buscarPasajero(campoBuscarPasajero[0], campoBuscarPasajero[1], encontrado)){
				pasajeroEncontrado = encontrado.identificacion;
				vueloEncontrado = encontrado.codigoVuelo;
				mensajeAVL = encontrado.nombre + " / vuelo " + encontrado.codigoVuelo;
			}else{
				pasajeroEncontrado = "";
				vueloEncontrado = "";
				mensajeAVL = "No esta asignado a ese vuelo.";
			}
		}else if(CheckCollisionPointRec(mouse, inorden)){ recorridoAVL = 0; paginaAVL = 0; }
		else if(CheckCollisionPointRec(mouse, preorden)){ recorridoAVL = 1; paginaAVL = 0; }
		else if(CheckCollisionPointRec(mouse, postorden)){ recorridoAVL = 2; paginaAVL = 0; }
		else if(CheckCollisionPointRec(mouse, anterior) && paginaAVL > 0) paginaAVL--;
		else if(CheckCollisionPointRec(mouse, siguiente) && (paginaAVL + 1) * 8 < total) paginaAVL++;
	}
	// No mover el dibujo mientras se escribe el nombre de una ciudad.
	if(campoBuscarPasajeroActivo == -1){
		if(IsKeyPressed(KEY_RIGHT) && moverAVLX < 7000) moverAVLX += 80;
		if(IsKeyPressed(KEY_LEFT) && moverAVLX > 0) moverAVLX -= 80;
		if(IsKeyPressed(KEY_DOWN) && moverAVLY < 4000) moverAVLY += 80;
		if(IsKeyPressed(KEY_UP) && moverAVLY > 0) moverAVLY -= 80;
		if(IsKeyPressed(KEY_HOME)){ moverAVLX = 0; moverAVLY = 0; }
	}
	int x[50], y[50];
	int posicion = 0;
	ubicarNodosPasajero(arbol, arbol->obtenerRaiz(), 0, posicion, x, y);
	BeginScissorMode(50, 250, 750, 430);
	dibujarNodosPasajero(arbol, arbol->obtenerRaiz(), x, y);
	if(total == 0) DrawText("Registre pasajeros para formar el AVL.", 70, 300, 20, BLUE);
	EndScissorMode();
	// Volver a obtener el recorrido por si se cambio el boton en este cuadro.
	arbol->obtenerRecorrido(recorridoAVL, recorrido);
	DrawText("RECORRIDO", 840, 250, 22, BLUE);
	const char* nombres[3] = {"Inorden", "Preorden", "Postorden"};
	DrawText(nombres[recorridoAVL], 840, 280, 20, DARKBLUE);
	BeginScissorMode(830, 310, 310, 335);
	for(int i = 0; i < 8 && paginaAVL * 8 + i < total; i++){
		int indice = paginaAVL * 8 + i;
		string texto = to_string(indice + 1) + ". " + recorrido[indice].identificacion + " / " + recorrido[indice].codigoVuelo;
		DrawText(texto.c_str(), 840, 320 + i * 38, 18, DARKBLUE);
	}
	EndScissorMode();
	BeginScissorMode(590, 125, 560, 60);
	DrawText(mensajeAVL.c_str(), 595, 142, 18, DARKBLUE);
	EndScissorMode();
}

void InterfazGrafica::mostrarPantallaPasajeros(){
	DrawTexture(fondoRegistros, 0, 0, WHITE);
	DrawText("REGISTRAR PASAJERO", 310, 175, 30, BLUE);
	DrawText("Identificacion", 270, 245, 22, BLUE);
	DrawText("Nombre", 550, 245, 22, BLUE);
	DrawText("Codigo del vuelo", 270, 350, 22, BLUE);
	Rectangle cajas[3] = {{270,280,240,45}, {550,280,240,45}, {270,385,520,45}};
	editarCampos(camposPasajero, cajas, 3, campoPasajeroActivo);
	DrawText("Primero registre el vuelo. Se verifica el cupo de su aeronave.", 270, 460, 16, BLUE);
	Rectangle volver = {270,510,160,50};
	Rectangle registrar = {450,510,160,50};
	Rectangle verArbol = {630,510,160,50};
	DrawRectangleLines(270, 510, 160, 50, BLUE);
	DrawText("VOLVER", 300, 527, 20, BLUE);
	DrawRectangleRec(registrar, BLUE);
	DrawText("REGISTRAR", 465, 527, 20, WHITE);
	DrawRectangleLines(630, 510, 160, 50, BLUE);
	DrawText("VER AVL", 660, 527, 20, BLUE);
	DrawText(mensajePasajero.c_str(), 270, 580, 15, DARKBLUE);
	string total = "Asignaciones registradas: " + to_string(gestorAdmin.obtenerArbolPasajeros()->obtenerCantidad()) + " / 50";
	DrawText(total.c_str(), 270, 615, 18, BLUE);
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		if(CheckCollisionPointRec(mouse, volver)) pantallaActual = ADMINISTRATIVO;
		else if(CheckCollisionPointRec(mouse, verArbol)) pantallaActual = AVL_PASAJEROS;
		else if(CheckCollisionPointRec(mouse, registrar)){
			Vuelo vuelo;
			Pasajero existente;
			if(!gestorAdmin.obtenerVueloPorCodigo(camposPasajero[2], vuelo)){
				mensajePasajero = "Ese vuelo no esta registrado.";
			}else if(gestorAdmin.buscarPasajero(camposPasajero[0], camposPasajero[2], existente)){
				mensajePasajero = "El pasajero ya esta asignado a ese vuelo.";
			}else if(gestorAdmin.registrarPasajero(camposPasajero[0], camposPasajero[1], camposPasajero[2])){
				mensajePasajero = "Pasajero registrado en el vuelo y en el AVL.";
				for(int i = 0; i < 3; i++) camposPasajero[i] = "";
			}else{
				mensajePasajero = "Revise nombre/ID, cupo, nombre anterior o limite de 50.";
			}
		}
	}
}




// ============================= CLIENTE YULI :P ====================================
// Llena las listas de origen y destino.
// Se llama una vez, al entrar a la pantalla del cliente.

void InterfazGrafica::cargarCombosCliente(){
	limpiarResultadosCliente();
	mensajeCliente = "";

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
	
	// encabezado
	Rectangle encabezado = {0, 0, 1300, 70};
	DrawRectangleRec(encabezado, BLUE);
	DrawText("Seleccion de viaje", 500, 10, 40, WHITE);
	
	Rectangle btnVolver = {10, 10, 150, 50};
	DrawRectangleLinesEx(btnVolver, 2, WHITE);
	DrawText("VOLVER", 25, 25, 25, WHITE);
	
	// mapa con las ciudades, rutas y resultados
	dibujarMapaCliente();
	
	DrawText("DESDE", 700, 120, 20, BLACK);
	DrawText("HASTA", 1000, 120, 20, BLACK);
	
	// si cambio la seleccion, los resultados anteriores ya no sirven
	if(resultadoRuta.existe){
		int primera = resultadoRuta.camino[0];
		int ultima = resultadoRuta.camino[resultadoRuta.largo - 1];
		
		if(primera != comboOrigen.getSeleccionada() || ultima != comboDestino.getSeleccionada()){
			limpiarResultadosCliente();
		}
	}
	
	if(mostrandoDestinos && comboOrigen.getSeleccionada() != origenExplorado){
		limpiarResultadosCliente();
	}
	
	// ---------- debajo del mapa ----------
	Rectangle btnMaleta = {50, 520, 600, 50};
	
	if(resultadoRuta.existe){
		dibujarBotonOpcion(btnMaleta, "VER VIAJE DE LA MALETA", verMaleta);
		
		if(verMaleta){
			if(mismaRuta(resultadoRuta, rutaMaleta)){
				DrawText("La maleta viaja en la misma ruta que usted.", 50, 590, 20, PURPLE);
			}
			else{
				DrawText("La maleta viaja por la ruta mas barata:", 50, 590, 20, PURPLE);
				DrawText(textoRuta(rutaMaleta).c_str(), 50, 620, 20, PURPLE);
				DrawText(TextFormat("Costo: $%.2f   Tiempo: %d min", rutaMaleta.costoTotal, rutaMaleta.duracionTotal), 50, 650, 20, PURPLE);
			}
		}
	}
	
	// detalle del destino escogido en la lista
	if(rutaExplorada.existe){
		DrawText("Ruta al destino seleccionado:", 50, 530, 20, DARKGREEN);
		DrawText(textoRuta(rutaExplorada).c_str(), 50, 560, 20, DARKGREEN);
		DrawText(TextFormat("Escalas: %d   Costo: $%.2f   Tiempo: %d min", rutaExplorada.largo - 2, rutaExplorada.costoTotal, rutaExplorada.duracionTotal), 50, 590, 20, DARKGREEN);
	}
	
	// ---------- panel derecho ----------
	Rectangle btnCosto = {700, 330, 250, 45};
	Rectangle btnTiempo = {1000, 330, 250, 45};
	Rectangle btnBuscar = {700, 400, 270, 55};
	Rectangle btnDestinos = {985, 400, 265, 55};
	
	dibujarBotonOpcion(btnCosto, "MAS BARATA", criterioBusqueda == POR_COSTO);
	dibujarBotonOpcion(btnTiempo, "MAS RAPIDA", criterioBusqueda == POR_TIEMPO);
	
	DrawRectangleRec(btnBuscar, DARKBLUE);
	DrawText("BUSCAR RUTA", btnBuscar.x + 45, btnBuscar.y + 15, 26, WHITE);
	
	DrawRectangleRec(btnDestinos, DARKGREEN);
	DrawText("VER DESTINOS", btnDestinos.x + 35, btnDestinos.y + 15, 26, WHITE);
	
	DrawText(mensajeCliente.c_str(), 700, 470, 20, RED);
	
	dibujarPanelResultado();
	dibujarPanelDestinos();
	
	if(!comboDestino.estaAbierta()){
		comboOrigen.actualizar();
	}
	if(!comboOrigen.estaAbierta()){
		comboDestino.actualizar();
	}
	
	bool hayListaAbierta = comboOrigen.estaAbierta() || comboDestino.estaAbierta();
	
	// ---------- rueda del mouse en la lista de destinos ----------
	if(!hayListaAbierta && mostrandoDestinos){
		float rueda = GetMouseWheelMove();
		
		if(rueda < 0 && scrollearDestinos + FILAS_DESTINOS < cantidadDestinos){
			scrollearDestinos++;
		}
		if(rueda > 0 && scrollearDestinos > 0){
			scrollearDestinos--;
		}
	}
	
	if(!hayListaAbierta && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		Vector2 mouse = GetMousePosition();
		
		if(CheckCollisionPointRec(mouse, btnVolver)){
			pantallaActual = PRINCIPAL;
		}
		else if(CheckCollisionPointRec(mouse, btnCosto)){
			criterioBusqueda = POR_COSTO;
			limpiarResultadosCliente();
		}
		else if(CheckCollisionPointRec(mouse, btnTiempo)){
			criterioBusqueda = POR_TIEMPO;
			limpiarResultadosCliente();
		}
		else if(CheckCollisionPointRec(mouse, btnBuscar)){
			limpiarResultadosCliente();
			buscarRutaCliente();
		}
		else if(CheckCollisionPointRec(mouse, btnDestinos)){
			explorarDestinosCliente();
		}
		else if(resultadoRuta.existe && CheckCollisionPointRec(mouse, btnMaleta)){
			verMaleta = !verMaleta;
		}
		else if(mostrandoDestinos){
			// ver si toco alguna fila de la lista
			for(int p = 0; p < FILAS_DESTINOS; p++){
				int posicionLista = scrollearDestinos + p;
				
				if(posicionLista >= cantidadDestinos){
					break;
				}
				
				if(CheckCollisionPointRec(mouse, filaDestino(p))){
					int ciudadIndice = ordenDestinos[posicionLista];
					
					if(distExplorar[ciudadIndice] != INFINITO){
						gestorCliente.armarCamino(origenExplorado, ciudadIndice, previoExplorar, distExplorar, rutaExplorada);
					}
				}
			}
		}
	}
	
	// las listas van de ultimo para que queden encima de todo
	comboDestino.dibujar();
	comboOrigen.dibujar();
}



// Las coordenadas de la ciudad estan guardadas en un mapa de 600 x 400.
// Esta funcion las pasa a la posicion real dentro del mapa del cliente.
Vector2 InterfazGrafica::posicionEnMapa(Ciudad& ciudad){
	float escalaX = mapaCliente.width / 600.0f;
	float escalaY = mapaCliente.height / 400.0f;

	Vector2 posicion;
	posicion.x = mapaCliente.x + ciudad.getX() * escalaX;
	posicion.y = mapaCliente.y + ciudad.getY() * escalaY;

	return posicion;
}


// Dibuja una linea gris por cada ruta registrada en el grafo
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


// Dibuja cada ciudad como un circulo con su nombre
void InterfazGrafica::dibujarMapaCliente(){
	DrawRectangleRounded(mapaCliente, 0.05, 0, WHITE);
	DrawRectangleRoundedLines(mapaCliente, 0.05, 0, 2, DARKBLUE);
	
	dibujarRutasMapa();
	
	// la maleta va primero y mas gruesa, para que se vea debajo del pasajero
	if(verMaleta){
		dibujarRutaEnMapa(rutaMaleta, PURPLE, 10);
	}
	
	dibujarRutaEnMapa(resultadoRuta, ORANGE, 5);
	dibujarRutaEnMapa(rutaExplorada, DARKGREEN, 5);
	dibujarCiudadesMapa();
}

// Dibuja cada ciudad como un circulo con su nombre
void InterfazGrafica::dibujarCiudadesMapa(){
	Grafo* grafo = gestorAdmin.getGrafo();
	int cantidad = grafo->obtenerCantidadCiudades();
	
	for(int i = 0; i < cantidad; i++){
		Ciudad ciudad;
		grafo->obtenerCiudadPorIndice(i, ciudad);
		
		Vector2 posicion = posicionEnMapa(ciudad);
		
		// color segun si es origen, destino o ninguna
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

	resultadoRuta.existe = false;

	if(origen == -1 || destino == -1){
		mensajeCliente = "Seleccione el origen y el destino.";
		return;
	}

	if(origen == destino){
		mensajeCliente = "El origen y el destino deben ser distintos.";
		return;
	}

	bool encontrada = gestorCliente.buscarMejorRuta(origen, destino, criterioBusqueda, resultadoRuta);

	if(encontrada){
		mensajeCliente = "";
		
		// la maleta SIEMPRE viaja por la ruta mas barata
		gestorCliente.buscarMejorRuta(origen, destino, POR_COSTO, rutaMaleta);
		
	}
	else{
		mensajeCliente = "No hay ninguna ruta disponible entre esas ciudades.";
		rutaMaleta.existe = false;
		
	}
	
	verMaleta = false;
}


// Revisa si dos rutas pasan exactamente por las mismas ciudades
bool InterfazGrafica::mismaRuta(ResultadoRuta& a, ResultadoRuta& b){
	if(a.largo != b.largo){
		return false;
	}
	
	for(int k = 0; k < a.largo; k++){
		if(a.camino[k] != b.camino[k]){
			return false;
		}
	}
	
	return true;
}

// Arma un texto como "San Jose -> Puntarenas -> Liberia"
string InterfazGrafica::textoRuta(ResultadoRuta& ruta){
	Grafo* grafo = gestorAdmin.getGrafo();
	string texto = "";
	
	for(int k = 0; k < ruta.largo; k++){
		Ciudad ciudad;
		grafo->obtenerCiudadPorIndice(ruta.camino[k], ciudad);
		
		texto = texto + ciudad.getNombre();
		
		if(k < ruta.largo - 1){
			texto = texto + " -> ";
		}
	}
	
	return texto;
}






// Muestra el recorrido, las escalas y los totales
void InterfazGrafica::dibujarPanelResultado(){
	if(!resultadoRuta.existe){
		return;
	}

	Grafo* grafo = gestorAdmin.getGrafo();
	int y = 490;

	if(criterioBusqueda == POR_COSTO){
		DrawText("Ruta mas barata:", 700, y, 24, DARKBLUE);
	}
	else{
		DrawText("Ruta mas rapida:", 700, y, 24, DARKBLUE);
	}
	y += 35;

	// cada ciudad del recorrido
	for(int k = 0; k < resultadoRuta.largo; k++){
		Ciudad ciudad;
		grafo->obtenerCiudadPorIndice(resultadoRuta.camino[k], ciudad);

		string linea = to_string(k + 1) + ". " + ciudad.getNombre();
		DrawText(linea.c_str(), 720, y, 20, BLACK);
		y += 25;
	}

	y += 10;

	int escalas = resultadoRuta.largo - 2;
	DrawText(TextFormat("Escalas: %d", escalas), 700, y, 22, BLACK);
	y += 30;

	DrawText(TextFormat("Costo total: $%.2f", resultadoRuta.costoTotal), 700, y, 22, BLACK);
	y += 30;

	DrawText(TextFormat("Tiempo total: %d min", resultadoRuta.duracionTotal), 700, y, 22, BLACK);
}



// Dibuja los tramos de una ruta con el color y grosor indicados
void InterfazGrafica::dibujarRutaEnMapa(ResultadoRuta& ruta, Color color, float grosor){
	if(!ruta.existe){
		return;
	}
	
	Grafo* grafo = gestorAdmin.getGrafo();
	
	for(int k = 0; k < ruta.largo - 1; k++){
		Ciudad desde;
		Ciudad hasta;
		grafo->obtenerCiudadPorIndice(ruta.camino[k], desde);
		grafo->obtenerCiudadPorIndice(ruta.camino[k + 1], hasta);
		
		DrawLineEx(posicionEnMapa(desde), posicionEnMapa(hasta), grosor, color);
	}

}

// limpiar un poquito la pantalla
void InterfazGrafica::limpiarResultadosCliente(){
	resultadoRuta.existe = false;
	rutaMaleta.existe = false;
	verMaleta = false;
	
	mostrandoDestinos = false;
	rutaExplorada.existe = false;
}


// Ejecuta Bellman-Ford desde el origen seleccionado
void InterfazGrafica::explorarDestinosCliente(){
	int origen = comboOrigen.getSeleccionada();
	
	limpiarResultadosCliente();
	
	if(origen == -1){
		mensajeCliente = "Seleccione la ciudad de origen.";
		return;
	}
	
	bool calculado = gestorCliente.bellmanFord(origen, criterioBusqueda, distExplorar, previoExplorar);
	
	if(!calculado){
		// validacion x si acaso ya que no se usara
		mensajeCliente = "No se pudieron calcular los destinos, debido a un origen invalido o numero negativo..";
		return;
	}
	
	cantidadDestinos = gestorCliente.ordenarDestinos(origen, distExplorar, ordenDestinos);
	
	origenExplorado = origen;
	scrollearDestinos = 0;
	mostrandoDestinos = true;
	mensajeCliente = "";
	
	if(cantidadDestinos == 0){
		mensajeCliente = "No hay otras ciudades registradas.";
	}
}


// Rectangulo de cada fila visible de la lista de destinos
Rectangle InterfazGrafica::filaDestino(int posicion){
	Rectangle fila = {700, 550 + posicion * 31.0f, 550, 28};
	return fila;
}


// Lista de destinos ordenada, con su total
void InterfazGrafica::dibujarPanelDestinos(){
	if(!mostrandoDestinos){
		return;
	}
	
	Grafo* grafo = gestorAdmin.getGrafo();
	
	Ciudad origen;
	grafo->obtenerCiudadPorIndice(origenExplorado, origen);
	
	string titulo = "Destinos desde " + origen.getNombre();
	DrawText(titulo.c_str(), 700, 500, 22, DARKBLUE);
	
	if(criterioBusqueda == POR_COSTO){
		DrawText("Ordenados del mas barato al mas caro", 700, 525, 16, GRAY);
	}else{
		DrawText("Ordenados del mas rapido al mas lento", 700, 525, 16, GRAY);
	}
	
	Vector2 mouse = GetMousePosition();
	
	for(int p = 0; p < FILAS_DESTINOS; p++){
		int posicionLista = scrollearDestinos + p;
		
		if(posicionLista >= cantidadDestinos){
			break;
		}
		
		int ciudadIndice = ordenDestinos[posicionLista];
		bool alcanzable = distExplorar[ciudadIndice] != INFINITO;
		
		Rectangle fila = filaDestino(p);
		
		// color de fondo de la fila seleccionada, mouse encima o normal
		Color fondoFila = RAYWHITE;
		
		if(rutaExplorada.existe && rutaExplorada.camino[rutaExplorada.largo - 1] == ciudadIndice){
			fondoFila = SKYBLUE;
		}else if(alcanzable && CheckCollisionPointRec(mouse, fila)){
			fondoFila = LIGHTGRAY;
		}
		
		DrawRectangleRec(fila, fondoFila);
		DrawRectangleLinesEx(fila, 1, LIGHTGRAY);
		
		Ciudad ciudad;
		grafo->obtenerCiudadPorIndice(ciudadIndice, ciudad);
		
		DrawText(ciudad.getNombre().c_str(), fila.x + 10, fila.y + 5, 18, BLACK);
		
		// total segun el criterio
		if(!alcanzable){
			DrawText("Sin ruta", fila.x + 380, fila.y + 5, 18, GRAY);
		}
		else if(criterioBusqueda == POR_COSTO){
			DrawText(TextFormat("$%.2f", distExplorar[ciudadIndice]), fila.x + 380, fila.y + 5, 18, DARKGREEN);
		}else{
			DrawText(TextFormat("%d min", (int)distExplorar[ciudadIndice]), fila.x + 380, fila.y + 5, 18, DARKGREEN);
		}
	}
	
	if(cantidadDestinos > FILAS_DESTINOS){
		DrawText("Use la rueda del mouse para ver mas", 700, 865, 16, GRAY);
	}
}



void InterfazGrafica::mostrarPantallaCreditos(){
	DrawTexture(fondoCreditos, 0, 0, WHITE);
	
	Rectangle btnVolver = {30, 20, 150, 45};
	DrawRectangleRec(btnVolver, WHITE);
	DrawText("VOLVER", 55, 32, 22, BLUE);
	
	if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
		if(CheckCollisionPointRec(GetMousePosition(), btnVolver)){
			pantallaActual = PRINCIPAL;
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
		}else if(pantallaActual == RUTAS){
			mostrarPantallaRutas();
		}else if(pantallaActual == VUELOS){
			mostrarPantallaVuelos();
		}else if(pantallaActual == TRIPULACION){
			mostrarPantallaTripulacion();
		}else if(pantallaActual == ABB_CIUDADES){
			mostrarPantallaArbolCiudades();
		}else if(pantallaActual == PASAJEROS){
			mostrarPantallaPasajeros();
		}else if(pantallaActual == AVL_PASAJEROS){
			mostrarPantallaArbolPasajeros();
		}else if(pantallaActual == CLIENTE){
			mostrarPantallaCliente();
		}else if (pantallaActual == CREDITOS){
			mostrarPantallaCreditos();
		}


		EndDrawing();
	}

	liberarAssets();
	CloseAudioDevice();
	CloseWindow();

}
