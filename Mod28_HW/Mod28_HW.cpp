#include "Mod28_HW.h"
#include <cstdlib>
#include <ctime>
#include <future>



using namespace std;

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
void mergeSort(int* arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2; // Находим середину
        mergeSort(arr, l, m);    // Сортировка левой части
        mergeSort(arr, m + 1, r);// Сортировка правой части
        merge(arr, l, m, r);     // Слияние двух частей
    }
}



int main()
{


    setlocale(LC_ALL, "Russian");

    const int size = 100000;
    int* arr = new int[size];
	
    cout << "Запуск... \n Запуск генерации " << size << " случайных значений..." << endl;

    srand(static_cast<unsigned int>(time(0)));  // Засеять рандомайзер
    
    for (size_t i = 0; i < size; i++)
    {
        arr[i] = (rand() % 100000) + 1;             // Рандом от 1 до 100000
    }
    
    cout << "Запуск алгоритма merge sort в однопоточном режиме" << endl;

    clock_t time_start = clock();
    mergeSort(arr, 0, size - 1);
    clock_t time_end = clock();

    cout << "Алгоритм  merge sort завешил работу за " << double(time_end - time_start) / double(CLOCKS_PER_SEC) << " секунд" << endl;
    
    int enter = 0;
    for (size_t i = 0; i < size; i++)
    {
        
        cout << arr[i] << " ";
        if (++enter % 10 == 0)
            cout << "\n";
    }

    delete[] arr;
	return 0;
}
