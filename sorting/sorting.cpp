#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <cctype>
#include <iomanip>
#include <functional>

// Структура для хранения результатов сортировки
struct SortResult {
    std::string method_name;
    double time_ms;
    int iterations;
};

// ==================== ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ====================

// Функция для разделения строки на элементы
std::vector<std::string> split(const std::string& str) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;
    while (ss >> item) {
        result.push_back(item);
    }
    return result;
}

// Проверка, является ли строка числом (int или double)
bool isNumber(const std::string& str) {
    if (str.empty()) return false;
    bool hasDecimal = false;
    bool hasSign = false;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        if (c == '-' || c == '+') {
            if (i != 0 || hasSign) return false;
            hasSign = true;
        }
        else if (c == '.') {
            if (hasDecimal) return false;
            hasDecimal = true;
        }
        else if (!isdigit(c)) {
            return false;
        }
    }
    return true;
}

// Проверка, является ли строка датой в формате YYYY-MM-DD
bool isDate(const std::string& str) {
    if (str.length() != 10) return false;
    if (str[4] != '-' || str[7] != '-') return false;
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Проверка, является ли строка временем в формате HH:MM или HH:MM:SS
bool isTime(const std::string& str) {
    if (str.length() != 5 && str.length() != 8) return false;
    size_t colon1 = str.find(':');
    if (colon1 == std::string::npos || colon1 != 2) return false;
    if (str.length() == 8) {
        size_t colon2 = str.find(':', colon1 + 1);
        if (colon2 == std::string::npos || colon2 != 5) return false;
    }
    for (size_t i = 0; i < str.length(); i++) {
        if (i == 2 || i == 5) continue;
        if (!isdigit(str[i])) return false;
    }
    return true;
}

// Чтение файла
std::vector<std::string> readFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<std::string> data;
    if (!file) {
        std::cerr << "Ошибка открытия файла: " << filename << std::endl;
        return data;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            data.push_back(token);
        }
    }
    file.close();
    return data;
}

// Определение типа данных
std::string detectDataType(const std::vector<std::string>& data) {
    if (data.empty()) return "String";

    std::string firstElement = data[0];
    if (isNumber(firstElement)) {
        for (const auto& elem : data) {
            if (elem.find('.') != std::string::npos) {
                return "Double";
            }
        }
        return "Integer";
    }
    else if (isDate(firstElement)) {
        return "Date";
    }
    else if (isTime(firstElement)) {
        return "Time";
    }
    return "String";
}

// ==================== СОРТИРОВКА ВСТАВКАМИ ====================

template<typename T>
SortResult insertionSort(std::vector<T>& arr, const std::string& method_name = "Insertion Sort") {
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 1; i < arr.size(); i++) {
        T key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            iterations++;
        }
        arr[j + 1] = key;
        iterations++;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return { method_name, time_ms, iterations };
}

// ==================== СОРТИРОВКА СЛИЯНИЕМ ====================

template<typename T>
void merge(std::vector<T>& arr, int left, int mid, int right, int& iterations) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<T> L(n1), R(n2);

    for (int i = 0; i < n1; i++) L[i] = arr[left + i];
    for (int j = 0; j < n2; j++) R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        iterations++;
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
        iterations++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        iterations++;
    }
}

template<typename T>
void mergeSortHelper(std::vector<T>& arr, int left, int right, int& iterations) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSortHelper(arr, left, mid, iterations);
    mergeSortHelper(arr, mid + 1, right, iterations);
    merge(arr, left, mid, right, iterations);
}

template<typename T>
SortResult mergeSort(std::vector<T>& arr, const std::string& method_name = "Merge Sort") {
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    if (!arr.empty()) {
        mergeSortHelper(arr, 0, arr.size() - 1, iterations);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return { method_name, time_ms, iterations };
}

// ==================== БЫСТРАЯ СОРТИРОВКА ====================

template<typename T>
void quickSortHelper(std::vector<T>& arr, int left, int right, int& iterations) {
    if (left >= right) return;

    T pivot = arr[(left + right) / 2];
    int i = left, j = right;

    while (i <= j) {
        while (arr[i] < pivot) {
            i++;
            iterations++;
        }
        while (arr[j] > pivot) {
            j--;
            iterations++;
        }
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            i++;
            j--;
            iterations++;
        }
    }

    quickSortHelper(arr, left, j, iterations);
    quickSortHelper(arr, i, right, iterations);
}

template<typename T>
SortResult quickSort(std::vector<T>& arr, const std::string& method_name = "Quick Sort") {
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    if (!arr.empty()) {
        quickSortHelper(arr, 0, arr.size() - 1, iterations);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return { method_name, time_ms, iterations };
}

// ==================== СОРТИРОВКА ШЕЛЛА ====================

template<typename T>
SortResult shellSort(std::vector<T>& arr, const std::string& method_name = "Shell Sort") {
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    int n = static_cast<int>(arr.size());
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            T temp = arr[i];
            int j;
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
                iterations++;
            }
            arr[j] = temp;
            if (j != i) iterations++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return { method_name, time_ms, iterations };
}

// ==================== ОСНОВНАЯ ЛОГИКА ====================

void processFileWithAllMethods(const std::string& filename) {
    std::cout << "\n=== Обработка файла: " << filename << " ===" << std::endl;

    auto data = readFile(filename);
    if (data.empty()) {
        std::cout << "Файл пуст или не найден!" << std::endl;
        return;
    }

    std::string dataType = detectDataType(data);
    std::cout << "Тип данных: " << dataType << std::endl;

    std::vector<SortResult> all_results;

    // Применяем все методы сортировки
    if (dataType == "Integer") {
        std::vector<int> numbers;
        for (const auto& elem : data) numbers.push_back(std::stoi(elem));

        auto numbers1 = numbers; // копии для каждого метода
        auto numbers2 = numbers;
        auto numbers3 = numbers;
        auto numbers4 = numbers;

        all_results.push_back(insertionSort(numbers1));
        all_results.push_back(mergeSort(numbers2));
        all_results.push_back(quickSort(numbers3));
        all_results.push_back(shellSort(numbers4));

    }
    else if (dataType == "Double") {
        std::vector<double> numbers;
        for (const auto& elem : data) {
            std::string tmp = elem;
            std::replace(tmp.begin(), tmp.end(), ',', '.');
            numbers.push_back(std::stod(tmp));
        }

        auto numbers1 = numbers;
        auto numbers2 = numbers;
        auto numbers3 = numbers;
        auto numbers4 = numbers;

        all_results.push_back(insertionSort(numbers1));
        all_results.push_back(mergeSort(numbers2));
        all_results.push_back(quickSort(numbers3));
        all_results.push_back(shellSort(numbers4));

    }
    else {
        // Для строк, дат и времени
        auto data1 = data;
        auto data2 = data;
        auto data3 = data;
        auto data4 = data;

        all_results.push_back(insertionSort(data1));
        all_results.push_back(mergeSort(data2));
        all_results.push_back(quickSort(data3));
        all_results.push_back(shellSort(data4));
    }

    // Выводим результаты
    std::cout << "\nРезультаты сортировки:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    std::cout << std::left << std::setw(15) << "Метод"
        << std::setw(12) << "Время (мс)"
        << std::setw(12) << "Итерации" << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    for (const auto& result : all_results) {
        std::cout << std::left << std::setw(15) << result.method_name
            << std::setw(12) << std::fixed << std::setprecision(3) << result.time_ms
            << std::setw(12) << result.iterations << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;

    // Находим самый быстрый метод
    auto fastest = std::min_element(all_results.begin(), all_results.end(),
        [](const SortResult& a, const SortResult& b) { return a.time_ms < b.time_ms; });

    if (fastest != all_results.end()) {
        std::cout << "Самый быстрый метод: " << fastest->method_name
            << " (" << fastest->time_ms << " мс)" << std::endl;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<std::string> files = {
        "sorted_int.txt",
        "sorted_double.txt",
        "sorted_string.txt",
        "sorted_date.txt",
        "sorted_time.txt",
        "sorted_letter.txt"
    };

    std::cout << "=== СРАВНЕНИЕ МЕТОДОВ СОРТИРОВКИ ===" << std::endl;
    std::cout << "Доступные методы: Insertion Sort, Merge Sort, Quick Sort, Shell Sort" << std::endl;

    for (const auto& file : files) {
        processFileWithAllMethods(file);
        std::cout << std::endl;
    }

    std::cout << "Все файлы обработаны!" << std::endl;
    return 0;
}