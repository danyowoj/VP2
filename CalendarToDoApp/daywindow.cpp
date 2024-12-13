#include "daywindow.h"
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>

DayWindow::DayWindow(const QString &date, QWidget *parent)
    : QWidget(parent), date(date) {
    setWindowTitle(QString("День: %1").arg(date));
    resize(800, 600);

    tabWidget = new QTabWidget(this);

    // Вкладка "Тренировка"
    primaryTab = new QWidget(this);
    primaryLayout = new QVBoxLayout(primaryTab);

    primaryTaskInput = new QLineEdit(primaryTab);
    primaryTaskInput->setPlaceholderText("Введите упражнение...");
    primaryAddTaskButton = new QPushButton("Добавить упражнение", primaryTab);
    primaryTaskList = new QListWidget(primaryTab);
    primaryRemoveTaskButton = new QPushButton("Удалить выбранное упражнение", primaryTab);

    primaryLayout->addWidget(primaryTaskInput);
    primaryLayout->addWidget(primaryAddTaskButton);
    primaryLayout->addWidget(primaryTaskList);
    primaryLayout->addWidget(primaryRemoveTaskButton);

    tabWidget->addTab(primaryTab, "Тренировка");

    // Вкладка "Питание"
    secondaryTab = new QWidget(this);
    secondaryLayout = new QVBoxLayout(secondaryTab);

    secondaryFoodTable = new QTableWidget(0, 2, secondaryTab);
    secondaryFoodTable->setHorizontalHeaderLabels(QStringList() << "Блюдо" << "Калории");
    secondaryFoodTable->horizontalHeader()->setStretchLastSection(true);
    secondaryFoodTable->verticalHeader()->setVisible(false);
    secondaryFoodTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    secondaryFoodTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    secondaryFoodInput = new QLineEdit(secondaryTab);
    secondaryAddFoodButton = new QPushButton("Добавить блюдо", secondaryTab);
    secondaryFoodInput->setPlaceholderText("Введите название блюда...");
    secondaryRemoveFoodButton = new QPushButton("Удалить выбранное блюдо", secondaryTab);

    secondaryLayout->addWidget(secondaryFoodInput);
    secondaryLayout->addWidget(secondaryAddFoodButton);
    secondaryLayout->addWidget(secondaryFoodTable);
    secondaryLayout->addWidget(secondaryRemoveFoodButton);

    tabWidget->addTab(secondaryTab, "Питание");

    // Кнопка возврата
    backButton = new QPushButton("Назад", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);

    // Подключение сигналов
    connect(secondaryAddFoodButton, &QPushButton::clicked, this, &DayWindow::addFoodToTable);
    connect(secondaryRemoveFoodButton, &QPushButton::clicked, this, &DayWindow::removeSelectedFood);
    connect(backButton, &QPushButton::clicked, this, &DayWindow::backToCalendar);

    // Загрузка данных о продуктах
    loadFoodData();
}

void DayWindow::loadFoodData() {
    QFile file("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/foods.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл foods.txt");
        return;
    }

    QTextStream in(&file);
    foodCalories.clear();
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(":");
        if (parts.size() == 2) {
            QString food = parts[0].trimmed();
            int calories = parts[1].trimmed().toInt();
            foodCalories[food] = calories;
        }
    }
    file.close();
}

void DayWindow::updateFoodTable() {
    secondaryFoodTable->setRowCount(0);
    for (auto it = foodCalories.begin(); it != foodCalories.end(); ++it) {
        int row = secondaryFoodTable->rowCount();
        secondaryFoodTable->insertRow(row);
        secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(it.key()));
        secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(it.value())));
    }
}

void DayWindow::addFoodToTable() {
    QString food = secondaryFoodInput->text().trimmed();
    if (food.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите название блюда.");
        return;
    }

    int calories = foodCalories.value(food, -1);
    if (calories == -1) {
        QMessageBox::information(this, "Упс...", "Блюдо отсутствует в файле foods.txt. Калорийность не учитывается.");
    }

    int row = secondaryFoodTable->rowCount();
    secondaryFoodTable->insertRow(row);
    secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(food));
    secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(calories)));

    secondaryFoodInput->clear();
}

void DayWindow::removeSelectedFood() {
    auto selectedItems = secondaryFoodTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите блюдо для удаления.");
        return;
    }

    secondaryFoodTable->removeRow(selectedItems[0]->row());
}

void DayWindow::backToCalendar() {
    emit backToCalendarRequested();
}
