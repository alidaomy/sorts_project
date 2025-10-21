#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <chrono>

// Структура для хранения результатов сортировки
struct SortResult {
    int iterations;
    double time_ms;
};

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

// Сортировка вставками для строк с подсчётом итераций
SortResult insertionSortStrings(std::vector<std::string>& arr) {
    int iterations = 0;
    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 1; i < arr.size(); i++) {
        std::string key = arr[i];
        int j = i - 1;

        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
            iterations++;
        }
        arr[j + 1] = key;
        iterations++; // считаем и внешний цикл
    }

    auto end = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end - start).count();

    return { iterations, time_ms };
}

// Сортировка вставками для чисел (int, double) с подсчётом итераций
template<typename T>
SortResult insertionSortNumbers(std::vector<T>& arr) {
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

    return { iterations, time_ms };
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

// Обработка файла
void processFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Не удалось открыть файл: " << filename << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);
    file.close();

    std::vector<std::string> elements = split(line);
    if (elements.empty()) {
        std::cout << "Файл пуст: " << filename << std::endl;
        return;
    }

    std::cout << "Элементов для сортировки: " << elements.size() << std::endl;

    // Определяем тип данных
    std::string firstElement = elements[0];
    SortResult result;

    if (isNumber(firstElement)) {
        // Проверяем, есть ли десятичная точка для определения double/int
        bool hasDecimal = false;
        for (const auto& elem : elements) {
            if (elem.find('.') != std::string::npos) {
                hasDecimal = true;
                break;
            }
        }

        if (hasDecimal) {
            std::vector<double> numbers;
            for (const auto& elem : elements) {
                numbers.push_back(std::stod(elem));
            }
            result = insertionSortNumbers(numbers);

            // Записываем результат
            std::ofstream outFile("sorted_" + filename);
            for (size_t i = 0; i < numbers.size(); i++) {
                outFile << numbers[i];
                if (i != numbers.size() - 1) outFile << " ";
            }
            outFile.close();
            std::cout << "Отсортированные дробные числа записаны в: sorted_" << filename << std::endl;
        }
        else {
            std::vector<int> numbers;
            for (const auto& elem : elements) {
                numbers.push_back(std::stoi(elem));
            }
            result = insertionSortNumbers(numbers);

            // Записываем результат
            std::ofstream outFile("sorted_" + filename);
            for (size_t i = 0; i < numbers.size(); i++) {
                outFile << numbers[i];
                if (i != numbers.size() - 1) outFile << " ";
            }
            outFile.close();
            std::cout << "Отсортированные целые числа записаны в: sorted_" << filename << std::endl;
        }
    }
    else if (isDate(firstElement)) {
        result = insertionSortStrings(elements);

        std::ofstream outFile("sorted_" + filename);
        for (size_t i = 0; i < elements.size(); i++) {
            outFile << elements[i];
            if (i != elements.size() - 1) outFile << " ";
        }
        outFile.close();
        std::cout << "Отсортированные даты записаны в: sorted_" << filename << std::endl;
    }
    else if (isTime(firstElement)) {
        result = insertionSortStrings(elements);

        std::ofstream outFile("sorted_" + filename);
        for (size_t i = 0; i < elements.size(); i++) {
            outFile << elements[i];
            if (i != elements.size() - 1) outFile << " ";
        }
        outFile.close();
        std::cout << "Отсортированное время записаны в: sorted_" << filename << std::endl;
    }
    else {
        // Считаем это строками/словами
        result = insertionSortStrings(elements);

        std::ofstream outFile("sorted_" + filename);
        for (size_t i = 0; i < elements.size(); i++) {
            outFile << elements[i];
            if (i != elements.size() - 1) outFile << " ";
        }
        outFile.close();
        std::cout << "Отсортированные строки записаны в: sorted_" << filename << std::endl;
    }

    // Выводим статистику
    std::cout << "Время выполнения: " << result.time_ms << " мс" << std::endl;
    std::cout << "Количество итераций: " << result.iterations << std::endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::vector<std::string> files = {
        "test_date.txt",
        "test_double.txt",
        "test_int.txt",
        "test_letter.txt",
        "test_string.txt",
        "test_time.txt"
    };

    for (const auto& file : files) {
        std::cout << "Обрабатываем файл: " << file << std::endl;
        processFile(file);
        std::cout << "----------------------------------------" << std::endl;
    }

    std::cout << "Все файлы обработаны!" << std::endl;
    return 0;
}