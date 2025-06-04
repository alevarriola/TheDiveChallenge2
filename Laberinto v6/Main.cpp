// archivo del main, llamamos a todo

#include "Laberinto.hpp"
#include <iostream>
#include <random>

using namespace std;

int filas;
int columnas;
int valorIngresado;

int main()
{
    // mensaje de bienvenida
    cout << "\nBienvenido al generador de laberintos!" << endl;
    cout << "------------------------------------------------\n\n";

    // el usuario ingresa el numero de filas y columnas
    cout << "Ingresa el numero de filas: ";
    cin >> filas;
    cout << "Ingresa el numero de colummnas: ";
    cin >> columnas;

    cout << "\n------------------------------------------------\n" << endl;

    // iniciamos el laberinto
    Laberinto laberinto(filas, vector<Celda>(columnas));
    definirEntradaSalida(laberinto, filas, columnas);

    
    // print del tablero
    imprimirTablero(laberinto, filas, columnas);
    cout << "\n------------------------------------------------" << endl;
    cout << "Celdas generadas con exito! \n" << endl;


    // consulta al usuario que hacer con el tablero
    cout << "Marque 1 para muchos caminos. \nMarque  2 para generar un solo camino. \nO cualquier otro numero para salir." << endl;
    cin >> valorIngresado;


    // para usar random
    random_device rd;
    mt19937 generador(rd());

    // si el usuario ingresa 1, le imprime el Laberinto con muchos caminos.
    if (valorIngresado == 1) {
        generarLaberinto(laberinto, filas - 1, columnas - 1, generador, false, filas, columnas);
        imprimirTablero(laberinto, filas, columnas);
        cout << "Laberinto con varios caminos a la salida generado con exito!\n";
    }
    // si preciona 2, con un solo camino
    else if (valorIngresado == 2) {
        generarLaberinto(laberinto, filas - 1, columnas - 1, generador, true, filas, columnas);
        imprimirTablero(laberinto, filas, columnas);
        cout << "Laberinto con un camino a la salida generado con exito!\n";
    }
    else {
        cout << "Gracias por probar nuestro tablero!" << endl;
        return 0;
    }

    // consulta al usuario si quiere jugar o resolver
    cout << "Desea resolver el laberinto?\nMarque 1 para resolver.\nMarque 2 para jugar manualmente.\nO cualquier otro numero para salir." << endl;
    cin >> valorIngresado;

    // si presiona 1 resolvemos
    if (valorIngresado == 1) {
        reiniciarVisitadas(laberinto, filas, columnas);
        resolverLaberinto(laberinto, filas, columnas, 0, 0);
        imprimirTablero(laberinto, filas, columnas);
        cout << "Laberinto resuelto!\n";
    }

    // si presiona 2 le dejamos jugar
    else if (valorIngresado == 2) {
        reiniciarVisitadas(laberinto, filas, columnas);
        jugarLaberinto(laberinto, filas, columnas);
        imprimirTablero(laberinto, filas, columnas);
        cout << "Laberinto resuelto!\n";
    }
    else {
        cout << "Gracias por probar nuestro laberinto!" << endl;
    }

    return 0;
}


