#include <iostream>
#include <string>
#include <vector> // para vector "listas"
#include <random> // para random shuffle

using namespace std;

random_device rd; 
mt19937 generador(rd());
uniform_int_distribution<int> distribucion(0, 1);

int filas;
int columnas;
int valorIngresado;

struct Celda
{
    bool paredSuperior;
    bool paredInferior;
    bool paredIzquierda;
    bool paredDerecha;
    char simbolo;

    // esto es el constructor de la estructura Celda.iniciamos con True/default cada uno de las variables.
    Celda() :   paredSuperior(true), paredInferior(true), 
                paredIzquierda(true), paredDerecha(true), 
                simbolo(' '){}
};

using Laberinto = vector<vector<Celda>>;

void imprimirTablero(Laberinto& laberinto) 
{
    for (int i = 0; i < filas; i++){
        
        for (int j = 0; j < columnas; j++) {
            cout << "+";
            cout << (laberinto[i][j].paredSuperior ? "---" : "   "); // superior

        }
        cout << "+" << endl;

        for (int j = 0; j < columnas; ++j) {

            // si la paredIzquierda es true imprime "|" y si no espacio vacio
            cout << (laberinto[i][j].paredIzquierda ? "|" : " ");

            // colocacion del simbolo en el medio
            char simbolo = laberinto[i][j].simbolo;
            cout << " " << (simbolo != ' ' ? simbolo : ' ') << " ";
        }
        cout << "|" << endl;
    }
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
}

void definirEntradaSalida(Laberinto& laberinto)
{
    laberinto[0][0].simbolo = 'E'; // entrada
    laberinto[filas - 1][columnas - 1].simbolo = 'S'; // salida
}


void paredesRandom(Laberinto& laberinto)
{
    // recorremos todo el laberinto y por cada celda
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            Celda& celda = laberinto[i][j];

            // si no es un borde, valor rand() entre true y false
            celda.paredSuperior = (i == 0) ? true : distribucion(generador);
            celda.paredInferior = (i == filas - 1) ? true : distribucion(generador);
            celda.paredIzquierda = (j == 0) ? true : distribucion(generador);
            celda.paredDerecha = (j == columnas - 1) ? true : distribucion(generador);
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
    cout << "Laberinto generado con exito! \n" << endl;


    // consulta al usuario que hacer con el tablero
    cout << "Le gustaria randomizar las paredes?" << endl;

    cout << "Marque 1 (Si) o marque cualquier otro numero (No)" << endl;
    cin >> valorIngresado;

    // si el usuario ingresa 1, le imprime el Laberinto randomizado.
    if (valorIngresado == 1) {
        paredesRandom(laberinto);
        imprimirTablero(laberinto);
        cout << "Paredes randomizadas con exito!";

    }
    else {
        cout << "Gracias por jugar!";
    }


    return 0;
}

