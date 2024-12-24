#include <iostream>
#include <cmath>
#include <iomanip>

// Рекурсивная функция для расчета элемента ряда
double powerSeriesElement(int n, double x)
{
    if (n == 1)
    {
        return (x * x) / 2.0; // Первый элемент
    }
    double prev = powerSeriesElement(n - 1, x);
    return prev * (-x * x) / (2.0 * n * (2 * n - 1));
}

// Рекурсивная функция для суммы ряда при заданном n
double N(int n, double x)
{
    if (n == 1)
    {
        return (x * x) / 2.0; // Первый элемент
    }
    return N(n - 1, x) + powerSeriesElement(n, x);
}

// Рекурсивная функция для суммы ряда при заданной точности ε
double E(double x, double epsilon, int n = 1, double sum = 0.0)
{
    double term = powerSeriesElement(n, x);
    if (std::abs(term) < epsilon)
    {
        return sum + term; // Условие остановки
    }
    return E(x, epsilon, n + 1, sum + term);
}

// Основная функция
int main()
{
    const double a = 0.1, b = 0.8; // Диапазон
    const int k = 10;             // Количество шагов
    const int n = 10;             // Заданное количество членов ряда
    const double epsilon = 0.0001; // Заданная точность

    double step = (b - a) / k;    // Шаг изменения x

    // Вывод заголовка таблицы
    std::cout << std::fixed << std::setprecision(6);
    std::cout << "X\t\tSN\t\tSE\t\tY\n";

    // Цикл по значениям x
    for (double x = a; x <= b; x += step)
    {
        double SN = N(n, x);              // Сумма ряда для заданного n
        double SE = E(x, epsilon);        // Сумма ряда для заданной точности
        double Y = x * atan(x) - log(sqrt(1 + x * x));  // Точное значение функции

        // Вывод результатов
        std::cout << x << "\t" << SN << "\t" << SE << "\t" << Y << "\n";
    }

    return 0;
}
