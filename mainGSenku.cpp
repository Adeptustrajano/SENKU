#include "GSenku.hpp"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {
    // Verificación de argumentos
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <ficheroTablero> <ficheroMovimientos> <retardo> [ficheroSalida]" << endl;
        cerr << "Ejemplo: " << argv[0] << " tablero.txt movimientos.txt 100 resultado.txt" << endl;
        return 1;
    }

    // Parámetros de entrada
    string ficheroTablero = argv[1];
    string ficheroMovimientos = argv[2];
    int retardo = stoi(argv[3]);
    string ficheroSalida = (argc == 5) ? argv[4] : "resultado.txt";

    // Estructuras principales
    tpTablero tablero;
    tpMovimientosValidos movimientosValidos;
    tpListaMovimientos solucion;

    // Inicialización del tablero
    if (!inicializarTablero(ficheroTablero, tablero)) {
        cerr << "Error al inicializar el tablero desde el fichero: " << ficheroTablero << endl;
        return 1;
    }

    // Inicialización de movimientos válidos
    if (!inicializarMovimientosValidos(ficheroMovimientos, movimientosValidos)) {
        cerr << "Error al inicializar los movimientos válidos desde el fichero: " << ficheroMovimientos << endl;
        return 1;
    }

    // Mostrar el estado inicial del tablero
    cout << "Estado inicial del tablero:" << endl;
    mostrarTablero(tablero);

    // Buscar solución
    cout << "Buscando solución..." << endl;
    int resultado = buscaSolucion(tablero, movimientosValidos, solucion, retardo);

    // Mostrar resultados
    if (resultado == 1) {
        cout << "¡Solución encontrada!" << endl;
    } else {
        cout << "No se encontró solución." << endl;
    }

    // Escribir la solución en el fichero de salida
    escribeListaMovimientos(ficheroSalida, solucion);
    cout << "La solución ha sido escrita en el fichero: " << ficheroSalida << endl;

    return 0;
}