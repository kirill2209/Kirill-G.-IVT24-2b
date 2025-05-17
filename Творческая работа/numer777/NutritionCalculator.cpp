#include "NutritionCalculator.h"
#include <QRandomGenerator>

NutritionCalculator::NutritionCalculator() {
    dishes = {
        {"Гречка с куриной грудкой", 120, 10, 2, 15},
        {"Овощной суп с чечевицей", 80, 4, 1, 12},
        {"Запеченная треска с брокколи", 100, 15, 3, 5},
        {"Салат из свежих овощей с оливковым маслом", 50, 1, 3, 5},
        {"Тушеная индейка с кабачками", 110, 12, 4, 6},
        {"Овсянка на воде с ягодами", 90, 3, 2, 15},
        {"Паровые котлеты из говядины", 130, 14, 5, 2},
        {"Рагу из овощей с фасолью", 85, 4, 2, 13},
        {"Запеченный лосось с лимоном", 140, 15, 7, 0},
        {"Киноа с авокадо и шпинатом", 110, 4, 5, 12},
        {"Куриный суп с цветной капустой", 70, 6, 2, 8},
        {"Тушеная капуста с грибами", 60, 2, 1, 10},
        {"Рыбные котлеты на пару", 120, 13, 4, 3},
        {"Салат с тунцом и яйцом", 100, 10, 5, 3},
        {"Запеченная курица с травами", 125, 14, 4, 1},
        {"Овощной омлет без масла", 80, 7, 4, 2},
        {"Стручковая фасоль с чесноком", 50, 2, 1, 8},
        {"Гречневая каша с грибами", 95, 4, 2, 14},
        {"Запеченный судак с зеленью", 110, 16, 3, 0},
        {"Салат с курицей и огурцом", 90, 10, 3, 4},
        {"Тушеная брокколи с яйцом", 70, 5, 3, 6}
    };
}

double NutritionCalculator::calculateBMR(double height, double weight, int age, QString gender) {
    if (gender == "Мужской") {
        return 88.362 + (13.397 * weight) + (4.799 * height) - (5.677 * age);
    } else {
        return 447.593 + (9.247 * weight) + (3.098 * height) - (4.330 * age);
    }
}

double NutritionCalculator::applyActivityFactor(double bmr, QString activity) {
    if (activity == "Низкая") return bmr * 1.2;
    if (activity == "Средняя") return bmr * 1.375;
    return bmr * 1.55;
}

QString NutritionCalculator::generateMealPlan(double targetCalories, QString health, double &dailyCaloriesConsumed) {
    QString plan = "=== План питания ===\n";
    double dailyCalories = 0;
    int mealCount = 4;
    double caloriesPerMeal = targetCalories / mealCount;

    for (int i = 0; i < mealCount && dailyCalories < targetCalories; ++i) {
        QString mealName = (i == 0) ? "Завтрак" : (i == 1) ? "Обед" : (i == 2) ? "Полдник" : "Ужин";
        int dishIndex = QRandomGenerator::global()->bounded(dishes.size());
        Dish dish = dishes[dishIndex];
        double portion = caloriesPerMeal / dish.calories * 100;
        dailyCalories += caloriesPerMeal;

        plan += QString("%1: %2 (%3 г)\nКалории: %4 ккал, Белки: %5 г, Жиры: %6 г, Углеводы: %7 г\n")
                    .arg(mealName)
                    .arg(dish.name)
                    .arg(portion, 0, 'f', 0)
                    .arg(dish.calories * portion / 100, 0, 'f', 0)
                    .arg(dish.protein * portion / 100, 0, 'f', 1)
                    .arg(dish.fat * portion / 100, 0, 'f', 1)
                    .arg(dish.carbs * portion / 100, 0, 'f', 1);
    }

    dailyCaloriesConsumed = dailyCalories;
    plan += QString("\nОбщая калорийность: %1 ккал\n").arg(dailyCalories, 0, 'f', 0);
    return plan;
}

QString NutritionCalculator::calculateNutritionPlan(double height, double weight, int age, double targetWeight, QString gender, QString activity, QString health, double &dailyCaloriesConsumed, double &bmr, double &tdee) {
    bmr = calculateBMR(height, weight, age, gender);
    tdee = applyActivityFactor(bmr, activity);
    double targetCalories = tdee;

    if (targetWeight < weight) {
        targetCalories -= 400; // Дефицит для похудения
    } else if (targetWeight > weight) {
        targetCalories += 500; // Профицит для набора
    }

    return generateMealPlan(targetCalories, health, dailyCaloriesConsumed);
}
