#include "Mod28_HW.h"
#include <cstdlib>
#include <ctime>
#include <future>
#include <mutex>

/*
Сортировка данным методом не показала существенного прироста, при некоторых настройках показатели, 
наоборот, ухудшались, несмотря на введенное ограничение потоков 
MAX_THREAD атомарным счетчиком active_threads и проверки длины массива async_on_f

Недостатки:
    много накладных расходов на std::async;
    рекурсия создает много мелких задач;
    слияние — узкое место.

*/ 

using namespace std;

// ограничение потоков
const unsigned int MAX_THREAD = 2;
// Атомарный счетчик потоков
std::atomic<unsigned int> active_threads{ 0 };

// Функция слияния двух отсортированных подмассивов
void merge(int* array, int leftIndex, int midIndex, int rightIndex) {
    // Вычисляем размеры временных массивов
    int leftSize = midIndex - leftIndex + 1;
    int rightSize = rightIndex - midIndex;

    // Создаем временные массивы
    int* leftArray = new int[leftSize];
    int* rightArray = new int[rightSize];

    // Копируем данные во временные массивы
    for (int i = 0; i < leftSize; i++)
        leftArray[i] = array[leftIndex + i];
    for (int j = 0; j < rightSize; j++)
        rightArray[j] = array[midIndex + 1 + j];

    // Индексы для объединения
    int leftCurrent = 0;
    int rightCurrent = 0;
    int mergedCurrent = leftIndex;

    // Слияние временных массивов обратно в основной
    while (leftCurrent < leftSize && rightCurrent < rightSize) {
        if (leftArray[leftCurrent] <= rightArray[rightCurrent]) {
            array[mergedCurrent] = leftArray[leftCurrent];
            leftCurrent++;
        }
        else {
            array[mergedCurrent] = rightArray[rightCurrent];
            rightCurrent++;
        }
        mergedCurrent++;
    }

    // Копируем остаток из левого массива
    while (leftCurrent < leftSize) {
        array[mergedCurrent] = leftArray[leftCurrent];
        leftCurrent++;
        mergedCurrent++;
    }

    // Копируем остаток из правого массива
    while (rightCurrent < rightSize) {
        array[mergedCurrent] = rightArray[rightCurrent];
        rightCurrent++;
        mergedCurrent++;
    }

    // Освобождаем память
    delete[] leftArray;
    delete[] rightArray;
}


// Вызов: mergeSort(arr, 0, size-1);
void mergeSort(int* arr, int l, int r, bool async_on) {
    if (l < r) {
        int m = l + (r - l) / 2; // Находим середину
        bool async_on_f = async_on && (m - l > 1000000);
        
        

        if (async_on_f && active_threads.load() < MAX_THREAD)
        {
            active_threads++; // занимаем слот потока
            future<void> f = async(launch::async, [arr, l, m, async_on]() {
                mergeSort(arr, l, m, async_on); // Сортировка левой части в другом потоке
                active_threads--;               // освобождаем слот потока после завершения
                });

            mergeSort(arr, m + 1, r, async_on);// Сортировка правой части
            f.wait();
        }
        else
        {
            mergeSort(arr, l, m, async_on);    // Сортировка левой части
            mergeSort(arr, m + 1, r, async_on);// Сортировка правой части
        }

        merge(arr, l, m, r);     // Слияние двух частей

    }
}



int main()
{


    setlocale(LC_ALL, "Russian");

    const int size = 10000000;
    int* arr = new int[size];
    int* arr2 = new int[size];

    cout << "Запуск... \n Запуск генерации " << size << " случайных значений..." << endl;

    srand(static_cast<unsigned int>(time(0)));  // Засеять рандомайзер
    
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = (rand() % 100000) + 1;             // Рандом от 1 до 100000
    }

    cout << "Подготовка массива для 2 запуска алгоритма ..." << endl;
    //копирование массива для проверки идентичной ситуации в одном и в нескольких потоках
    for (size_t i = 0; i < size; i++)
    {
        arr2[i] = arr[i];
    }
    
    cout << "Запуск алгоритма merge sort в однопоточном режиме" << endl;

    clock_t time_start = clock();
    mergeSort(arr, 0, size - 1, false);
    clock_t time_end = clock();

    cout << "Алгоритм  merge sort завешил работу за " << double(time_end - time_start) / double(CLOCKS_PER_SEC) << " секунд" << endl;
    

    cout << "\nЗапуск алгоритма merge sort в многопоточном режиме" << endl;

    time_start = clock();
    mergeSort(arr2, 0, size - 1, true);
    time_end = clock();

    cout << "Алгоритм  merge sort завешил работу за " << double(time_end - time_start) / double(CLOCKS_PER_SEC) << " секунд" << endl;


    /*for (size_t i = 0; i < size; i += 10000)
    {
        cout << "arr1[" << i << "] " << arr[i] << "\t\t\t";
        cout << "arr2[" << i << "] " << arr2[i] << "\n";
    }*/

    delete[] arr;
    delete[] arr2;
	return 0;
}
