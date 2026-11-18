#include "ArbolExpansion.h"
#include <cassert>
#include <cmath>
#include <iostream>

int raizPrueba(int padres[], int i){ while(padres[i] != i) i = padres[i]; return i; }

// Oraculo independiente: enumera todos los subconjuntos en redes pequenas.
double optimo(Grafo& g, Criterio criterio, int& componentes){
    int n = g.obtenerCantidadCiudades();
    ConexionExpansion aristas[10];
    int cantidad = 0, padres[5];
    for(int i = 0; i < n; i++) padres[i] = i;
    componentes = n;
    for(int i = 0; i < n; i++) for(int j = i + 1; j < n; j++){
        double a = g.obtenerPeso(i,j,criterio), b = g.obtenerPeso(j,i,criterio);
        double peso = a < 0 ? b : b < 0 ? a : a < b ? a : b;
        if(peso < 0) continue;
        aristas[cantidad++] = {i,j,peso};
        int ra = raizPrueba(padres,i), rb = raizPrueba(padres,j);
        if(ra != rb){ padres[ra] = rb; componentes--; }
    }
    double mejor = 1e100;
    for(int mascara = 0; mascara < (1 << cantidad); mascara++){
        for(int i = 0; i < n; i++) padres[i] = i;
        bool ciclo = false;
        int elegidas = 0;
        double suma = 0;
        for(int k = 0; k < cantidad; k++) if(mascara & (1 << k)){
            int a = raizPrueba(padres,aristas[k].origen), b = raizPrueba(padres,aristas[k].destino);
            if(a == b){ ciclo = true; break; }
            padres[a] = b; elegidas++; suma += aristas[k].peso;
        }
        if(!ciclo && elegidas == n - componentes && suma < mejor) mejor = suma;
    }
    return mejor;
}

void comprobar(Grafo& g, ArbolExpansion& a){
    ResultadoExpansion resultados[] = {a.prim(g),a.kruskal(g)};
    for(int i = 0; i < 2; i++){
        ResultadoExpansion& r = resultados[i];
        int componentes;
        assert(std::fabs(r.total - optimo(g,i == 0 ? POR_COSTO : POR_TIEMPO,componentes)) < 1e-8);
        assert(r.componentes == componentes);
        assert(r.cantidadConexiones == r.cantidadCiudades - componentes);
        for(int tipo = 0; tipo < 3; tipo++){
            int orden[Grafo::N]; bool vistos[Grafo::N] = {};
            int total = a.obtenerRecorrido(r,tipo,orden);
            assert(total == r.cantidadCiudades);
            for(int j = 0; j < total; j++){
                assert(orden[j] >= 0 && orden[j] < total && !vistos[orden[j]]);
                vistos[orden[j]] = true;
            }
        }
    }
}

int main(){
    ArbolExpansion algoritmo;
    Grafo vacio;
    comprobar(vacio,algoritmo);
    unsigned semilla = 17;
    for(int prueba = 0; prueba < 100; prueba++){
        Grafo g;
        for(int i = 0; i < 5; i++){ Ciudad c(100+i,"Ciudad"+std::to_string(i),0,0); assert(g.agregarCiudad(c)); }
        for(int i = 0; i < 5; i++) for(int j = 0; j < 5; j++){
            semilla = semilla * 1664525u + 1013904223u;
            if(i != j && semilla % 3 == 0){
                double costo = (semilla >> 8) % 12;
                int minutos = 1 + (semilla >> 16) % 20;
                assert(g.agregarRuta(100+i,100+j,costo,minutos));
            }
        }
        comprobar(g,algoritmo);
    }
    Grafo aislado;
    Ciudad c(99,"Unica",0,0); aislado.agregarCiudad(c);
    comprobar(aislado,algoritmo);
    // La conversion a conexiones sin direccion no cambia las rutas originales.
    Grafo dirigida;
    Ciudad uno(10,"Uno",0,0), dos(20,"Dos",0,0);
    dirigida.agregarCiudad(uno); dirigida.agregarCiudad(dos);
    dirigida.agregarRuta(10,20,10,5); dirigida.agregarRuta(20,10,0,20);
    assert(algoritmo.prim(dirigida).total == 0);
    assert(algoritmo.kruskal(dirigida).total == 5);
    assert(dirigida.obtenerCosto(10,20) == 10 && dirigida.obtenerCosto(20,10) == 0);
    Grafo grande;
    for(int i = 0; i < Grafo::N; i++){
        Ciudad ciudad(1000+i,"Nodo"+std::to_string(i),0,0);
        grande.agregarCiudad(ciudad);
    }
    assert(algoritmo.prim(grande).componentes == Grafo::N);
    for(int i = 0; i < Grafo::N; i++) for(int j = i+1; j < Grafo::N; j++)
        grande.agregarRuta(1000+i,1000+j,1,2);
    ResultadoExpansion p = algoritmo.prim(grande), k = algoritmo.kruskal(grande);
    assert(p.cantidadConexiones == 49 && p.total == 49 && p.componentes == 1);
    assert(k.cantidadConexiones == 49 && k.total == 98 && k.componentes == 1);
    for(int tipo = 0; tipo < 3; tipo++){
        int orden[Grafo::N];
        assert(algoritmo.obtenerRecorrido(k,tipo,orden) == Grafo::N);
    }
    std::cout << "OK: Prim y Kruskal frente a oraculo exhaustivo en 100 redes, recorridos y casos limite.\n";
}
