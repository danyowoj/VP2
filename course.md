Создание адаптивного дневника питания и тренировок с использованием Qt требует структурированного подхода. Давайте разобьем проект на основные части.

## 1. Структура проекта

Ваша структура проекта может выглядеть следующим образом:

```
/DnevnikPitaniyaTrenirovok
    /src
        main.cpp
        mainwindow.cpp
        mainwindow.h
        trainingplan.cpp
        trainingplan.h
        nutrition.cpp
        nutrition.h
        meal.cpp
        meal.h
        summary.cpp
        summary.h
    /resources
        styles.qss
```
## 2. Основные классы и их функционал
### 2.1. Главное окно (MainWindow)

В этом классе мы создадим основное расписание. Это будет `QTableWidget` с кнопками для каждого дня.

```
// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTableWidget *scheduleTable;
    QPushButton *pageSelector[4];  // Для выбора страниц
    void setupSchedule();
};

#endif // MAINWINDOW_H
```

```
// mainwindow.cpp
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Адаптивный Дневник Питания и Тренировок");
    setupSchedule();
}

MainWindow::~MainWindow() {}

void MainWindow::setupSchedule() {
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    scheduleTable = new QTableWidget(14, 8, this); // 14 строк, 8 столбцов (неделя, дни, кнопки)
    // Заполнение заголовков
    // ...
    
    layout->addWidget(scheduleTable);
    setCentralWidget(centralWidget);
}
```
### 2.2. Планы тренировок (TrainingPlan)

Создадим интерфейс для просмотра и добавления упражнений.

```
// trainingplan.h
#ifndef TRAININGPLAN_H
#define TRAININGPLAN_H

#include <QWidget>

class TrainingPlan : public QWidget {
    Q_OBJECT

public:
    explicit TrainingPlan(QWidget *parent = nullptr);
};

#endif // TRAININGPLAN_H
```

```
// trainingplan.cpp
#include "trainingplan.h"

TrainingPlan::TrainingPlan(QWidget *parent) : QWidget(parent) {
    // Инициализация GUI для плана тренировок
    // Создание таблицы с упражнениями
    // Добавление кнопок
}
```
### 2.3. Питание (Nutrition)

Здесь реализуем интерфейс для приема пищи.

```
// nutrition.h
#ifndef NUTRITION_H
#define NUTRITION_H

#include <QWidget>

class Nutrition : public QWidget {
    Q_OBJECT

public:
    explicit Nutrition(QWidget *parent = nullptr);
};

#endif // NUTRITION_H
```

```
// nutrition.cpp
#include "nutrition.h"

Nutrition::Nutrition(QWidget *parent) : QWidget(parent) {
    // Создание интерфейса для выбора приемов пищи
}
```
## 3. Реализация логики меню и функций

Для переключения между экранами и добавления элементов управления можно использовать сигнал и слот:

```
// В основном окне добавим логику для открытия меню
connect(scheduleTable, &QTableWidget::cellClicked, this, &MainWindow::showMenu);
```

Где `showMenu` – это функция, открывающая pop-up с выбором между *"Питанием"* и *"Тренировкой"*.

## 4. Завершение работы

Каждый экран можно адаптировать, чтобы делать его более интерактивным. Можно использовать `QTableWidget` для отображения блюд, калорийности и КБЖУ, а также реализовать логику по добавлению/удалению элементов.

## Итог

Таким образом, мы определили основные компоненты приложения и их функционал. Вы можете доработать каждый класс, добавляя реализацию конкретных взаимодействий и визуальных элементов. Это обеспечит адаптивный интерфейс и позволит пользователю удобно отслеживать их питание и тренировки. Если у Вас есть конкретные вопросы по реализации любого из компонентов, спрашивайте!