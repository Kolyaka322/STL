#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
#include <algorithm>

std::string normalizeWord(const std::string& raw) {
    std::string cleaned;
    for (unsigned char ch : raw) {
        if (std::isalnum(ch) || ch == '\'' || ch == '-') {
            cleaned.push_back(static_cast<char>(std::tolower(ch)));
        }
    }
    while (!cleaned.empty() && (cleaned.front() == '-' || cleaned.front() == '\'')) {
        cleaned.erase(cleaned.begin());
    }
    while (!cleaned.empty() && (cleaned.back() == '-' || cleaned.back() == '\'')) {
        cleaned.pop_back();
    }
    return cleaned;
}

std::map<std::string, int> buildFrequencyDictionary(const std::string& inputPath) {
    std::ifstream inFile(inputPath);
    if (!inFile.is_open()) {
        throw std::runtime_error("Не вдалося відкрити вхідний файл: " + inputPath);
    }

    std::map<std::string, int> freqMap;
    std::string token;

    while (inFile >> token) {
        std::string word = normalizeWord(token);
        if (!word.empty()) {
            ++freqMap[word];
        }
    }

    return freqMap;
}

void printAllWords(const std::map<std::string, int>& freqMap) {
    std::cout << "=== Частотний словник усіх слів ===\n";
    if (freqMap.empty()) {
        std::cout << "Слів не знайдено.\n";
        return;
    }

    for (const auto& entry : freqMap) {
        std::cout << entry.first << " : " << entry.second << "\n";
    }
}

void printMostFrequentWord(const std::map<std::string, int>& freqMap) {
    std::cout << "\n=== Найбільш часто вживане слово ===\n";
    if (freqMap.empty()) {
        std::cout << "Словник порожній.\n";
        return;
    }

    auto bestIt = freqMap.begin();
    for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
        if (it->second > bestIt->second) {
            bestIt = it;
        }
    }

    std::cout << "Слово: \"" << bestIt->first << "\" (кількість появ: " << bestIt->second << ")\n";
}

void saveDictionaryToFile(const std::map<std::string, int>& freqMap, const std::string& outputPath) {
    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Не вдалося відкрити вихідний файл для запису: " + outputPath);
    }

    outFile << "Частотний словник слів:\n";
    outFile << "------------------------\n";
    for (const auto& entry : freqMap) {
        outFile << entry.first << " : " << entry.second << "\n";
    }

    if (!freqMap.empty()) {
        auto bestIt = freqMap.begin();
        for (auto it = freqMap.begin(); it != freqMap.end(); ++it) {
            if (it->second > bestIt->second) {
                bestIt = it;
            }
        }
        outFile << "\nНайбільш часто вживане слово: \"" << bestIt->first
            << "\" (" << bestIt->second << ")\n";
    }
}

int main() {
    system("chcp 1251>null");
    const std::string inputPath = "input.txt";
    const std::string outputPath = "frequency_dictionary.txt";

    try {
        {
            std::ofstream sample(inputPath);
            sample << "C++ templates and STL map make dictionary tasks clean. "
                << "STL containers help organize data, and C++ map keeps keys sorted.\n";
        }

        std::map<std::string, int> dictionary = buildFrequencyDictionary(inputPath);

        printAllWords(dictionary);
        printMostFrequentWord(dictionary);
        saveDictionaryToFile(dictionary, outputPath);

        std::cout << "\nРезультат збережено у файл: " << outputPath << "\n";
    }
    catch (const std::exception& ex) {
        std::cerr << "Помилка: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
