#include "GSenku.hpp"
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <stack>

using namespace std;

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
            char estado;
            archivo >> estado;
            switch (estado) {
                case '-':
                    tablero.matriz[i][j] = NO_USADA;
                    break;
                case 'o':
                    tablero.matriz[i][j] = OCUPADA;
                    break;
                case 'x':
                    tablero.matriz[i][j] = VACIA;
                    break;
                default:
                    cerr << "Error: Estado inválido en el fichero del tablero." << endl;
                    return false;
            }
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
            if (i == 1 && j == 1) continue; // Celda central no se usa
            movimientos.validos[i * 3 + j] = (c == '+');
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
                    cout << "x";
                    break;
                case OCUPADA:
                    cout << "o";
                    break;
            }
        }
        cout << endl;
    }
}

// Función auxiliar para verificar si un movimiento es válido
bool esMovimientoValido(const tpTablero &tablero, const tpMovimientosValidos &movValidos, const tpMovimientoPieza &mov) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    // Asegurarse de que el movimiento está dentro de los límites
    if (mov.destino.x < 0 || mov.destino.x >= tablero.nfils || mov.destino.y < 0 || mov.destino.y >= tablero.ncols) {
        return false;
    }

    // Verificar si las celdas origen, destino y la intermedia cumplen las condiciones
    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    if (tablero.matriz[mov.origen.x][mov.origen.y] == OCUPADA &&
        tablero.matriz[mov.destino.x][mov.destino.y] == VACIA &&
        tablero.matriz[interX][interY] == OCUPADA) {
        return true;
    }

    return false;
}

// Función auxiliar para realizar un movimiento
void realizarMovimiento(tpTablero &tablero, const tpMovimientoPieza &mov) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    tablero.matriz[mov.origen.x][mov.origen.y] = VACIA;
    tablero.matriz[mov.destino.x][mov.destino.y] = OCUPADA;
    tablero.matriz[interX][interY] = VACIA;
}

// Función auxiliar para deshacer un movimiento
void deshacerMovimiento(tpTablero &tablero, const tpMovimientoPieza &mov) {
    int dx = mov.destino.x - mov.origen.x;
    int dy = mov.destino.y - mov.origen.y;

    int interX = mov.origen.x + dx / 2;
    int interY = mov.origen.y + dy / 2;

    tablero.matriz[mov.origen.x][mov.origen.y] = OCUPADA;
    tablero.matriz[mov.destino.x][mov.destino.y] = VACIA;
    tablero.matriz[interX][interY] = OCUPADA;
}

// Implementa buscaSolucion
int buscaSolucion(tpTablero &tablero, const tpMovimientosValidos &movValidos, tpListaMovimientos &solucionParcial, const int retardo) {
    bool hayMovimientos = false;

    for (int i = 0; i < tablero.nfils; ++i) {
        for (int j = 0; j < tablero.ncols; ++j) {
            if (tablero.matriz[i][j] == OCUPADA) {
                for (int dx = -2; dx <= 2; dx += 2) {
                    for (int dy = -2; dy <= 2; dy += 2) {
                        tpMovimientoPieza mov = {{i, j}, {i + dx, j + dy}};
                        if (esMovimientoValido(tablero, movValidos, mov)) {
                            hayMovimientos = true;
                            realizarMovimiento(tablero, mov);
                            solucionParcial.movs[solucionParcial.numMovs++] = mov;

                            if (retardo > 0) {
                                mostrarTablero(tablero);
                                this_thread::sleep_for(chrono::milliseconds(retardo));
                            }

                            if (buscaSolucion(tablero, movValidos, solucionParcial, retardo) == 1) {
                                return 1;
                            }

                            deshacerMovimiento(tablero, mov);
                            solucionParcial.numMovs--;
                        }
                    }
                }
            }
        }
    }

    if (!hayMovimientos) {
        int piezasRestantes = 0;
        for (int i = 0; i < tablero.nfils; ++i) {
            for (int j = 0; j < tablero.ncols; ++j) {
                if (tablero.matriz[i][j] == OCUPADA) {
                    piezasRestantes++;
                }
            }
        }
        return (piezasRestantes == 1) ? 1 : -1;
    }

    return -1;
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
