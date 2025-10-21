#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>

struct SortStats {
    int iterationCount;
    long long timeMicroseconds;
    size_t elementsCount;
};
SortStats globalStats;


// Функция для целых числе
void mergeInt(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<int> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        globalStats.iterationCount++;
        if (L[i] < R[j]) {
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
        globalStats.iterationCount++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        globalStats.iterationCount++;
    }
}
void mergeSortInt(std::vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortInt(arr, left, mid);
    mergeSortInt(arr, mid + 1, right);
    mergeInt(arr, left, mid, right);
}

//Функция для вещественных чисел
void mergeDouble(std::vector<double>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<double> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        globalStats.iterationCount++;
        if (L[i] < R[j]) {
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
        globalStats.iterationCount++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        globalStats.iterationCount++;
    }
}
void mergeSortDouble(std::vector<double>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortDouble(arr, left, mid);
    mergeSortDouble(arr, mid + 1, right);
    mergeDouble(arr, left, mid, right);
}

//Функция для символов
void mergeChar(std::vector<char>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<char> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        globalStats.iterationCount++;
        // Приводим к нижнему регистру для сравнения
        char leftChar = std::tolower(L[i]);
        char rightChar = std::tolower(R[j]);
        if (leftChar < rightChar) {
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
        globalStats.iterationCount++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        globalStats.iterationCount++;
    }
}
void mergeSortChar(std::vector<char>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortChar(arr, left, mid);
    mergeSortChar(arr, mid + 1, right);
    mergeChar(arr, left, mid, right);
}

//Функция для строк
void mergeString(std::vector<std::string>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<std::string> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        globalStats.iterationCount++;
        std::string leftStr = L[i];
        std::string rightStr = R[j];
        // Нижний регистр для сравнения
        std::transform(leftStr.begin(), leftStr.end(), leftStr.begin(), ::tolower);
        std::transform(rightStr.begin(), rightStr.end(), rightStr.begin(), ::tolower);
        bool less = false;
        bool found = false;
        size_t min_len = std::min(leftStr.length(), rightStr.length());

        for (size_t idx = 0; idx < min_len; ++idx) {
            if (leftStr[idx] != rightStr[idx]) {
                less = (leftStr[idx] < rightStr[idx]);
                found = true;
                break;
            }
        }
        if (!found) {
            less = (leftStr.length() < rightStr.length());
        }

        if (less) {
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
        globalStats.iterationCount++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        globalStats.iterationCount++;
    }
}
void mergeSortString(std::vector<std::string>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortString(arr, left, mid);
    mergeSortString(arr, mid + 1, right);
    mergeString(arr, left, mid, right);
}

//Функции для дат и времени
struct DateTime {
    int year, month, day;
    int hour, minute, second;
    // Инициализация полей
    DateTime() : year(0), month(0), day(0), hour(0), minute(0), second(0) {}
    DateTime(const std::string& dateStr) : year(0), month(0), day(0), hour(0), minute(0), second(0) {
        int result = 0;
        if (dateStr.find('-') != std::string::npos && dateStr.find(':') != std::string::npos) {
            result = sscanf_s(dateStr.c_str(), "%d-%d-%d %d:%d:%d",
                &year, &month, &day, &hour, &minute, &second);
        }
        else if (dateStr.find('-') != std::string::npos) {
            result = sscanf_s(dateStr.c_str(), "%d-%d-%d", &year, &month, &day);
        }
        else if (dateStr.find('.') != std::string::npos && dateStr.find(':') != std::string::npos) {
            result = sscanf_s(dateStr.c_str(), "%d.%d.%d %d:%d:%d",
                &day, &month, &year, &hour, &minute, &second);
        }
        else if (dateStr.find('.') != std::string::npos) {
            result = sscanf_s(dateStr.c_str(), "%d.%d.%d", &day, &month, &year);
        }
        else if (dateStr.find(':') != std::string::npos) {
            // Только время
            if (dateStr.length() == 5) {
                result = sscanf_s(dateStr.c_str(), "%d:%d", &hour, &minute);
            }
            else if (dateStr.length() == 8) {
                result = sscanf_s(dateStr.c_str(), "%d:%d:%d", &hour, &minute, &second);
            }
        }
        (void)result;
    }
};
void mergeDateTime(std::vector<DateTime>& arr, int left, int mid, int right, bool compareByDate) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    std::vector<DateTime> L(n1), R(n2);
    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        globalStats.iterationCount++;
        bool less = false;
        if (compareByDate) {
            // Сравнение по дате (и времени)
            if (L[i].year != R[j].year) {
                less = (L[i].year < R[j].year);
            }
            else if (L[i].month != R[j].month) {
                less = (L[i].month < R[j].month);
            }
            else if (L[i].day != R[j].day) {
                less = (L[i].day < R[j].day);
            }
            else if (L[i].hour != R[j].hour) {
                less = (L[i].hour < R[j].hour);
            }
            else if (L[i].minute != R[j].minute) {
                less = (L[i].minute < R[j].minute);
            }
            else {
                less = (L[i].second < R[j].second);
            }
        }
        else {
            // Сравнение только по времени
            if (L[i].hour != R[j].hour) {
                less = (L[i].hour < R[j].hour);
            }
            else if (L[i].minute != R[j].minute) {
                less = (L[i].minute < R[j].minute);
            }
            else {
                less = (L[i].second < R[j].second);
            }
        }

        if (less) {
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
        globalStats.iterationCount++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
        globalStats.iterationCount++;
    }
}
void mergeSortDateTime(std::vector<DateTime>& arr, int left, int right, bool compareByDate) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSortDateTime(arr, left, mid, compareByDate);
    mergeSortDateTime(arr, mid + 1, right, compareByDate);
    mergeDateTime(arr, left, mid, right, compareByDate);
}

//Определение типов данных
std::string detectDataType(const std::string& sample) {
    // Проверка на вещественное число
    if (sample.find('.') != std::string::npos || sample.find(',') != std::string::npos) {
        return "Double";
    }
    // Проверка на целое число
    if (!sample.empty()) {
        size_t start = 0;
        if (sample[0] == '-') start = 1;
        bool isInt = std::all_of(sample.begin() + start, sample.end(), ::isdigit);
        if (isInt) return "Integer";
    }
    // Проверка на символ
    if (sample.length() == 1 && std::isalpha(sample[0])) return "Letter";
    // Проверка на время
    if (sample.find(':') != std::string::npos) {
        if (sample.length() == 5 && sample[2] == ':') return "Time";
        if (sample.length() == 8 && sample[2] == ':' && sample[5] == ':') return "Time";
    }
    // Проверка на дату
    if (sample.length() == 10) {
        if ((sample[4] == '-' && sample[7] == '-') ||
            (sample[2] == '.' && sample[5] == '.')) {
            return "Date";
        }
    }
    // Проверка на дату-время
    if (sample.length() >= 16) {
        if ((sample.find('-') != std::string::npos && sample.find(':') != std::string::npos) ||
            (sample.find('.') != std::string::npos && sample.find(':') != std::string::npos)) {
            return "Date";
        }
    }
    return "String";
}

// Чтение и запись файлов
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

void writeToFile(const std::vector<int>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

void writeToFile(const std::vector<double>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

void writeToFile(const std::vector<char>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

void writeToFile(const std::vector<std::string>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

void writeDateTimeToFile(const std::vector<DateTime>& data, const std::string& filename, const std::string& type) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        const DateTime& dt = data[i];
        if (type == "Date") {
            if (dt.hour == 0 && dt.minute == 0 && dt.second == 0) {
                file << std::setw(4) << std::setfill('0') << dt.year << "-"
                    << std::setw(2) << std::setfill('0') << dt.month << "-"
                    << std::setw(2) << std::setfill('0') << dt.day;
            }
            else {
                file << std::setw(4) << std::setfill('0') << dt.year << "-"
                    << std::setw(2) << std::setfill('0') << dt.month << "-"
                    << std::setw(2) << std::setfill('0') << dt.day << " "
                    << std::setw(2) << std::setfill('0') << dt.hour << ":"
                    << std::setw(2) << std::setfill('0') << dt.minute << ":"
                    << std::setw(2) << std::setfill('0') << dt.second;
            }
        }
        else if (type == "Time") {
            file << std::setw(2) << std::setfill('0') << dt.hour << ":"
                << std::setw(2) << std::setfill('0') << dt.minute;
            if (dt.second > 0) {
                file << ":" << std::setw(2) << std::setfill('0') << dt.second;
            }
        }
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

//Сортировка

void sortFile(const std::string& inputFile, const std::string& outputFile) {
    auto startTime = std::chrono::high_resolution_clock::now();
    std::vector<std::string> lines = readFile(inputFile);
    if (lines.empty()) {
        std::cout << "Файл пуст!" << std::endl;
        return;
    }
    globalStats.elementsCount = lines.size();
    globalStats.iterationCount = 0;
    std::cout << "Первый элемент: " << lines[0] << std::endl;
    std::string dataType = detectDataType(lines[0]);
    std::cout << "Тип: " << dataType << std::endl;
    if (dataType == "Integer") {
        std::vector<int> data;
        for (const auto& item : lines) {
            data.push_back(std::stoi(item));
        }
        mergeSortInt(data, 0, data.size() - 1);
        writeToFile(data, outputFile);
    }
    else if (dataType == "Double") {
        std::vector<double> data;
        for (const auto& item : lines) {
            // Запятая на точку
            std::string s = item;
            std::replace(s.begin(), s.end(), ',', '.');
            // istringstream с C-локалью, чтобы не зависеть от LC_NUMERIC
            std::istringstream iss(s);
            iss.imbue(std::locale::classic());
            double val = 0.0;
            if (!(iss >> val)) {
                // в случае ошибки парсинга можно кинуть исключение или пропустить
                // здесь просто ставим 0.0
                val = 0.0;
            }
            data.push_back(val);
        }
        mergeSortDouble(data, 0, data.size() - 1);
        writeToFile(data, outputFile);
    }
    else if (dataType == "Letter") {
        std::vector<char> data;
        for (const auto& item : lines) {
            if (!item.empty()) {
                data.push_back(item[0]);
            }
        }
        mergeSortChar(data, 0, data.size() - 1);
        writeToFile(data, outputFile);
    }
    else if (dataType == "Date") {
        std::vector<DateTime> data;
        for (const auto& item : lines) {
            data.push_back(DateTime(item));
        }
        mergeSortDateTime(data, 0, data.size() - 1, true);
        writeDateTimeToFile(data, outputFile, "Date");
    }
    else if (dataType == "Time") {
        std::vector<DateTime> data;
        for (const auto& item : lines) {
            data.push_back(DateTime(item));
        }
        mergeSortDateTime(data, 0, data.size() - 1, false);
        writeDateTimeToFile(data, outputFile, "Time");
    }
    else {
        // String
        std::vector<std::string> data = lines;
        mergeSortString(data, 0, data.size() - 1);
        writeToFile(data, outputFile);
    }
    auto endTime = std::chrono::high_resolution_clock::now();
    globalStats.timeMicroseconds =
        std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    std::cout << "\nРезультаты сортировки" << std::endl;
    std::cout << "Количество элементов: " << globalStats.elementsCount << std::endl;
    std::cout << "Количество итераций: " << globalStats.iterationCount << std::endl;
    std::cout << "Время выполнения: " << globalStats.timeMicroseconds / 1000.0 << " миллисекунд" << std::endl;
    std::cout << "Результат сохранен в файл: " << outputFile << std::endl;
}

//Тесты
int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "\nТестировние" << std::endl;
    sortFile("test_int.txt", "sorted_int.txt");
    std::cout << std::endl;
    sortFile("test_double.txt", "sorted_double.txt");
    std::cout << std::endl;
    sortFile("test_letter.txt", "sorted_letter.txt");
    std::cout << std::endl;
    sortFile("test_string.txt", "sorted_string.txt");
    std::cout << std::endl;
    sortFile("test_date.txt", "sorted_date.txt");
    std::cout << std::endl;
    sortFile("test_time.txt", "sorted_time.txt");

    return 0;
}