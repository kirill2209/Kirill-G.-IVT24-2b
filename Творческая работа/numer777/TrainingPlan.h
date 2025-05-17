#ifndef TRAININGPLAN_H
#define TRAININGPLAN_H

#include <QString>
#include <QList>

struct Exercise {
    QString name;
    int duration; // в минутах
    double caloriesPerMinute; // ккал/мин
};

struct DailyTraining {
    QString day;
    QList<Exercise> exercises;
};

class TrainingPlan {
public:
    TrainingPlan();
    QString generateTrainingPlan(QString health, double &caloriesBurnedPerDay, double currentWeight);

private:
    QList<DailyTraining> generateWeightBasedPlan(double currentWeight);
};

#endif
