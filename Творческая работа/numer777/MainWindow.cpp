#include "MainWindow.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Планировщик питания и тренировок");
    setWindowIcon(QIcon(":/icons/app_icon.png"));
    setStyleSheet(
        "QMainWindow { background-color: #E6F0FA; }"
        "QLabel { font-size: 14px; color: #333; }"
        "QLineEdit { border: 1px solid #ccc; border-radius: 5px; padding: 5px; background-color: #F9F9F9; }"
        "QComboBox { border: 1px solid #ccc; border-radius: 5px; padding: 5px; background-color: #F9F9F9; }"
        "QComboBox QAbstractItemView { background-color: #F9F9F9; border: 1px solid #ccc; }"
        "QTabWidget::pane { border: 1px solid #ccc; background-color: #FFFFFF; }"
        "QTabBar::tab { background-color: #D3E4F9; color: #333; padding: 8px 16px; border-top-left-radius: 5px; border-top-right-radius: 5px; }"
        "QTabBar::tab:selected { background-color: #FFFFFF; border-bottom: 2px solid #4A90E2; }"
        "QTextEdit { border: 1px solid #ccc; border-radius: 5px; background-color: #F9F9F9; font-size: 14px; }"
        );

    tabs = new QTabWidget(this);
    setCentralWidget(tabs);

    QWidget *surveyTab = new QWidget();
    QVBoxLayout *surveyLayout = new QVBoxLayout(surveyTab);
    surveyLayout->setContentsMargins(20, 20, 20, 20);
    QLabel *surveyTitle = new QLabel("Введите ваши данные");
    surveyTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #4A90E2; margin-bottom: 10px;");
    surveyLayout->addWidget(surveyTitle);

    QFormLayout *formLayout = new QFormLayout();
    formLayout->setLabelAlignment(Qt::AlignRight);
    formLayout->setSpacing(10);

    heightInput = new QLineEdit();
    weightInput = new QLineEdit();
    ageInput = new QLineEdit();
    targetWeightInput = new QLineEdit();
    genderCombo = new QComboBox();
    genderCombo->addItems({"Мужской", "Женский"});
    activityCombo = new QComboBox();
    activityCombo->addItems({"Низкая", "Средняя", "Высокая"});
    healthCombo = new QComboBox();
    healthCombo->addItems({"Диабет", "Преддиабет"});

    formLayout->addRow("Рост (см):", heightInput);
    formLayout->addRow("Вес (кг):", weightInput);
    formLayout->addRow("Возраст:", ageInput);
    formLayout->addRow("Целевой вес (кг):", targetWeightInput);
    formLayout->addRow("Пол:", genderCombo);
    formLayout->addRow("Уровень активности:", activityCombo);
    formLayout->addRow("Состояние здоровья:", healthCombo);

    QPushButton *calculateButton = new QPushButton("Рассчитать план");
    calculateButton->setStyleSheet(
        "QPushButton { background-color: #4A90E2; color: white; border-radius: 5px; padding: 10px; font-size: 14px; }"
        "QPushButton:hover { background-color: #357ABD; }"
        "QPushButton:pressed { background-color: #2A6395; }"
        );
    connect(calculateButton, &QPushButton::clicked, this, &MainWindow::calculatePlan);

    surveyLayout->addLayout(formLayout);
    surveyLayout->addSpacing(10);
    surveyLayout->addWidget(calculateButton, 0, Qt::AlignCenter);
    surveyLayout->addStretch();

    QWidget *resultTab = new QWidget();
    QVBoxLayout *resultLayout = new QVBoxLayout(resultTab);
    resultLayout->setContentsMargins(20, 20, 20, 20);

    QLabel *resultTitle = new QLabel("Ваш план");
    resultTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #4A90E2; margin-bottom: 10px;");
    resultLayout->addWidget(resultTitle);

    resultOutput = new QTextEdit();
    resultOutput->setReadOnly(true);
    resultLayout->addWidget(resultOutput);

    tabs->addTab(surveyTab, "Опрос");
    tabs->addTab(resultTab, "План");

    nutritionCalc = new NutritionCalculator();
    trainingPlan = new TrainingPlan();
    setMinimumSize(600, 400);
}

void MainWindow::calculatePlan() {
    bool ok;
    double height = heightInput->text().toDouble(&ok);
    double weight = weightInput->text().toDouble(&ok);
    int age = ageInput->text().toInt(&ok);
    double targetWeight = targetWeightInput->text().toDouble(&ok);
    QString gender = genderCombo->currentText();
    QString activity = activityCombo->currentText();
    QString health = healthCombo->currentText();

    if (!ok || height <= 0 || weight <= 0 || age <= 0 || targetWeight <= 0) {
        QMessageBox::warning(this, "Ошибка", "Введите корректные данные!");
        return;
    }

    double dailyCaloriesConsumed = 0;
    double bmr = 0, tdee = 0;
    QString nutritionPlan = nutritionCalc->calculateNutritionPlan(height, weight, age, targetWeight, gender, activity, health, dailyCaloriesConsumed, bmr, tdee);

    double caloriesBurnedPerDay = 0;
    QString trainingPlanText = trainingPlan->generateTrainingPlan(health, caloriesBurnedPerDay, weight); // Исправлен вызов

    // Формула Харриса-Бенедикта с подстановкой значений
    QString bmrFormula;
    if (gender == "Мужской") {
        bmrFormula = QString("BMR = 88.362 + (13.397 × %1) + (4.799 × %2) - (5.677 × %3) = %4 ккал/день")
                         .arg(weight)
                         .arg(height)
                         .arg(age)
                         .arg(bmr, 0, 'f', 1);
    } else {
        bmrFormula = QString("BMR = 447.593 + (9.247 × %1) + (3.098 × %2) - (4.330 × %3) = %4 ккал/день")
                         .arg(weight)
                         .arg(height)
                         .arg(age)
                         .arg(bmr, 0, 'f', 1);
    }

    // Коэффициент активности
    double activityFactor = (activity == "Низкая") ? 1.2 : (activity == "Средняя") ? 1.375 : 1.55;
    QString activityText = QString("TDEE = BMR × %1 (активность: %2) = %3 ккал/день")
                               .arg(activityFactor)
                               .arg(activity)
                               .arg(tdee, 0, 'f', 1);

    // Общее сжигание калорий: TDEE + тренировочные калории
    double totalCaloriesBurned = tdee + caloriesBurnedPerDay;

    // Проверка и корректировка баланса калорий
    QString balanceMessage;
    double calorieBalance = dailyCaloriesConsumed - totalCaloriesBurned;
    if (targetWeight < weight) { // Похудение
        if (calorieBalance >= 0 || calorieBalance > -1000) { // Цель: дефицит 500-1000 ккал
            double requiredDeficit = qMax(500.0, dailyCaloriesConsumed * 0.25); // Минимум 500 или 25% от потребления
            double requiredBurn = dailyCaloriesConsumed - requiredDeficit;
            caloriesBurnedPerDay = (requiredBurn > tdee) ? (requiredBurn - tdee) : 0;
            totalCaloriesBurned = tdee + caloriesBurnedPerDay;
            calorieBalance = dailyCaloriesConsumed - totalCaloriesBurned;
            balanceMessage = QString("Для похудения сжигаемые калории (тренировки) увеличены до %1 ккал/день (общее сжигание: %2 ккал/день, дефицит: %3 ккал/день).\n")
                                 .arg(caloriesBurnedPerDay, 0, 'f', 1)
                                 .arg(totalCaloriesBurned, 0, 'f', 1)
                                 .arg(-calorieBalance, 0, 'f', 1);
        }
        balanceMessage += "Цель: дефицит калорий для похудения.\n";
    } else if (targetWeight > weight) { // Набор веса
        if (calorieBalance <= 300) {
            double requiredBurn = dailyCaloriesConsumed - 300;
            caloriesBurnedPerDay = (requiredBurn < tdee) ? 0 : (requiredBurn - tdee);
            totalCaloriesBurned = tdee + caloriesBurnedPerDay;
            calorieBalance = dailyCaloriesConsumed - totalCaloriesBurned;
            balanceMessage = QString("Для набора веса сжигаемые калории (тренировки) уменьшены до %1 ккал/день (общее сжигание: %2 ккал/день).\n")
                                 .arg(caloriesBurnedPerDay, 0, 'f', 1)
                                 .arg(totalCaloriesBurned, 0, 'f', 1);
        }
        balanceMessage += "Цель: профицит калорий для набора веса.\n";
    }

    // Пояснение о наборе/сбросе
    QString gainLossMessage;
    if (calorieBalance > 0) {
        gainLossMessage = QString("Остаток калорий: %1 ккал/день. Примерно 7700 ккал = 1 кг веса, это около %2 кг/месяц.\n")
                              .arg(calorieBalance, 0, 'f', 1)
                              .arg(calorieBalance * 30 / 7700, 0, 'f', 2);
    } else if (calorieBalance < 0) {
        gainLossMessage = QString("Дефицит калорий: %1 ккал/день. Примерно 7700 ккал = 1 кг веса, это около %2 кг/месяц.\n")
                              .arg(-calorieBalance, 0, 'f', 1)
                              .arg(-calorieBalance * 30 / 7700, 0, 'f', 2);
    } else {
        gainLossMessage = "Баланс калорий нулевой, вес останется стабильным.\n";
    }

    QString result = "=== Ваш план ===\n\n"
                     "Расчет по формуле Харриса-Бенедикта:\n" + bmrFormula + "\n" + activityText + "\n\n" +
                     nutritionPlan + "\n" + trainingPlanText + "\n" + balanceMessage +
                     QString("Итоговый баланс калорий: %1 ккал/день (Потребление: %2, Сжигание: %3 )\n")
                         .arg(calorieBalance, 0, 'f', 1)
                         .arg(dailyCaloriesConsumed, 0, 'f', 1)
                         .arg(totalCaloriesBurned, 0, 'f', 1) +
                     gainLossMessage;
    resultOutput->setText(result);

    QFile file("plan.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << result;
        file.close();
    }
}
