#include "GestorAdministrativo.h"
#include <fstream>
#include <sstream>
#include <cctype>

namespace {
string quitarEspacios(string texto){
    size_t inicio = texto.find_first_not_of(" \t\r\n");
    if(inicio == string::npos) return "";
    size_t fin = texto.find_last_not_of(" \t\r\n");
    return texto.substr(inicio, fin - inicio + 1);
}

int separarCampos(string linea, string campos[]){
    int cantidad = 0;
    size_t inicio = 0;
    while(true){
        if(cantidad == 6) return -1;
        size_t fin = linea.find(';', inicio);
        campos[cantidad++] = quitarEspacios(linea.substr(inicio, fin == string::npos ? fin : fin - inicio));
        if(fin == string::npos) return cantidad;
        inicio = fin + 1;
    }
}

template<class Numero>
bool leerNumero(string texto, Numero& valor){
    istringstream entrada(texto);
    if(!(entrada >> valor)) return false;
    entrada >> ws;
    return entrada.eof();
}

bool cargarRegistro(GestorAdministrativo& datos, string campos[], int cantidad, string& error){
    string tipo = campos[0];
    for(size_t i = 0; i < tipo.length(); i++) tipo[i] = (char)toupper((unsigned char)tipo[i]);
    int esperados = 0;
    if(tipo == "CIUDAD" || tipo == "PASAJERO") esperados = 4;
    else if(tipo == "RUTA" || tipo == "AERONAVE" || tipo == "VUELO" || tipo == "TRIPULANTE") esperados = 5;
    else{ error = "Tipo desconocido: " + tipo; return false; }
    if(cantidad != esperados){ error = tipo + ": cantidad de campos incorrecta."; return false; }
    for(int i = 1; i < cantidad; i++){
        if(campos[i].empty()){ error = tipo + ": hay un campo vacio."; return false; }
        if(campos[i].length() > 48){ error = tipo + ": un campo supera 48 caracteres."; return false; }
    }
    bool registrado = false;
    if(tipo == "CIUDAD"){
        float x, y;
        if(!leerNumero(campos[2], x) || !leerNumero(campos[3], y)){
            error = "CIUDAD: X e Y deben ser numeros."; return false;
        }
        registrado = datos.registrarCiudad(campos[1], x, y);
        error = "CIUDAD: nombre repetido, coordenadas fuera del mapa o limite de 50.";
    }else if(tipo == "AERONAVE"){
        int capacidad;
        if(!leerNumero(campos[2], capacidad) || capacidad <= 0){
            error = "AERONAVE: capacidad debe ser un entero positivo."; return false;
        }
        registrado = datos.registrarAeronave(campos[1], capacidad, campos[3], campos[4]);
        error = "AERONAVE: placa repetida o limite de 50.";
    }else if(tipo == "RUTA" || tipo == "VUELO"){
        Ciudad origen, destino;
        int posOrigen = tipo == "RUTA" ? 1 : 3;
        int posDestino = tipo == "RUTA" ? 2 : 4;
        if(!datos.obtenerCiudadPorNombre(campos[posOrigen], origen) || !datos.obtenerCiudadPorNombre(campos[posDestino], destino)){
            error = tipo + ": registre primero las ciudades de origen y destino."; return false;
        }
        if(tipo == "RUTA"){
            double costo; int minutos;
            if(!leerNumero(campos[3], costo) || !leerNumero(campos[4], minutos)){
                error = "RUTA: costo numerico y minutos enteros requeridos."; return false;
            }
            registrado = datos.registrarRuta(origen.getId(), destino.getId(), costo, minutos);
            error = "RUTA: repetida, origen igual a destino, costo negativo o minutos no positivos.";
        }else{
            registrado = datos.registrarVuelo(campos[1], campos[2], origen.getId(), destino.getId());
            error = "VUELO: codigo repetido, aeronave o ruta inexistente, o limite de 50.";
        }
    }else if(tipo == "TRIPULANTE"){
        registrado = datos.registrarTripulante(campos[1], campos[2], campos[3], campos[4]);
        error = "TRIPULANTE: identificacion repetida, aeronave inexistente o limite de 50.";
    }else if(tipo == "PASAJERO"){
        registrado = datos.registrarPasajero(campos[1], campos[2], campos[3]);
        error = "PASAJERO: revise vuelo, cupo, duplicados, nombre previo o limite de 50.";
    }
    return registrado;
}
}

bool GestorAdministrativo::cargarArchivo(string ruta, int& registros, string& mensaje){
    registros = 0;
    mensaje = "";
    ifstream archivo(ruta.c_str());
    if(!archivo){ mensaje = "No se pudo abrir el archivo. Revise la ruta."; return false; }
    // Los modelos y arboles usan arreglos: esta copia conserva los datos actuales.
    // Solo se aplica si todas las lineas se registran correctamente.
    GestorAdministrativo copia = *this;
    string linea;
    int numeroLinea = 0, cargados = 0;
    while(getline(archivo, linea)){
        numeroLinea++;
        // Bloc de notas puede guardar UTF-8 con una marca al inicio (BOM).
        if(numeroLinea == 1 && linea.compare(0, 3, "\xEF\xBB\xBF") == 0) linea.erase(0, 3);
        linea = quitarEspacios(linea);
        if(linea.empty() || linea[0] == '#') continue;
        if(linea.length() > 512){ mensaje = "Linea " + to_string(numeroLinea) + ": demasiado larga."; return false; }
        string campos[6], error;
        int cantidad = separarCampos(linea, campos);
        if(cantidad < 0 || !cargarRegistro(copia, campos, cantidad, error)){
            mensaje = "Linea " + to_string(numeroLinea) + ": " + (cantidad < 0 ? "Demasiados campos." : error);
            return false;
        }
        cargados++;
    }
    if(archivo.bad()){ mensaje = "No se pudo terminar de leer el archivo."; return false; }
    if(cargados == 0){ mensaje = "El archivo no contiene registros."; return false; }
    *this = copia;
    registros = cargados;
    mensaje = "Carga completa: " + to_string(registros) + " registros agregados.";
    return true;
}
