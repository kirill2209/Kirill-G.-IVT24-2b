#include "TrainingPlan.h"

TrainingPlan::TrainingPlan() {}

QList<DailyTraining> TrainingPlan::generateWeightBasedPlan(double currentWeight) {
    QList<DailyTraining> plan;
    if (currentWeight >= 130) { // 150-130 кг: Низкая интенсивность
        plan.append({ "Понедельник", { {"Ходьба (медленный темп)", 40, 4.0}, {"Легкая йога", 20, 2.5} } });
        plan.append({ "Вторник", { {"Плавание (легкий темп)", 30, 4.5}, {"Дыхательная гимнастика", 15, 1.5} } });
        plan.append({ "Среда", { {"Ходьба (медленный темп)", 40, 4.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Четверг", { {"Велотренажер (низкая нагрузка)", 30, 5.0}, {"Йога (баланс)", 20, 2.5} } });
        plan.append({ "Пятница", { {"Ходьба (медленный темп)", 40, 4.0}, {"Легкие танцы", 25, 3.5} } });
        plan.append({ "Суббота", { {"Легкая прогулка", 30, 3.5}, {"Дыхательная гимнастика", 15, 1.5} } });
        plan.append({ "Воскресенье", { {"Отдых", 0, 0.0} } });
    } else if (currentWeight >= 110) { // 130-110 кг: Средняя интенсивность
        plan.append({ "Понедельник", { {"Ходьба (умеренный темп)", 45, 5.0}, {"Йога (выносливость)", 25, 3.0} } });
        plan.append({ "Вторник", { {"Плавание (умеренный темп)", 35, 6.0}, {"Легкие силовые (приседания)", 15, 5.5} } });
        plan.append({ "Среда", { {"Ходьба (умеренный темп)", 45, 5.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Четверг", { {"Велотренажер (средняя нагрузка)", 35, 6.5}, {"Йога (баланс)", 25, 3.0} } });
        plan.append({ "Пятница", { {"Танцы (умеренный темп)", 35, 4.5}, {"Силовые (отжимания)", 15, 5.5} } });
        plan.append({ "Суббота", { {"Активная прогулка", 45, 5.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Воскресенье", { {"Отдых", 0, 0.0} } });
    } else if (currentWeight >= 90) { // 110-90 кг: Высокая интенсивность
        plan.append({ "Понедельник", { {"Бег (легкий)", 30, 8.0}, {"Йога (сила)", 20, 3.5} } });
        plan.append({ "Вторник", { {"Плавание (интенсивное)", 30, 7.5}, {"Силовые (приседания, отжимания)", 20, 6.0} } });
        plan.append({ "Среда", { {"Интервальная ходьба", 35, 9.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Четверг", { {"Велотренажер (высокая нагрузка)", 30, 8.0}, {"Йога (баланс)", 20, 3.5} } });
        plan.append({ "Пятница", { {"Танцы (интенсивные)", 30, 6.0}, {"Силовые (подтягивания)", 15, 6.0} } });
        plan.append({ "Суббота", { {"Бег (умеренный)", 30, 8.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Воскресенье", { {"Отдых", 0, 0.0} } });
    } else { // 90-80 кг: Максимальная интенсивность
        plan.append({ "Понедельник", { {"Бег (умеренный)", 40, 9.0}, {"Силовая тренировка", 25, 7.0} } });
        plan.append({ "Вторник", { {"Плавание (интенсивное)", 35, 8.0}, {"Интервальные упражнения", 20, 7.5} } });
        plan.append({ "Среда", { {"Интервальная ходьба", 40, 10.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Четверг", { {"Велотренажер (высокая нагрузка)", 35, 9.0}, {"Йога (сила)", 20, 3.5} } });
        plan.append({ "Пятница", { {"Танцы (интенсивные)", 35, 7.0}, {"Силовые (все тело)", 20, 7.0} } });
        plan.append({ "Суббота", { {"Бег (интенсивный)", 40, 10.0}, {"Растяжка", 20, 2.0} } });
        plan.append({ "Воскресенье", { {"Отдых", 0, 0.0} } });
    }
    return plan;
}

QString TrainingPlan::generateTrainingPlan(QString health, double &caloriesBurnedPerDay, double currentWeight) {
    QString plan = "=== План тренировок ===\n";
    plan += "Программа адаптирована для людей с " + health + ".\n\n";

    QList<DailyTraining> trainingSchedule = generateWeightBasedPlan(currentWeight);

    double totalCaloriesBurned = 0;
    for (const auto &day : trainingSchedule) {
        plan += day.day + ":\n";
        double dayCalories = 0;
        for (const auto &exercise : day.exercises) {
            double calories = exercise.duration * exercise.caloriesPerMinute;
            dayCalories += calories;
            plan += QString("- %1 (%2 мин, %3 ккал)\n")
                        .arg(exercise.name)
                        .arg(exercise.duration)
                        .arg(calories, 0, 'f', 1);
        }
        totalCaloriesBurned += dayCalories;
    }

    caloriesBurnedPerDay = totalCaloriesBurned / 7; // Среднее за день
    plan += QString("\nСреднее сжигание калорий в день: %1 ккал\n").arg(caloriesBurnedPerDay, 0, 'f', 1);
    return plan;
}
