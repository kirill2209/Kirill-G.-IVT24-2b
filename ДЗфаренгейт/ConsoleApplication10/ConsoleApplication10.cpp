#include <iostream>
#include <Windows.h>

using namespace std;


// Объявление и инициализация глобальной переменной extern
extern double conversionFactor = 9.0 / 5.0;

int main()
{
    setlocale(LC_ALL, "Rus");
    // auto-переменная для автоматического определения типа
    auto celsius = 0.0;

    cout << "Введите температуру в градусах Цельсия: ";
    cin >> celsius;

    // register-переменная для хранения промежуточного результата
    register double intermediate = celsius * conversionFactor;

    // static-переменная для хранения результата перевода, сохраняющая свое значение
    static double fahrenheit = intermediate + 32;

    cout << "Температура в градусах Фаренгейта: " << fahrenheit << endl;

    return 0;
}
