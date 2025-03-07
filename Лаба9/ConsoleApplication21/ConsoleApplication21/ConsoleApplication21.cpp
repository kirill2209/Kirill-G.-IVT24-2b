#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

int main() {
    system("cls");
    system("chcp 1251");
    setlocale(LC_ALL, "Ru");
    const string file_f1 = "F1.txt";
    const string file_f2 = "F2.txt";
    int start_line;

    // 1. Получение начальной строки от пользователя
    cout << "Введите номер строки, с которой начать копирование (минимум 4): ";
    cin >> start_line;

    if (start_line < 4) {
        cerr << "Ошибка: Номер строки должен быть больше или равен 4." << endl;
        return 1;
    }

    // Очистка буфера ввода после cin
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // 2. Создание и запись в F1 (теперь с возможностью ручного ввода строк)
    ofstream f1(file_f1);
    if (!f1.is_open()) {
        cerr << "Ошибка: не удалось создать/открыть файл " << file_f1 << endl;
        return 1;
    }

    cout << "Введите строки для файла " << file_f1 << ".  Для завершения введите 'END'\n";
    string line;
    vector<string> lines_f1;

    while (getline(cin, line)) {
        if (line == "END") {
            break;
        }
        lines_f1.push_back(line);
        f1 << line << "\n";  // Запись в файл во время ввода
    }

    f1.close();

    // Проверка, что в файле F1 достаточно строк.
    if (lines_f1.size() < 1) {
        cerr << "Ошибка: В файле " << file_f1 << " должно быть хотя бы 1 строка." << endl;
        return 1;
    }

    // 3. Копирование строк из F1 в F2 и вывод в консоль
    ifstream f1_read(file_f1); // Reopen file for reading
    ofstream f2(file_f2);

    if (!f1_read.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << file_f1 << " для чтения." << endl;
        return 1;
    }
    if (!f2.is_open()) {
        cerr << "Ошибка: не удалось создать/открыть файл " << file_f2 << " для записи." << endl;
        f1_read.close();
        return 1;
    }

    string read_line;
    int line_number = 1;
    cout << "Строки, скопированные из " << file_f1 << " в " << file_f2 << " и выведенные в консоль:\n";
    while (getline(f1_read, read_line)) {
        if (line_number >= start_line) {
            f2 << read_line << "\n";
            cout << read_line << "\n";
        }
        line_number++;
    }
    f1_read.close();
    f2.close();


    // 4. Подсчет символов в последнем слове F2
    ifstream f2_read(file_f2);
    if (!f2_read.is_open()) {
        cerr << "Ошибка: не удалось открыть файл " << file_f2 << " для чтения." << endl;
        return 1;
    }

    string last_word;
    string current_line;
    string word;

    while (getline(f2_read, current_line)) {
        stringstream ss(current_line);
        while (ss >> word) {
            last_word = word;
        }
    }
    f2_read.close();

    cout << "Длина последнего слова в файле " << file_f2 << ": " << last_word.length() << endl;

    return 0;
}
