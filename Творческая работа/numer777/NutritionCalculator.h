#ifndef NUTRITIONCALCULATOR_H
#define NUTRITIONCALCULATOR_H

#include <QString>
#include <QList>

struct Dish {
    QString name;
    double calories;
    double protein;
    double fat;
    double carbs;
};

class NutritionCalculator {
public:
    NutritionCalculator();
    QString calculateNutritionPlan(double height, double weight, int age, double targetWeight, QString gender, QString activity, QString health, double &dailyCaloriesConsumed, double &bmr, double &tdee);

private:
    QList<Dish> dishes;
    double calculateBMR(double height, double weight, int age, QString gender);
    double applyActivityFactor(double bmr, QString activity);
    QString generateMealPlan(double targetCalories, QString health, double &dailyCaloriesConsumed);
};

#endif
