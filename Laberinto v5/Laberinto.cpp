#include <iostream>
#include <string>
#include <vector> // para vector "listas"
#include <random> // para random shuffle
#include <queue> // para usar queue "deque"
#include <map>  // para usar map "diccionario"

using namespace std;

// para usar random
random_device rd; 
mt19937 generador(rd());

// variables globales
int filas;
int columnas;
int valorIngresado;

// se declara un vector de pares de enteros. Cada par representa una dirección.
vector<pair<int, int> > DIRECCIONES = { 
        {1, 0},   // abajo
        {-1, 0},  // arriba
        {0, 1},   // derecha
        {0, -1}   // izquierda
};

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

    // esto es el constructor (__init__) de Celda.
    Celda() :   paredSuperior(true), paredInferior(true), 
                paredIzquierda(true), paredDerecha(true), 
                simbolo(' '), celdaVisitada(false) {}
};

// declaramos una valiable laberinto, que sera una lista de listas llenas de celdas
using Laberinto = vector<vector<Celda>>;

// funcion para imprimir tablero
void imprimirTablero(Laberinto& laberinto) 
{
    for (int fila = 0; fila < filas; fila++){
        
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

void definirEntradaSalida(Laberinto& laberinto)
{
    laberinto[0][0].simbolo = 'E'; // entrada
    laberinto[filas - 1][columnas - 1].simbolo = 'S'; // salida
}

void reiniciarVisitadas(Laberinto& laberinto, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            laberinto[i][j].celdaVisitada = false;
        }
    }
}

// generar laberinto con backtracking tipo DFS 
void generarLaberinto(Laberinto& laberinto, int fil, int col, mt19937& generador, bool modoSeguro) {
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
            generarLaberinto(laberinto, nuevaFila, nuevaColumna, generador, true);
        }
    }
    // si modo seguro es false, queremos mas caminos posibles
    if (!modoSeguro) {

        int cantidadExtra = (filas * columnas) / 5; // el 20% de nuestro tablero

        // por toda la cantidad extra de caminos
        for (int numero = 0; numero < cantidadExtra; ++numero) {

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


// pasamos como parametro el laberinto, y la cantidad total de filas y columnas
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

        // guardamos la primera posicion en una variable y sacamos su x e y
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
            if (nuevaFila < 0 || nuevaColumna < 0 || nuevaFila >= filas || nuevaColumna >= columnas) continue;

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
                cola.push({ nuevaFila, nuevaColumna });
            }
        }
    }

    // guardamos actual como la salida
    pair<int, int> actual = salida;

    // mientras no sea la entrada
    while (actual != entrada) {

        // guardamos en dos variables sus ejes
        int ejeX = actual.first;
        int ejeY = actual.second;

        // si no es la salida la marcamos como recorrido
        if (laberinto[ejeX][ejeY].simbolo != 'S') {
            laberinto[ejeX][ejeY].simbolo = 'X';
        }

        // actual sera ahora el eje guardado en padre
        actual = padre[ejeX][ejeY];

        // si no hay camino
        if (actual == make_pair(-1, -1)) {
            cout << "No hay camino disponible!" << endl;
            return;
        }
    }
}

void jugarLaberinto(Laberinto& laberinto, int fil, int col) {
    // definimos valores iniciales
    char opcion;
    pair<int, int> jugador = { 0, 0 };

    // el valor entrada cambia a jugador y un diccionario con los controles
    laberinto[0][0].simbolo = 'O';
    map<char, pair<int, int>> controles = {
    {'w', { -1, 0 }},  // arriba
    {'s', {  1, 0 }},  // abajo
    {'a', {  0, -1}},  // izquierda
    {'d', {  0, 1 }}   // derecha
    };

    // buble hasta que se direccione a la salida
    while (true) {
        imprimirTablero(laberinto);
        cout << "Ingresa una direccion (W/A/S/D) o presiona X para salir y resolver el laberinto:\n";
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
        else {
            cout << "Direccion invalida. Usa W, A, S o D.\n";
        }
    }
}

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
    definirEntradaSalida(laberinto);

    
    // print del tablero
    imprimirTablero(laberinto);
    cout << "\n------------------------------------------------" << endl;
    cout << "Celdas generadas con exito! \n" << endl;


    // consulta al usuario que hacer con el tablero
    cout << "Marque 1 para muchos caminos. \nMarque  2 para generar un solo camino. \nO cualquier otro numero para salir." << endl;
    cin >> valorIngresado;

    // si el usuario ingresa 1, le imprime el Laberinto randomizado.
    if (valorIngresado == 1) {
        generarLaberinto(laberinto, filas - 1, columnas - 1, generador, false);
        imprimirTablero(laberinto);
        cout << "Paredes randomizadas con exito! \n";

        // consultamos para resolver laberinto
        cout << "Desea resolver el laberinto? \nMarque  1 para resolver. \nMarque  2 para jugar manualmente. \nO cualquier otro numero para salir. \n";
        cin >> valorIngresado;

        // si marco que si, tratamos de resolver
        if (valorIngresado == 1) {
            reiniciarVisitadas(laberinto, filas, columnas);
            resolverLaberinto(laberinto, filas, columnas, 0, 0);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!\n";
        }
        else if (valorIngresado == 2) {
            reiniciarVisitadas(laberinto, filas, columnas);
            jugarLaberinto(laberinto, filas, columnas);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!\n";
        }
    }
    
    // si ingresa 2, imprimimos el laberinto generado con backtraking
    else if (valorIngresado == 2) {
        generarLaberinto(laberinto, filas-1, columnas-1, generador, true); 
        imprimirTablero(laberinto);
        cout << "Laberinto generado con exito!\n";

        cout << "Desea resolver el laberinto? \nMarque  1 para resolver. \nMarque  2 para jugar manualmente. \nO cualquier otro numero para salir. \n";
        cin >> valorIngresado;

        if (valorIngresado == 1) {
            reiniciarVisitadas(laberinto, filas, columnas);
            resolverLaberinto(laberinto, filas, columnas, 0, 0);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!";
        }
        else if (valorIngresado == 2) {
            reiniciarVisitadas(laberinto, filas, columnas);
            jugarLaberinto(laberinto, filas, columnas);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!\n";
        }
    }

    else {
        cout << "Gracias por probar nuestro tablero!";
    }


    return 0;
}

