#include "InterfazGrafica.h"

void InterfazGrafica::calcularExpansion(){
    Grafo& red = *gestorAdmin.getGrafo();
    resultadoExpansion = expansionPorCosto ? algoritmosExpansion.prim(red) : algoritmosExpansion.kruskal(red);
    paginaExpansion = 0;
    moverExpansionX = 0;
    moverExpansionY = 0;
}

// Un arbol de expansion puede tener mas de dos hijos: no es un ABB ni un AVL.
// Los hijos se ordenan por indice y el padre queda centrado sobre ellos.
void InterfazGrafica::ubicarExpansion(int actual, int padre, bool visitados[], int nivel, int& columna, Vector2 posiciones[]){
    visitados[actual] = true;
    int hijos = 0;
    float suma = 0;
    for(int vecino = 0; vecino < resultadoExpansion.cantidadCiudades; vecino++){
        if(vecino == padre || visitados[vecino]) continue;
        bool conectado = false;
        for(int k = 0; k < resultadoExpansion.cantidadConexiones; k++){
            ConexionExpansion& c = resultadoExpansion.conexiones[k];
            if((c.origen == actual && c.destino == vecino) || (c.destino == actual && c.origen == vecino)) conectado = true;
        }
        if(conectado){
            ubicarExpansion(vecino, actual, visitados, nivel + 1, columna, posiciones);
            suma += posiciones[vecino].x;
            hijos++;
        }
    }
    posiciones[actual].x = hijos ? suma / hijos : 135.0f + columna++ * 155.0f;
    posiciones[actual].y = 295.0f + nivel * 90.0f;
}

void InterfazGrafica::mostrarPantallaExpansion(){
    DrawTexture(fondoRegistros, 0, 0, WHITE);
    DrawRectangle(25, 35, 1250, 820, RAYWHITE);
    DrawText("ARBOLES DE EXPANSION", 55, 60, 30, DARKBLUE);
    DrawText("Conecte las ciudades con el menor peso total.", 55, 103, 18, GRAY);

    Rectangle prim = {55, 145, 285, 55};
    Rectangle kruskal = {355, 145, 315, 55};
    Rectangle actualizar = {690, 145, 180, 55};
    Rectangle volver = {1060, 65, 170, 45};
    DrawRectangleRec(prim, expansionPorCosto ? BLUE : WHITE);
    DrawRectangleLinesEx(prim, 2, BLUE);
    DrawText("PRIM  /  COSTO", 82, 163, 24, expansionPorCosto ? WHITE : BLUE);
    DrawRectangleRec(kruskal, expansionPorCosto ? WHITE : BLUE);
    DrawRectangleLinesEx(kruskal, 2, BLUE);
    DrawText("KRUSKAL  /  TIEMPO", 377, 163, 23, expansionPorCosto ? BLUE : WHITE);
    DrawRectangleLinesEx(actualizar, 2, BLUE);
    DrawText("RECALCULAR", 708, 164, 21, BLUE);
    DrawRectangleLinesEx(volver, 2, BLUE);
    DrawText("VOLVER", 1100, 78, 22, BLUE);

    Vector2 mouse = GetMousePosition();
    Rectangle previo = {900, 742, 140, 38};
    Rectangle siguiente = {1060, 742, 160, 38};
    Rectangle botones[3] = {{900, 350, 150, 38}, {1070, 350, 150, 38}, {900, 400, 320, 38}};
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        if(CheckCollisionPointRec(mouse, volver)){ pantallaActual = ADMINISTRATIVO; return; }
        if(CheckCollisionPointRec(mouse, prim)){ expansionPorCosto = true; calcularExpansion(); }
        else if(CheckCollisionPointRec(mouse, kruskal)){ expansionPorCosto = false; calcularExpansion(); }
        else if(CheckCollisionPointRec(mouse, actualizar)) calcularExpansion();
        for(int i = 0; i < 3; i++){
            if(CheckCollisionPointRec(mouse, botones[i])){ recorridoExpansion = i; paginaExpansion = 0; }
        }
        if(CheckCollisionPointRec(mouse, previo) && paginaExpansion > 0) paginaExpansion--;
        if(CheckCollisionPointRec(mouse, siguiente) && (paginaExpansion + 1) * 7 < resultadoExpansion.cantidadCiudades) paginaExpansion++;
    }
    if(IsKeyPressed(KEY_RIGHT)) moverExpansionX += 100;
    if(IsKeyPressed(KEY_LEFT)) moverExpansionX = moverExpansionX > 100 ? moverExpansionX - 100 : 0;
    if(IsKeyPressed(KEY_DOWN)) moverExpansionY += 90;
    if(IsKeyPressed(KEY_UP)) moverExpansionY = moverExpansionY > 90 ? moverExpansionY - 90 : 0;
    if(IsKeyPressed(KEY_HOME)){ moverExpansionX = 0; moverExpansionY = 0; }
    // Acotar el desplazamiento para no perder el dibujo fuera de la vista.
    float maxX = resultadoExpansion.cantidadCiudades * 155.0f;
    float maxY = resultadoExpansion.cantidadCiudades * 90.0f;
    if(moverExpansionX > maxX) moverExpansionX = maxX;
    if(moverExpansionY > maxY) moverExpansionY = maxY;

    DrawRectangle(55, 235, 800, 490, WHITE);
    DrawRectangleLines(55, 235, 800, 490, LIGHTGRAY);
    DrawRectangle(880, 235, 365, 565, WHITE);
    DrawRectangleLines(880, 235, 365, 565, LIGHTGRAY);
    if(resultadoExpansion.cantidadCiudades == 0){
        DrawText("Registre ciudades y rutas para generar el arbol.", 100, 440, 23, GRAY);
    }else{
        Vector2 posiciones[Grafo::N];
        bool visitados[Grafo::N] = {};
        int columna = 0;
        for(int raiz = 0; raiz < resultadoExpansion.cantidadCiudades; raiz++){
            if(!visitados[raiz]){ ubicarExpansion(raiz, -1, visitados, 0, columna, posiciones); columna++; }
        }
        for(int i = 0; i < resultadoExpansion.cantidadCiudades; i++){
            posiciones[i].x -= moverExpansionX;
            posiciones[i].y -= moverExpansionY;
        }
        BeginScissorMode(56, 236, 798, 488);
        for(int i = 0; i < resultadoExpansion.cantidadConexiones; i++){
            ConexionExpansion& c = resultadoExpansion.conexiones[i];
            Vector2 a = posiciones[c.origen], b = posiciones[c.destino];
            DrawLineEx(a, b, 3, BLUE);
            const char* etiqueta = expansionPorCosto ? TextFormat("$%.2f", c.peso) : TextFormat("%.0f min", c.peso);
            int ancho = MeasureText(etiqueta, 16);
            int x = (int)((a.x + b.x) / 2), y = (int)((a.y + b.y) / 2);
            DrawRectangle(x - ancho / 2 - 4, y - 11, ancho + 8, 22, WHITE);
            DrawText(etiqueta, x - ancho / 2, y - 8, 16, DARKBLUE);
        }
        for(int i = 0; i < resultadoExpansion.cantidadCiudades; i++){
            Ciudad ciudad;
            gestorAdmin.obtenerCiudadPorIndice(i, ciudad);
            Vector2 pos = posiciones[i];
            string nombre = ciudad.getNombre();
            if(nombre.length() > 17) nombre = nombre.substr(0, 14) + "...";
            DrawRectangleRounded({pos.x - 67, pos.y - 22, 134, 44}, 0.2f, 6, BLUE);
            DrawText(nombre.c_str(), (int)pos.x - MeasureText(nombre.c_str(), 15) / 2, (int)pos.y - 7, 15, WHITE);
        }
        EndScissorMode();
    }
    DrawText("Flechas: mover el arbol   |   Inicio: restablecer", 65, 745, 17, GRAY);
    DrawText("Ciudades", 900, 255, 18, GRAY);
    DrawText(TextFormat("%d", resultadoExpansion.cantidadCiudades), 1180, 255, 20, DARKBLUE);
    DrawText("Conexiones", 900, 285, 18, GRAY);
    DrawText(TextFormat("%d", resultadoExpansion.cantidadConexiones), 1180, 285, 20, DARKBLUE);
    DrawText(expansionPorCosto ? TextFormat("Costo total: $%.2f", resultadoExpansion.total)
                               : TextFormat("Duracion total: %.0f min", resultadoExpansion.total), 900, 315, 21, DARKBLUE);
    const char* nombres[] = {"PREORDEN", "POSTORDEN", "POR NIVELES"};
    for(int i = 0; i < 3; i++){
        DrawRectangleRec(botones[i], recorridoExpansion == i ? BLUE : RAYWHITE);
        DrawText(nombres[i], (int)botones[i].x + 15, (int)botones[i].y + 11, 18, recorridoExpansion == i ? WHITE : DARKBLUE);
    }
    int orden[Grafo::N];
    int total = algoritmosExpansion.obtenerRecorrido(resultadoExpansion, recorridoExpansion, orden);
    DrawText("RECORRIDO", 900, 465, 18, GRAY);
    for(int fila = 0; fila < 7 && paginaExpansion * 7 + fila < total; fila++){
        int indice = paginaExpansion * 7 + fila;
        Ciudad ciudad;
        gestorAdmin.obtenerCiudadPorIndice(orden[indice], ciudad);
        string texto = to_string(indice + 1) + ". " + ciudad.getNombre();
        if(texto.length() > 29) texto = texto.substr(0, 26) + "...";
        DrawText(texto.c_str(), 900, 500 + fila * 31, 19, DARKBLUE);
    }
    DrawRectangleLinesEx(previo, 1, BLUE);
    DrawRectangleLinesEx(siguiente, 1, BLUE);
    DrawText("ANTERIOR", 915, 754, 17, BLUE);
    DrawText("SIGUIENTE", 1085, 754, 17, BLUE);
    if(resultadoExpansion.componentes > 1){
        DrawText(TextFormat("Red desconectada: bosque minimo de %d componentes.", resultadoExpansion.componentes), 65, 787, 20, DARKBLUE);
    }else if(resultadoExpansion.cantidadCiudades > 0){
        DrawText("Arbol de expansion minima: todas las ciudades estan conectadas.", 65, 787, 20, DARKGREEN);
    }
    DrawText("Conexion sin direccion: se usa el menor peso disponible entre ida y vuelta.", 65, 825, 17, GRAY);
}
