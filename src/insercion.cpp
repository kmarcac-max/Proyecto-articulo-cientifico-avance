#include <iostream>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <chrono>
#include <iomanip>
using namespace std;
using namespace chrono;

void generardatos(vector<int>& a, int patron, int semilla) {
    int n = a.size();
    srand(semilla);

    if (patron == 0) {
        for (int i = 0; i < n; i++)
            a[i] = rand() % 1000001;
    }
    else if (patron == 1) {
        for (int i = 0; i < n; i++)
            a[i] = i;
    }
    else if (patron == 2) {
        for (int i = 0; i < n; i++)
            a[i] = n - i;
    }
    else if (patron == 3) {
        for (int i = 0; i < n; i++)
            a[i] = i;

        int cambios = n * 5 / 100;

        for (int i = 0; i < cambios; i++) {
            int x = rand() % n;
            int y = rand() % n;
            int aux = a[x];
            a[x] = a[y];
            a[y] = aux;
        }
    }
    else {
        for (int i = 0; i < n; i++)
            a[i] = rand() % 15;
    }
}

void insercion(vector<int>& a, long long& comparaciones, long long& desplazamientos) {
    int n = a.size();
    
    for (int i = 1; i < n; i++) {
        int clave = a[i];
        int j = i - 1;

        while (j >= 0) {
            comparaciones++;

            if (a[j] > clave) {
                a[j + 1] = a[j];
                desplazamientos++;
                j--;
            }
            else {
                break;
            }
        }

        a[j + 1] = clave;
    }
}

bool estaordenado(const vector<int>& a) {
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i] < a[i - 1])
            return false;
    }
    return true;
}

int main() {
    ofstream archivo("resultados_insercion.csv");

    if (!archivo.is_open()) {
        cout << "No se pudo crear el archivo CSV." << endl;
        return 1;
    }

    archivo << "algoritmo,n,patron,repeticion,tiempo_ms,"
            << "comparaciones,intercambios,profundidad_recursion,"
            << "semilla,commit_hash\n";

    int tamanos[] = {1000, 5000, 10000};

    string patrones[] = {
        "aleatorio", "ascendente", "descendente",
        "casi_ordenado", "duplicados"
    };

    for (int t = 0; t < 3; t++) {
        int n = tamanos[t];

        for (int p = 0; p < 5; p++) {
            for (int r = 1; r <= 30; r++) {
                int semilla = 42 + n * 7 + p * 100 + r;

                vector<int> a(n);
                generardatos(a, p, semilla);

                long long comparaciones = 0;
                long long desplazamientos = 0;

                steady_clock::time_point inicio = steady_clock::now();

                insercion(a, comparaciones, desplazamientos);

                steady_clock::time_point fin = steady_clock::now();

                double tiempo = duration<double, milli>(fin - inicio).count();

                if (!estaordenado(a)) {
                    cout << "Error: arreglo desordenado." << endl;
                    return 1;
                }

                archivo << "insercion," << n << ","
                        << patrones[p] << "," << r << ","
                        << fixed << setprecision(6 )<< tiempo << ","
                        << comparaciones << ","
                        << desplazamientos << ",0,"
                        << semilla << ",NA\n";

                cout << "Insertion Sort | n=" << n
                     << " | " << patrones[p]
                     << " | repeticion " << r
                     << " | " << tiempo << " ms" << endl;
            }
        }
    }

    archivo.close();

    cout << "\nPruebas terminadas correctamente." << endl;
    cout << "Archivo creado: resultados_insercion.csv" << endl;

    return 0;
}
