// aqui va la logica de generar laberinto y resolver laberinto (DFS y BFS)

#include "Laberinto.hpp"
#include <queue>
#include <iostream>
#include <chrono> 
#include <thread>
#include <random>
#include <algorithm>   

using namespace std;

// se declara un vector de pares de enteros. Cada par representa una dirección.
vector<pair<int, int> > DIRECCIONES = {
        {1, 0},   // abajo
        {-1, 0},  // arriba
        {0, 1},   // derecha
        {0, -1}   // izquierda
};

// esto es el constructor (__init__) de Celda.
Celda::Celda(): 
    paredSuperior(true), paredInferior(true),
    paredIzquierda(true), paredDerecha(true),
    simbolo(' '), celdaVisitada(false), 
    esPortal(false), destinoPortal({ -1, -1 }) {
}

// generar laberinto con DFS recursivo con backtracking (busqueda en profundidad)
void generarLaberinto(Laberinto& laberinto, int fil, int col, mt19937& generador, bool modoSeguro, int filas, int columnas) {
    // se marca la celda como visitada
    laberinto[fil][col].celdaVisitada = true;

    // mezclamos las direcciones usando un generador de números aleatorios con la funcion shuffle.
    shuffle(DIRECCIONES.begin(), DIRECCIONES.end(), generador);

    // el bucle recorre cada elemento en el vector 'direcciones'.
    for (size_t movi = 0; movi < DIRECCIONES.size(); ++movi) {

        // obtenemos la dirección actual y la nombramos "dir".
        const pair<int, int>& dir = DIRECCIONES[movi];

        // Calculamos la nueva posición.
        int nuevaFila = fil + dir.first;
        int nuevaColumna = col + dir.second;

        // verificamos si la celda está dentro del limite de Laberinto y si no ha sido visitada
        if ((nuevaFila >= 0 && nuevaFila < filas) &&
            (nuevaColumna >= 0 && nuevaColumna < columnas) &&
            (!laberinto[nuevaFila][nuevaColumna].celdaVisitada)) {

            // eliminamos las paredes entre las celdas actuales
            if (dir.first == 1) {
                laberinto[fil][col].paredInferior = false;
                laberinto[nuevaFila][nuevaColumna].paredSuperior = false; // empujar hacia abajo
            }
            else if (dir.first == -1) {
                laberinto[fil][col].paredSuperior = false;
                laberinto[nuevaFila][nuevaColumna].paredInferior = false; // empujar hacia arriba
            }
            else if (dir.second == 1) {
                laberinto[fil][col].paredDerecha = false;
                laberinto[nuevaFila][nuevaColumna].paredIzquierda = false; // empujar hacia la derecha
            }
            else if (dir.second == -1) {
                laberinto[fil][col].paredIzquierda = false;
                laberinto[nuevaFila][nuevaColumna].paredDerecha = false; // empujar hacia la izquierda
            }

            // llamamos de vuelta a nuestra funcion recursivamente
            generarLaberinto(laberinto, nuevaFila, nuevaColumna, generador, true, filas, columnas);
        }
    }
    // si modo seguro es false, queremos mas caminos posibles
    if (!modoSeguro) {

        int cantidadExtra = (filas * columnas) / 5; // el 20% de nuestro tablero

        // por toda la cantidad extra de caminos
        for (int numero = 0; numero < cantidadExtra; ++numero) {

            // elejimos una fila y columna random (random.randint(0, filas-1)
            int filaRandom = generador() % filas;
            int columnaRandom = generador() % columnas;

            // obtenemos direccion random y recorremos
            shuffle(DIRECCIONES.begin(), DIRECCIONES.end(), generador);
            for (size_t movi = 0; movi < DIRECCIONES.size(); ++movi) {

                // obtenemos la dirección actual y la nombramos "dir".
                const pair<int, int>& dir = DIRECCIONES[movi];

                // Calculamos la nueva posición.
                int nuevaFila = filaRandom + dir.first;
                int nuevaColumna = columnaRandom + dir.second;

                // verificamos si la celda está dentro del limite de Laberinto y pero no si no ha sido visitada
                if ((nuevaFila >= 0 && nuevaFila < filas) &&
                    (nuevaColumna >= 0 && nuevaColumna < columnas)) {

                    // abri una pared aunque ambas celdas ya hayan sido visitadas
                    if (dir.first == 1 && laberinto[filaRandom][columnaRandom].paredInferior) {
                        laberinto[filaRandom][columnaRandom].paredInferior = false;
                        laberinto[nuevaFila][nuevaColumna].paredSuperior = false; // empujar hacia abajo
                    }
                    else if (dir.first == -1 && laberinto[filaRandom][columnaRandom].paredSuperior) {
                        laberinto[filaRandom][columnaRandom].paredSuperior = false;
                        laberinto[nuevaFila][nuevaColumna].paredInferior = false; // empujar hacia arriba
                    }
                    else if (dir.second == 1 && laberinto[filaRandom][columnaRandom].paredDerecha) {
                        laberinto[filaRandom][columnaRandom].paredDerecha = false;
                        laberinto[nuevaFila][nuevaColumna].paredIzquierda = false; // empujar hacia la derecha
                    }
                    else if (dir.second == -1 && laberinto[filaRandom][columnaRandom].paredIzquierda) {
                        laberinto[filaRandom][columnaRandom].paredIzquierda = false;
                        laberinto[nuevaFila][nuevaColumna].paredDerecha = false; // empujar hacia la izquierda
                    }

                    break; // solo abrí una pared por celda extra
                }
            }
        }
    }
};


// Resolucion de laberinto con BFS con cola (queue) (busqueda en anchura)
void resolverLaberinto(Laberinto& laberinto, int fil, int col, int entradaX, int entradaY) {
    // 2 variables, una que seria un deque que llamamos cola y otra una lista de listas de pares que sera nuestro padre
    queue<pair<int, int>> cola;
    vector<vector<pair<int, int>>> padre(fil, vector<pair<int, int>>(col, { -1, -1 }));

    // guardamos dos variables de entrada y salida
    pair<int, int> entrada = { entradaX, entradaY };
    pair<int, int> salida = { fil - 1, col - 1 };

    // agregamos la entrada a nuestra cosa y la marcamos como visitada
    cola.push(entrada);
    laberinto[entrada.first][entrada.second].celdaVisitada = true;

    // mientras nuestra cola no este vacia
    while (!cola.empty()) {

        // guardamos la primera posicion en una variable, luego lo eliminamos y sacamos su x e y
        pair<int, int> actual = cola.front();
        cola.pop();

        int ejeX = actual.first;
        int ejeY = actual.second;

        // si es la salida, rompemos el buble
        if (make_pair(ejeX, ejeY) == salida) break;

        for (size_t movi = 0; movi < DIRECCIONES.size(); ++movi) {

            // guardamos la direccion y modificamos la fila y columna para tener una nueva ubicacion
            const pair<int, int>& dir = DIRECCIONES[movi];

            int nuevaFila = ejeX + dir.first;
            int nuevaColumna = ejeY + dir.second;

            // verificamos que este dentro del tablero
            if (nuevaFila < 0 || nuevaColumna < 0 || nuevaFila >= fil || nuevaColumna >= col) continue;

            // Verificamos que no haya paredes
            if (dir.first == 1 && (laberinto[ejeX][ejeY].paredInferior || laberinto[nuevaFila][nuevaColumna].paredSuperior)) continue;
            if (dir.first == -1 && (laberinto[ejeX][ejeY].paredSuperior || laberinto[nuevaFila][nuevaColumna].paredInferior)) continue;
            if (dir.second == 1 && (laberinto[ejeX][ejeY].paredDerecha || laberinto[nuevaFila][nuevaColumna].paredIzquierda)) continue;
            if (dir.second == -1 && (laberinto[ejeX][ejeY].paredIzquierda || laberinto[nuevaFila][nuevaColumna].paredDerecha)) continue;

            // Si no fue visitado aún
            if (!laberinto[nuevaFila][nuevaColumna].celdaVisitada) {
                // la marcamos como visitada, guardamos en padre y en cola
                laberinto[nuevaFila][nuevaColumna].celdaVisitada = true;
                padre[nuevaFila][nuevaColumna] = { ejeX, ejeY };

                // Verificamos si es portal
                if (laberinto[nuevaFila][nuevaColumna].esPortal) {
                    pair<int, int> destino = laberinto[nuevaFila][nuevaColumna].destinoPortal;

                    // Marcamos el destino también como visitado
                    laberinto[destino.first][destino.second].celdaVisitada = true;
                    padre[destino.first][destino.second] = { nuevaFila, nuevaColumna };

                    // Desactivamos ambos portales
                    laberinto[nuevaFila][nuevaColumna].esPortal = false;
                    laberinto[destino.first][destino.second].esPortal = false;

                    // Agregamos el destino a la cola en lugar del portal original
                    cola.push(destino);
                }
                else {
                    // Si no es portal, guardamos en cola normal
                    cola.push({ nuevaFila, nuevaColumna });
                }
            }
        }
    }

    // creamos una lista que va a ser el camino y guardamos actual como la salida
    vector<pair<int, int>> camino;
    pair<int, int> actual = salida;

    // mientras no sea la entrada
    while (actual != entrada) {

        // guardamos el actual en camino y mi nuevo actual va a ser su padre
        camino.push_back(actual);
        actual = padre[actual.first][actual.second];

        // si no hay camino
        if (actual == make_pair(-1, -1)) {
            cout << "No hay camino disponible!" << endl;
            return;
        }
    }

    // Añadimos la entrada al final
    camino.push_back(entrada);

    // Lo damos vuelta para recorrer de entrada a salida
    reverse(camino.begin(), camino.end());

    // recorremos cada paso en camino
    for (pair<int, int> pasos : camino) {

        int ejeX = pasos.first;
        int ejeY = pasos.second;

        // si es un espacio vacio, la marcamos como recorrido
        if (laberinto[ejeX][ejeY].simbolo == ' ') {
            laberinto[ejeX][ejeY].simbolo = 'X';
        }

        // mostramos el tablero de ese paso, esperamos 0,05seg, limpiamos la terminal
        imprimirTablero(laberinto, fil, col);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        system("cls");
    }
}