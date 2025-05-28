#include <iostream>
#include <string>
#include <vector> // para vector "listas"


using namespace std;

int filas;
int columnas;

struct Celda
{
    bool paredSuperior;
    bool paredInferior;
    bool paredIzquierda;
    bool paredDerecha;

    // esto es el constructor de la estructura Celda.iniciamos con True/default cada uno de las variables.
    Celda() :   paredSuperior(true), paredInferior(true), 
                paredIzquierda(true), paredDerecha(true) {}
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

            cout << "   ";
        }
        cout << "|" << endl;
    }
    for (int j = 0; j < columnas; ++j) {
        cout << "+---";
    }
    cout << "+" << endl;
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

    
    // print del tablero
    imprimirTablero(laberinto);
    cout << "\n------------------------------------------------" << endl;
    cout << "Laberinto generado con exito! \n" << endl;


    return 0;
}

