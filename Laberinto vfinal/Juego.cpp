// aqui va todo lo que seria para el juego, el imprimir, el movimiento de usuario, etc

#include "Laberinto.hpp"
#include <iostream>
#include <string>
#include <map>
#include <random>
#include <algorithm> 
#include <thread>     
#include <chrono>

using namespace std;


void imprimirTablero(Laberinto& laberinto, int filas, int columnas)
{
    for (int fila = 0; fila < filas; fila++) {

        for (int columna = 0; columna < columnas; columna++) {
            cout << "+";
            cout << (laberinto[fila][columna].paredSuperior ? "---" : "   "); // verificamos superior

        }
        cout << "+" << endl; // en la esquina derecha

        for (int columna = 0; columna < columnas; ++columna) {

            // si la paredIzquierda es true imprime "|" y si no espacio vacio
            cout << (laberinto[fila][columna].paredIzquierda ? "|" : " ");

            // colocacion del simbolo en el medio
            char simbolo = laberinto[fila][columna].simbolo;
            cout << " " << (simbolo != ' ' ? simbolo : ' ') << " ";
        }
        cout << "|" << endl;
    }
    for (int columna = 0; columna < columnas; ++columna) {
        cout << "+---";
    }
    cout << "+" << endl;
}

void definirObjetos(Laberinto& laberinto, int filas, int columnas, mt19937& generador)
{
    laberinto[0][0].simbolo = 'E'; // entrada
    laberinto[filas - 1][columnas - 1].simbolo = 'S'; // salida
    
    // definimos donde estara los portales
    int filaA, colA, filaB, colB;
    
    // definimos posiciones random mientras que no sea entrada ni salida
    do {
        filaA = generador() % filas;
        colA = generador() % columnas;
    } while ((filaA == 0 && colA == 0) || (filaA == filas - 1 && colA == columnas - 1));
    

    // Asegurarse de que no esten en el mismo lugar ni entradas y salidas
    do {
        filaB = generador() % filas;
        colB = generador() % columnas;
    } while ((filaB == 0 && colB == 0) ||
            (filaB == filas - 1 && colB == columnas - 1) ||
            (filaB == filaA && colB == colA));

    // actualizamos nuestro laberinto con ambos portales
    laberinto[filaA][colA].simbolo = 'A';
    laberinto[filaA][colA].esPortal = true;
    laberinto[filaA][colA].destinoPortal = { filaB, colB };

    laberinto[filaB][colB].simbolo = 'B';
    laberinto[filaB][colB].esPortal = true;
    laberinto[filaB][colB].destinoPortal = { filaA, colA };
}

void reiniciarVisitadas(Laberinto& laberinto, int filas, int columnas) {
    
    // recorremos todo el laberinto y todas las celdas las marcamos como false
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            laberinto[i][j].celdaVisitada = false;
        }
    }
}

void jugarLaberinto(Laberinto& laberinto, int fil, int col) {
    // definimos valores iniciales
    char opcion;
    pair<int, int> jugador = { 0, 0 };

    // el simbolo entrada cambia a jugador y un diccionario con los controles
    laberinto[0][0].simbolo = 'O';
    map<char, pair<int, int>> controles = {
    {'w', { -1, 0 }},  // arriba
    {'s', {  1, 0 }},  // abajo
    {'a', {  0, -1}},  // izquierda
    {'d', {  0, 1 }}   // derecha
    };

    // buble hasta que se direccione a la salida o resuleva
    while (true) {

        // limpiamos la terminal
        system("cls");

        // mostramos el tablero y consultamos que hacer al usuario
        imprimirTablero(laberinto, fil, col);
        cout << "Ingresa una direccion (W/A/S/D) o presiona (X) para salir y resolver el laberinto:\n";
        cin >> opcion;

        // si existe esa key en opciones
        if (controles.count(opcion)) {
            // recumeramos esa direccion y modificamos posicion
            pair<int, int> dir = controles[opcion];
            int dirX = dir.first;
            int dirY = dir.second;

            // posicion a probar
            int nuevaPosicionX = jugador.first + dirX;
            int nuevaPosicionY = jugador.second + dirY;

            // verificamos si puede moverse
            if (dir.first == 1 && (laberinto[jugador.first][jugador.second].paredInferior || laberinto[nuevaPosicionX][nuevaPosicionY].paredSuperior)) continue;
            if (dir.first == -1 && (laberinto[jugador.first][jugador.second].paredSuperior || laberinto[nuevaPosicionX][nuevaPosicionY].paredInferior)) continue;
            if (dir.second == 1 && (laberinto[jugador.first][jugador.second].paredDerecha || laberinto[nuevaPosicionX][nuevaPosicionY].paredIzquierda)) continue;
            if (dir.second == -1 && (laberinto[jugador.first][jugador.second].paredIzquierda || laberinto[nuevaPosicionX][nuevaPosicionY].paredDerecha)) continue;

            // quitamos el símbolo de la posición actual
            laberinto[jugador.first][jugador.second].simbolo = ' ';

            // actualizamos la posición del jugador
            jugador.first = nuevaPosicionX;
            jugador.second = nuevaPosicionY;

            // verificamos si esta en un portal
            if (laberinto[jugador.first][jugador.second].esPortal) {
                
                cout << "¡Has entrado a un portal!\n";
                pair<int, int> destino = laberinto[jugador.first][jugador.second].destinoPortal;

                // desactivamos ambos portales
                laberinto[jugador.first][jugador.second].esPortal = false;
                laberinto[destino.first][destino.second].esPortal = false;

                // quitamos los símbolos visuales
                laberinto[jugador.first][jugador.second].simbolo = ' ';
                laberinto[destino.first][destino.second].simbolo = ' ';

                // movemos al jugador
                jugador = destino;
            }

            // marcamos la nueva posición con 'O'
            laberinto[jugador.first][jugador.second].simbolo = 'O';

            // si esta en el final
            if (jugador.first == fil - 1 && jugador.second == col - 1) {
                cout << "¡Felicidades! Llegaste a la salida.\n";
                break; // salís del bucle
            }

        }
        else if (opcion == 'x') {
            resolverLaberinto(laberinto, fil, col, jugador.first, jugador.second);
            cout << "Juego finalizado.\n";
            break;
        }
    }
}