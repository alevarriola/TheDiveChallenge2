#pragma once

#include <vector>
#include <string>
#include <utility>
#include <random>

using namespace std;

// creamos una class Celda
struct Celda
{
    // declaramos variables
    bool paredSuperior;
    bool paredInferior;
    bool paredIzquierda;
    bool paredDerecha;
    char simbolo;
    bool celdaVisitada;
    bool esPortal;
    pair<int, int> destinoPortal;

    Celda();
};

// declaramos una valiable laberinto, que sera una lista de listas llenas de celdas
using Laberinto = vector<vector<Celda>>;

// Declaramos todas nuestras funciones
void imprimirTablero(Laberinto& laberinto, int filas, int columnas);
void definirObjetos(Laberinto& laberinto, int filas, int columnas, mt19937& generador);
void reiniciarVisitadas(Laberinto& laberinto, int filas, int columnas);
void jugarLaberinto(Laberinto& laberinto, int fil, int col);
void generarLaberinto(Laberinto& laberinto, int fil, int col, mt19937& generador, bool modoSeguro, int filas, int columnas);
void resolverLaberinto(Laberinto& laberinto, int fil, int col, int entradaX, int entradaY);
