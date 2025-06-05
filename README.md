# 🌀 The Dive Challenge 2 - Generador y Juego de Laberintos en C++

¡Bienvenido a TheDiveChallenge2! 🎮 Este proyecto es un generador y explorador de laberintos desarrollado en C++, como parte de un desafío de programación. Incluye funcionalidades para generar laberintos aleatorios, resolverlos mediante BFS y permitir la interacción del usuario para recorrerlos.

## 🚀 Características

- Generación de laberintos: Crea laberintos aleatorios con múltiples caminos o con un único camino seguro.
- Resolución automática: Implementa el algoritmo de búsqueda en anchura (BFS) para encontrar la salida del laberinto.
- Modo de juego interactivo: Permite al usuario moverse dentro del laberinto utilizando comandos.
- Elementos especiales:
    - Portales (A y B): Transportan al jugador de una ubicación a otra y desaparecen tras su uso.
    - (Próximamente): Trampas y puertas que añaden desafíos adicionales.

## 📁 Estructura del Repositorio

El repositorio contiene varias versiones del juego, organizadas en carpetas:


    - Laberinto v1 a Laberinto vFinal: Evolución del juego con mejoras progresivas.
    - README.md: este archivo


## 📁 Estructura del Proyecto

    Laberinto.sln/
    ├── main.cpp           // Punto de entrada del programa
    ├── juego.cpp          // Funciones relacionadas con la interacción del juego
    ├── logica.cpp         // Lógica de generación y resolución del laberinto
    ├── laberinto.hpp      // Definición de estructuras y declaraciones de funciones

## 🧱 Estructura de Datos

- Celda: Representa cada casilla del laberinto, con información sobre paredes, si ha sido visitada y elementos especiales como simbolos y portales.
- Laberinto: Matriz bidimensional de celdas que conforman el laberinto.

## 🎮 Cómo Jugar

### Con Visual Studio Code 🔵
- Compilación:
Asegúrate de tener un compilador de C++ instalado. Luego, compila el programa con:

        g++ main.cpp juego.cpp logica.cpp -o laberinto

### Con Microsoft Visual Studio 🟣
Asegúrate de estar en el archivo Main.cpp:
- Compilación: Ctrl + F7
- Depuracion: F5
- Ejecucion: Ctrl + F5

### Ejecución 👾
    - Ingresa el número de filas y columnas para el laberinto.
    - Elige el modo de generación: múltiples caminos o un único camino seguro.
    - En el modo de juego "Jugar usuario", utiliza las teclas indicadas para moverte por el laberinto.

## 🧠 Aprendizajes y Desafíos

Durante el desarrollo de este proyecto, se abordaron varios desafíos:
- Manejo de estructuras dinámicas: Uso de vectores para representar el laberinto y sus celdas.
- Implementación de algoritmos de búsqueda: Aplicación del algoritmo BFS para la resolución automática del laberinto y DFS para la generacion del laberinto.
- Interacción con el usuario: Desarrollo de una interfaz de texto para permitir la navegación dentro del laberinto.
- Elementos especiales: Incorporación de portales que modifican la lógica de movimiento y resolución.

## 📄 Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo LICENSE para más detalles.

