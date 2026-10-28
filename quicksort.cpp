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
int particion(vector<int>& a, int inicio, int fin, long long& comparaciones, long long& intercambios) {
    
	int pivote = a[inicio + (fin - inicio) / 2];
    int i = inicio;
    int j = fin;

    while (i <= j) {
        while (i <= fin) {
            comparaciones++;
            if (a[i] < pivote)
                i++;
            else
                break;
        }
        while (j >= inicio) {
            comparaciones++;
            if (a[j] > pivote)
                j--;
            else
                break;
        }
        if (i <= j) {
            if (i != j) {
                int aux = a[i];
                a[i] = a[j];
                a[j] = aux;
                intercambios++;
            }
            i++;
            j--;
        }
    }
    return i;
}
void quicksort(vector<int>& a, int inicio, int fin, long long& comparaciones, long long& intercambios, int profundidad, int& profundidadmaxima) {
               	
    if (profundidad > profundidadmaxima)
        profundidadmaxima = profundidad;
        
    if (inicio >= fin)
        return;
    int indice = particion(a, inicio, fin, comparaciones, intercambios);
    
    if (inicio < indice - 1) {
        quicksort(a, inicio, indice - 1,comparaciones, intercambios, profundidad + 1, profundidadmaxima);
    }
    
    if (indice < fin) { 
        quicksort(a, indice, fin, comparaciones, intercambios, profundidad + 1, profundidadmaxima);
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
    ofstream archivo("resultados_quicksort.csv");
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
                long long intercambios = 0;
                int profundidadmaxima = 0;

                steady_clock::time_point inicio =
                    steady_clock::now();
                quicksort(a, 0, n - 1, comparaciones, intercambios,1, profundidadmaxima);
                steady_clock::time_point fin = steady_clock::now();
                double tiempo = duration<double, milli>(fin - inicio).count();

                if (!estaordenado(a)) {
                    cout << "Error: arreglo desordenado." << endl;
                    return 1;
                }
                archivo << "quicksort," << n << ","
                        << patrones[p] << "," << r << ","
                        << fixed << setprecision(6) << tiempo << ","
                        << comparaciones << ","
                        << intercambios << ","
                        << profundidadmaxima << ","
                        << semilla << ",NA\n";
                cout << "Quick Sort | n=" << n
                     << " | " << patrones[p]
                     << " | repeticion " << r
                     << " | " << tiempo << " ms" << endl;
            }
        }
    }
    archivo.close();

    cout << "\nPruebas terminadas correctamente." << endl;
    cout << "Archivo creado: resultados_quicksort.csv" << endl;

    return 0;
}
