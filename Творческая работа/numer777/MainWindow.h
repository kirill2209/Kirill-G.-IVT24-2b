#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTextEdit>
#include "NutritionCalculator.h"
#include "TrainingPlan.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void calculatePlan();

private:
    QTabWidget *tabs;
    QLineEdit *heightInput, *weightInput, *ageInput, *targetWeightInput;
    QComboBox *genderCombo, *activityCombo, *healthCombo;
    QTextEdit *resultOutput;
    NutritionCalculator *nutritionCalc;
    TrainingPlan *trainingPlan;
};

#endif
