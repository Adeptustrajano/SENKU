#include "GSenku.hpp"
#include <thread>
#include <chrono>
#include <sstream>

// Implementa inicializarTablero
bool inicializarTablero(const string nombreFichero, tpTablero &tablero) {
    ifstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero del tablero." << endl;
        return false;
    }

    archivo >> tablero.nfils >> tablero.ncols;
    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            int estado;
            archivo >> estado;
            tablero.matriz[i][j] = static_cast<tpEstadoCelda>(estado);
        }
    }

    archivo.close();
    return true;
}

// Implementa inicializarMovimientosValidos
bool inicializarMovimientosValidos(const string nombreFichero, tpMovimientosValidos &movimientos) {
    ifstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero de movimientos válidos." << endl;
        return false;
    }

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            char c;
            archivo >> c;
            if (i == 1 || j == 1) { // Lógica para convertir en direcciones válidas
                movimientos.validos[i * 3 + j] = (c == '+');
            }
        }
    }

    archivo.close();
    return true;
}

// Implementa mostrarTablero
void mostrarTablero(const tpTablero &tablero) {
    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            switch (tablero.matriz[i][j]) {
                case NO_USADA:
                    cout << " ";
                    break;
                case VACIA:
                    cout << ".";
                    break;
                case OCUPADA:
                    cout << "O";
                    break;
            }
        }
        cout << endl;
    }
}

// Implementa buscaSolucion
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo) {
    // Implementar lógica de búsqueda recursiva de soluciones
    // Esta función será compleja y debe manejar el backtracking
    // Se deja como ejercicio completar la lógica específica del juego
    return -1; // Retorna -1 por defecto si no se encuentra solución
}

// Implementa escribeListaMovimientos
void escribeListaMovimientos(string nombreFichero, const tpListaMovimientos &solucion) {
    ofstream archivo(nombreFichero);
    if (!archivo.is_open()) {
        cerr << "Error: No se pudo abrir el fichero de salida." << endl;
        return;
    }

    if (solucion.numMovs == 0) {
        archivo << "-1" << endl;
    } else {
        for (int i = 0; i < solucion.numMovs; ++i) {
            archivo << solucion.movs[i].origen.x << " " << solucion.movs[i].origen.y << " "
                    << solucion.movs[i].destino.x << " " << solucion.movs[i].destino.y << endl;
        }
    }

    archivo.close();
}