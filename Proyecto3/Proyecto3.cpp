// --------------------------------------- BIBLIOTECAS USADAS EN EL PROYECTO ----------------------------------------------------------

#include <algorithm>
#include <iostream>
#include <vector>
#include <random>
#include <chrono>

using namespace std;
using namespace chrono;

// ------------------------------ GENERAR VECTORES PARA UTILIZAR EN LOS ALGORITMOS DE ORDENAMIENTO --------------------------------------

// Generar un vector sin duplicados
vector<int> generarSinDuplicados(int minSize, int maxSize)
{
    random_device rd;
    mt19937 gen(rd());

    int size = uniform_int_distribution<int>(minSize, maxSize)(gen);

    vector<int> arr(size);

    for(int i = 0; i < size; ++i)
    {
        arr[i] = i;
    }

    shuffle(arr.begin(), arr.end(), gen);

    return arr;
}

// Generar un vector con duplicados
vector<int> generarConDuplicados(int minSize, int maxSize)
{
    random_device rd;
    mt19937 gen(rd());

    int size = uniform_int_distribution<int>(minSize, maxSize)(gen);

    vector<int> arr(size);

    for(int i = 0; i < size; ++i)
    {
        arr[i] = uniform_int_distribution<int>(0, size)(gen);
    }

    return arr;
}

// Generar un vector inversamente ordenado
vector<int> generarOrdenadoinversamente(int minSize, int maxSize)
{
    auto arr = generarSinDuplicados(minSize, maxSize);
    sort(arr.rbegin(), arr.rend());
    return arr;
}

// Generar vector ordenado
vector<int> generarOrdenado(int minSize, int maxSize)
{
    auto arr = generarSinDuplicados(minSize, maxSize);
    sort(arr.begin(), arr.end());
    return arr;
}

// -------------------------------------- IMPLEMENTACION DE ALGORITMOS DE ORDENAMIENTO -----------------------------------------------

// Shell Sort
void shellSort(vector<int>& arr)
{
    int n = arr.size();

    for(int gap = n / 2; gap > 0; gap /= 2)
    {
        for(int i = gap; i < n; ++i)
        {
            int temp = arr[i];
            int j;

            for(j = i; j >= gap && arr[j - gap] > temp; j -= gap)
            {
                arr[j] = arr[j - gap];
            }

            arr[j] = temp;
        }
    }
}

// Selection Sort
void selectionSort(vector<int>& arr)
{
    int n = arr.size();

    for(int i = 0; i < n - 1; ++i)
    {
        int minIndex = i;

        for(int j = i + 1; j < n; ++j)
        {
            if(arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        swap(arr[i], arr[minIndex]);
    }
}

// Insertion Sort
void insertionSort(vector<int>& arr)
{
    int n = arr.size();

    for(int i = 1; i < n; ++i)
    {
        int key = arr[i];
        int j = i - 1;

        while(j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }

        arr[j + 1] = key;
    }
}

// Merge Sort
void merge(vector<int>& arr, int left, int middle, int right)
{
    int n1 = middle - left + 1;
    int n2 = right - middle;

    vector<int> leftArr(n1), rightArr(n2);

    for(int i = 0; i < n1; ++i)
    {
        leftArr[i] = arr[left + i];
    }
    for(int j = 0; j < n2; ++j)
    {
        rightArr[j] = arr[middle + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while(i < n1 && j < n2)
    {
        if (leftArr[i] <= rightArr[j])
        {
            arr[k] = leftArr[i];
            ++i;
        }else
        {
            arr[k] = rightArr[j];
            ++j;
        }
        ++k;
    }

    while(i < n1)
    {
        arr[k] = leftArr[i];
        ++i;
        ++k;
    }

    while(j < n2)
    {
        arr[k] = rightArr[j];
        ++j;
        ++k;
    }
}

void mergeSort(vector<int>& arr, int left, int right)
{
    if (left < right)
    {
        int middle = left + (right - left) / 2;

        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);

        merge(arr, left, middle, right);
    }
}

// Heap Sort
void heapify(vector<int>& arr, int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if(left < n && arr[left] > arr[largest])
    {
        largest = left;
    }

    if(right < n && arr[right] > arr[largest])
    {
        largest = right;
    }

    if(largest != i)
    {
        swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

void heapSort(vector<int>& arr)
{
    int n = arr.size();

    for(int i = n / 2 - 1; i >= 0; --i)
    {
        heapify(arr, n, i);
    }

    for(int i = n - 1; i > 0; --i)
    {
        swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

// Quick Sort
int partition(vector<int>& arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    for(int j = low; j < high; ++j)
    {
        if(arr[j] < pivot)
        {
            ++i;
            swap(arr[i], arr[j]);
        }
    }

    swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high)
{
    vector<pair<int, int>> stack;
    stack.push_back({low, high});

    while (!stack.empty())
    {
        low = stack.back().first;
        high = stack.back().second;
        stack.pop_back();

        int pi = partition(arr, low, high);

        if (pi - 1 > low)
        {
            stack.push_back({low, pi - 1});
        }

        if (pi + 1 < high)
        {
            stack.push_back({pi + 1, high});
        }
    }
}

// Bubble Sort
void bubbleSort(vector<int>& arr)
{
    int n = arr.size();

    for (int i = 0; i < n - 1; ++i)
    {
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// ------------------------------------ FUNCION PARA MEDIR EL TIEMPO DE EJECUCION DE CADA ALGORITMO --------------------------
template <typename Func>
void medirTiempo(Func algoritmo, vector<int> arr, const string& nombreAlgoritmo)
{
    auto inicio = high_resolution_clock::now();

    algoritmo(arr);

    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);

    cout << nombreAlgoritmo << " tuvo un tiempo de " << duracion.count() / 1000.0 << " segundos con un arreglo de " << arr.size() << endl;
}

void medirTiempoHeapSort(vector<int>& arr)
{
    auto inicio = high_resolution_clock::now();
    heapSort(arr);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "Heap Sort tuvo un tiempo de " << duracion.count() / 1000.0 << " segundos con un arreglo de " << arr.size() << endl;
}

void medirTiempoMergeSort(vector<int>& arr)
{
    auto inicio = high_resolution_clock::now();
    mergeSort(arr, 0, arr.size() - 1);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "Merge Sort tuvo un tiempo de " << duracion.count() / 1000.0 << " segundos con un arreglo de " << arr.size() << endl;
}
    
void medirTiempoQuickSort(vector<int>& arr)
{
    auto inicio = high_resolution_clock::now();
    quickSort(arr, 0, arr.size() - 1);
    auto fin = high_resolution_clock::now();
    auto duracion = duration_cast<milliseconds>(fin - inicio);
    cout << "Quick Sort tuvo un tiempo de " << duracion.count() / 1000.0 << " segundos con un arreglo de " << arr.size() << endl;
}

// ------------------------------------ FUNCION PARA HACER LAS CARRERAS DE LOS ALGORITMOS --------------------------

void carreraSinDuplicados(const vector<int>& sinDuplicadosOriginal)
{
    vector<int> sinDuplicados = sinDuplicadosOriginal;
    cout << "\n\n ------ Carrera con arreglo sin Duplicados ------\n\n";
    medirTiempo(selectionSort, sinDuplicados, "Selection Sort");
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempo(insertionSort, sinDuplicados, "Insertion Sort");
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempo(shellSort, sinDuplicados, "Shell Sort");
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempo(bubbleSort, sinDuplicados, "Bubble Sort");
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempoMergeSort(sinDuplicados);
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempoHeapSort(sinDuplicados);
    sinDuplicados = sinDuplicadosOriginal;
    medirTiempoQuickSort(sinDuplicados);
}

void carreraDuplicados(const vector<int>& conDuplicadosOriginal)
{
    vector<int> conDuplicados = conDuplicadosOriginal;
    cout << "\n\n ------ Carrera con arreglo con duplicados ------\n\n";
    medirTiempo(selectionSort, conDuplicados, "Selection Sort");
    conDuplicados = conDuplicadosOriginal;
    medirTiempo(insertionSort, conDuplicados, "Insertion Sort");
    conDuplicados = conDuplicadosOriginal;
    medirTiempo(shellSort, conDuplicados, "Shell Sort");
    conDuplicados = conDuplicadosOriginal;
    medirTiempo(bubbleSort, conDuplicados, "Bubble Sort");
    conDuplicados = conDuplicadosOriginal;
    medirTiempoMergeSort(conDuplicados);
    conDuplicados = conDuplicadosOriginal;
    medirTiempoHeapSort(conDuplicados);
    conDuplicados = conDuplicadosOriginal;
    medirTiempoQuickSort(conDuplicados);
}

void carreraOrdenados(const vector<int>& ordenadosOriginal)
{
    vector<int> ordenados = ordenadosOriginal;
    cout << "\n\n ------ Carrera con arreglo ordenado ------\n\n";
    medirTiempo(selectionSort, ordenados, "Selection Sort");
    ordenados = ordenadosOriginal;
    medirTiempo(insertionSort, ordenados, "Insertion Sort");
    ordenados = ordenadosOriginal;
    medirTiempo(shellSort, ordenados, "Shell Sort");
    ordenados = ordenadosOriginal;
    medirTiempo(bubbleSort, ordenados, "Bubble Sort");
    ordenados = ordenadosOriginal;
    medirTiempoMergeSort(ordenados);
    ordenados = ordenadosOriginal;
    medirTiempoHeapSort(ordenados);
    ordenados = ordenadosOriginal;
    medirTiempoQuickSort(ordenados);
}

void carreraInversamenteOrdenados(const vector<int>& inversamenteOriginal)
{
    vector<int> inversamente = inversamenteOriginal;
    cout << "\n\n ------ Carrera con arreglo inversamente ordenado ------\n\n";
    medirTiempo(selectionSort, inversamente, "Selection Sort");
    inversamente = inversamenteOriginal;
    medirTiempo(insertionSort, inversamente, "Insertion Sort");
    inversamente = inversamenteOriginal;
    medirTiempo(shellSort, inversamente, "Shell Sort");
    inversamente = inversamenteOriginal;
    medirTiempo(bubbleSort, inversamente, "Bubble Sort");
    inversamente = inversamenteOriginal;
    medirTiempoMergeSort(inversamente);
    inversamente = inversamenteOriginal;
    medirTiempoHeapSort(inversamente);
    inversamente = inversamenteOriginal;
    medirTiempoQuickSort(inversamente);
}

void imprimirMenu()
{
    cout << "\n1. Carrera Cola de espera\n";
    cout << "2. Carrera Trazabilidad de objetos\n";
    cout << "3. Carrera Eventos de cada escenario\n";
    cout << "4. Salir\n";
    cout << "Seleccione una opcion: ";
}

int main()
{
    int opcion;

    do
    {
        imprimirMenu();
        cin >> opcion;

        switch (opcion)
        {
            case 1:
                {
                    auto sinDuplicados = generarSinDuplicados(100000, 110000);  
                    auto conDuplicados = generarConDuplicados(100000, 110000);
                    auto ordenados = generarOrdenado(100000, 110000);
                    auto inversamente = generarOrdenadoinversamente(100000, 110000);

                    carreraSinDuplicados(sinDuplicados);
                    carreraDuplicados(conDuplicados);
                    carreraOrdenados(ordenados);
                    carreraInversamenteOrdenados(inversamente);

                }
                break;
            case 2:
                {
                    auto sinDuplicados = generarSinDuplicados(1000, 1500);
                    auto conDuplicados = generarConDuplicados(1000, 1500);
                    auto ordenados = generarOrdenado(1000, 1500);
                    auto inversamente = generarOrdenadoinversamente(1000, 1500);

                    carreraSinDuplicados(sinDuplicados);
                    carreraDuplicados(conDuplicados);
                    carreraOrdenados(ordenados);
                    carreraInversamenteOrdenados(inversamente);
                }
                break;
            case 3:
                {
                    auto sinDuplicados = generarSinDuplicados(60000, 80000);
                    auto conDuplicados = generarConDuplicados(60000, 80000);
                    auto ordenados = generarOrdenado(60000, 80000);
                    auto inversamente = generarOrdenadoinversamente(60000, 80000);

                    carreraSinDuplicados(sinDuplicados);
                    carreraDuplicados(conDuplicados);
                    carreraOrdenados(ordenados);
                    carreraInversamenteOrdenados(inversamente);
                }
                break;
            case 4:
                {
                    cout << "\nCerrando el programa...";
                }
                break;
            default:
                {
                    cout << "Opcion invalida, reintente por favor...\n";
                }
                break;
        }

    } while (opcion != 4);


    return 0;
}
