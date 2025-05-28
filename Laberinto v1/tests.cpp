// Laberinto.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>


std::string saludar(std::string nombre, float altura, int edad)
{
    return "Hello World me llamo " + nombre +
        " tengo " + std::to_string(edad) +
        " anhos de edad y mi estatura es de " + std::to_string(altura); // asi se concatena en return
}


void test() {
    int edad = 31;
    float altura = 1.89f; // obligatorio el f al final
    std::string nombre = "Alejandro";

    std::string mensaje = saludar(nombre, altura, edad);
    std::cout << mensaje << std::endl;


    if (edad > 18)  // if va entre parentesis y abre llave, si se cumple. se realiza lo que hay en la llave
    {
        std::cout << "Soy mayor de edad" << std::endl;
    }
    else if (edad == 18)
    {
        std::cout << "Tengo 18 anhos" << std::endl;
    }
    else
    {
        std::cout << "Soy menor de edad" << std::endl;
    }

    for (int numero = 0; numero < 10; numero++) // buble for, definimos varialbe int, mientras sea menor a cero, sumamos 1 a la variable
    {
        std::cout << "numero: " << numero << std::endl; // asi se concatena en prints
    }
}



// ejecutar compilacion: Ctrl + F7 o menu Compilar > Compilar
// Ejecutar programa: Ctrl + F5 o menú Depurar > Iniciar sin depurar
// Depurar programa: F5 o menú Depurar > Iniciar depuración

//   6. En el futuro, para volver a abrir este proyecto, vaya a Archivo > Abrir > Proyecto y seleccione el archivo .sln