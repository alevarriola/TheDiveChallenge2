#include <iostream>
#include <string>
#include <vector> // para vector "listas"
#include <random> // para random shuffle
#include <queue> // para usar queue "deque"

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

void reiniciarVisitadas(Laberinto& laberinto, int filas, int columnas) {
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            laberinto[i][j].celdaVisitada = false;
        }
    }
}

// pasamos como parametro donde iniciar en filas, colummnas, el laberinto y el generador
void generarLaberinto(Laberinto& laberinto, int fil, int col, mt19937& generador) {
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
            laberinto[fila][columna].celdaVisitada = true;

            if (fila == 0) {
                celda.paredSuperior = true;  // Esta en el borde superior
            }
            else if (laberinto[fila - 1][columna].paredInferior == true
                && laberinto[fila - 1][columna].celdaVisitada == true) {
                celda.paredSuperior = true;  // La celda de arriba tiene su pared inferior activada randomizada
            }
            else if (laberinto[fila - 1][columna].paredInferior == false) {
                celda.paredSuperior = false; // ya se randomizo esa pared a false
            }
            else {
                celda.paredSuperior = distribucion(generador); // Random
            }

            // para la pared inferior
            if (fila == filas - 1) {
                celda.paredInferior = true;
            }
            else if (laberinto[fila + 1][columna].paredSuperior == true 
                && laberinto[fila + 1][columna].celdaVisitada == true) {
                celda.paredInferior = true;
            }
            else if (laberinto[fila + 1][columna].paredSuperior == false) {
                celda.paredInferior = false;
            }
            else {
                celda.paredInferior = distribucion(generador);
            }

            // para la pared izquierda
            if (columna == 0) {
                celda.paredIzquierda = true;
            }
            else if (laberinto[fila][columna - 1].paredDerecha == true
                && laberinto[fila][columna - 1].celdaVisitada == true) {
                celda.paredIzquierda = true;
            }
            else if (laberinto[fila][columna - 1].paredDerecha == false) {
                celda.paredIzquierda = false; 
            }
            else {
                celda.paredIzquierda = distribucion(generador);
            }
            
            // para la pared derecha
            if (columna == columnas - 1) {
                celda.paredDerecha = true;
            }
            else if (laberinto[fila][columna + 1].paredIzquierda == true
                && laberinto[fila][columna + 1].celdaVisitada == true) {
                celda.paredDerecha = true;
            }
            else if (laberinto[fila][columna + 1].paredIzquierda == false) {
                celda.paredDerecha = false; 
            }
            else {
                celda.paredDerecha = distribucion(generador);
            }
        }
    }
}

// pasamos como parametro el laberinto, y la cantidad total de filas y columnas
void resolverLaberinto(Laberinto& laberinto, int fil, int col) {
    // 2 variables, una que seria un deque que llamamos cola y otra una lista de listas de pares que sera nuestro padre
    queue<pair<int, int>> cola;
    vector<vector<pair<int, int>>> padre(fil, vector<pair<int, int>>(col, { -1, -1 }));

    // guardamos dos variables de entrada y salida
    pair<int, int> entrada = { 0, 0 };
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
        cout << "Paredes randomizadas con exito! \n";

        // consultamos para resolver laberinto
        cout << "Desea resolver el laberinto? \nMarque  1 para resolver. \nO cualquier otro numero para salir. \n";
        cin >> valorIngresado;

        // si marco que si, tratamos de resolver
        if (valorIngresado == 1) {
            reiniciarVisitadas(laberinto, filas, columnas);
            resolverLaberinto(laberinto, filas, columnas);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!\n";
        }
    }
    
    // si ingresa 2, imprimimos el laberinto generado con backtraking
    else if (valorIngresado == 2) {
        generarLaberinto(laberinto, filas-1, columnas-1, generador); // que el laberinto inicie al final. asi solo hay 1 solo camino
        imprimirTablero(laberinto);
        cout << "Laberinto generado con exito!\n";

        cout << "Desea resolver el laberinto? \nMarque  1 para resolver. \nO cualquier otro numero para salir. \n";
        cin >> valorIngresado;

        if (valorIngresado == 1) {
            reiniciarVisitadas(laberinto, filas, columnas);
            resolverLaberinto(laberinto, filas, columnas);
            imprimirTablero(laberinto);
            cout << "Laberinto resuelto!";
        }
    }

    else {
        cout << "Gracias por probar nuestro tablero!";
    }


    return 0;
}

