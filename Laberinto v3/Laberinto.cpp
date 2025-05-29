#include <iostream>
#include <string>
#include <vector> // para vector "listas"
#include <random> // para random shuffle

using namespace std;

// para usar random
random_device rd; 
mt19937 generador(rd());
uniform_int_distribution<int> distribucion(0, 1);

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

// definimos la funcion pasando como parametro la cantidad de filas de colummnas y los referenciamos respectivamente.
void generarLaberinto(Laberinto& laberinto, int fil, int col, mt19937& generador) {
    // se marca la celda en la posición [fil][col] como visitada.
    laberinto[fil][col].celdaVisitada = true;
    
    // mezclamos las direcciones usando un generador de números aleatorios con la funcion shuffle.
    shuffle(DIRECCIONES.begin(), DIRECCIONES.end(), generador);
    
    // el bucle recorre cada elemento en el vector 'direcciones'.
    for (size_t movi = 0; movi < DIRECCIONES.size(); ++movi) {
        
        // obtenemos la dirección actual y referenciamos "dir".
        const pair<int, int>& dir = DIRECCIONES[movi];
        
        // Calculamos la nueva posición.
        int nuevaFila = fil + dir.first;
        int nuevaColumna = col + dir.second;
        
        // verificamos si la celda está dentro del limite de Laberinto y si no ha sido visitada, por defecto nos devuelve falso, y con el operador logico le decimos true
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
            generarLaberinto(laberinto, nuevaFila, nuevaColumna, generador);
        }
    }
};


void paredesRandom(Laberinto& laberinto)
{
    // recorremos todo el laberinto y por cada celda
    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas; ++columna) {
            Celda& celda = laberinto[fila][columna];

            if (fila == 0) {
                celda.paredSuperior = true;  // Esta en el borde superior
            }
            else if (laberinto[fila - 1][columna].paredInferior == true) {
                celda.paredSuperior = true;  // La celda de arriba tiene su pared inferior activada
            }
            else {
                celda.paredSuperior = distribucion(generador); // Random
            }

            // para la pared inferior
            if (fila == filas - 1) {
                celda.paredInferior = true;
            }
            else if (laberinto[fila + 1][columna].paredSuperior == true) {
                celda.paredInferior = true;
            }
            else {
                celda.paredInferior = distribucion(generador);
            }

            // para la pared izquierda
            if (columna == 0) {
                celda.paredIzquierda = true;
            }
            else if (laberinto[fila][columna - 1].paredDerecha == true) {
                celda.paredIzquierda = true;
            }
            else {
                celda.paredIzquierda = distribucion(generador);
            }
            
            // para la pared derecha
            if (columna == columnas - 1) {
                celda.paredDerecha = true;
            }
            else if (laberinto[fila][columna + 1].paredIzquierda == true) {
                celda.paredDerecha = true;
            }
            else {
                celda.paredDerecha = distribucion(generador);
            }
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
    cout << "Vamos a jugar un poco!" << endl;

    cout << "Marque 1 para randomizar paredes. \nMarque  2 para generar laberinto. \nO cualquier otro numero para salir." << endl;
    cin >> valorIngresado;

    // si el usuario ingresa 1, le imprime el Laberinto randomizado.
    if (valorIngresado == 1) {
        paredesRandom(laberinto);
        imprimirTablero(laberinto);
        cout << "Paredes randomizadas con exito!";

    }

    else if (valorIngresado == 2) {
        generarLaberinto(laberinto, 0, 0, generador); // 0, 0 porque es la ubicacion de mi entrada siempre
        imprimirTablero(laberinto);
        cout << "Laberinto generado con exito!";

    }

    else {
        cout << "Gracias por probar nuestro tablero!";
    }


    return 0;
}

