#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <functional>

struct SortStats {
    int iterationCount = 0;
    long long timeMicroseconds = 0;
    size_t elementsCount = 0;
    std::string outputFile;
};

template<typename T, typename Compare>
int shellSort(std::vector<T>& arr, Compare comp) {
    int iterations = 0;
    int n = static_cast<int>(arr.size());
    for (int gap = n / 2; gap > 0; gap /= 2) {
        for (int i = gap; i < n; i++) {
            T temp = arr[i];
            int j;
            for (j = i; j >= gap && comp(temp, arr[j - gap]); j -= gap) {
                arr[j] = arr[j - gap];
                iterations++;
            }
            arr[j] = temp;
            if (j != i) iterations++;
        }
    }
    return iterations;
}

bool charLess(char a, char b) {
    return std::tolower(static_cast<unsigned char>(a)) < std::tolower(static_cast<unsigned char>(b));
}

bool stringLess(const std::string& a, const std::string& b) {
    std::string a_low = a, b_low = b;
    std::transform(a_low.begin(), a_low.end(), a_low.begin(),
        [](unsigned char c) { return std::tolower(c); });
    std::transform(b_low.begin(), b_low.end(), b_low.begin(),
        [](unsigned char c) { return std::tolower(c); });
    return a_low < b_low;
}

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

template<typename T>
void writeToFile(const std::vector<T>& data, const std::string& filename) {
    std::ofstream file(filename);
    for (size_t i = 0; i < data.size(); ++i) {
        file << data[i];
        if (i != data.size() - 1) file << " ";
    }
    file.close();
}

std::string detectDataType(const std::string& sample) {
    if (sample.find('.') != std::string::npos || sample.find(',') != std::string::npos) {
        return "Double";
    }
    if (!sample.empty()) {
        size_t start = (sample[0] == '-') ? 1 : 0;
        if (start < sample.size() && std::all_of(sample.begin() + start, sample.end(), ::isdigit)) {
            return "Integer";
        }
    }
    if (sample.length() == 1 && std::isalpha(static_cast<unsigned char>(sample[0]))) {
        return "Letter";
    }
    return "String";
}

template<typename T>
void printStats(const SortStats& stats) {
    std::string typeName;
    if constexpr (std::is_same_v<T, int>) typeName = "Integer";
    else if constexpr (std::is_same_v<T, double>) typeName = "Double";
    else if constexpr (std::is_same_v<T, char>) typeName = "Letter";
    else if constexpr (std::is_same_v<T, std::string>) typeName = "String";
    else typeName = "Unknown";

    std::cout << "\nРезультаты сортировки (" << typeName << ")" << std::endl;
    std::cout << "Количество элементов: " << stats.elementsCount << std::endl;
    std::cout << "Количество итераций: " << stats.iterationCount << std::endl;
    std::cout << "Время выполнения: " << stats.timeMicroseconds / 1000.0 << " миллисекунд" << std::endl;
    std::cout << "Результат сохранен в файл: " << stats.outputFile << std::endl;
}

SortStats processFile(const std::string& inputFile, const std::string& outputFile) {
    auto lines = readFile(inputFile);
    if (lines.empty()) {
        std::cout << "Файл пуст!" << std::endl;
        return {};
    }

    std::string dataType = detectDataType(lines[0]);
    SortStats stats;
    stats.elementsCount = lines.size();
    stats.outputFile = outputFile;

    auto startTime = std::chrono::high_resolution_clock::now();

    if (dataType == "Integer") {
        std::vector<int> data;
        for (const auto& s : lines) data.push_back(std::stoi(s));
        stats.iterationCount = shellSort(data, std::less<int>{});
        writeToFile(data, outputFile);
    }
    else if (dataType == "Double") {
        std::vector<double> data;
        for (const auto& s : lines) {
            std::string tmp = s;
            std::replace(tmp.begin(), tmp.end(), ',', '.');
            std::istringstream iss(tmp);
            iss.imbue(std::locale::classic());
            double val = 0.0;
            iss >> val;
            data.push_back(val);
        }
        stats.iterationCount = shellSort(data, std::less<double>{});
        writeToFile(data, outputFile);
    }
    else if (dataType == "Letter") {
        std::vector<char> data;
        for (const auto& s : lines) data.push_back(s.empty() ? '\0' : s[0]);
        stats.iterationCount = shellSort(data, charLess);
        writeToFile(data, outputFile);
    }
    else {
        std::vector<std::string> data = lines;
        stats.iterationCount = shellSort(data, stringLess);
        writeToFile(data, outputFile);
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    stats.timeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
    return stats;
}

int main() {
    setlocale(LC_ALL, "Russian");
    std::cout << "\nТестирование" << std::endl;

    auto stats = processFile("test_int.txt", "sorted_int.txt");
    printStats<int>(stats);

    stats = processFile("test_double.txt", "sorted_double.txt");
    printStats<double>(stats);

    stats = processFile("test_letter.txt", "sorted_letter.txt");
    printStats<char>(stats);

    stats = processFile("test_string.txt", "sorted_string.txt");
    printStats<std::string>(stats);

    stats = processFile("test_date.txt", "sorted_date.txt");
    printStats<std::string>(stats);

    stats = processFile("test_time.txt", "sorted_time.txt");
    printStats<std::string>(stats);

    return 0;
}