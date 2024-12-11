#include "daywindow.h"

DayWindow::DayWindow(const QString &date, QWidget *parent)
    : QWidget(parent), date(date) {
    setWindowTitle(QString("День %1").arg(date));
    resize(500, 400);

    // Создаем виджет вкладок
    tabWidget = new QTabWidget(this);

    // Создаем вкладку "Тренировка"
    primaryTab = new QWidget(this);
    primaryLayout = new QVBoxLayout(primaryTab);

    primaryTaskInput = new QLineEdit(primaryTab);
    primaryTaskInput->setPlaceholderText("Введите упражнение...");
    primaryLayout->addWidget(primaryTaskInput);

    primaryAddTaskButton = new QPushButton("Добавить упражнение", primaryTab);
    primaryLayout->addWidget(primaryAddTaskButton);

    primaryTaskList = new QListWidget(primaryTab);
    primaryLayout->addWidget(primaryTaskList);

    primaryRemoveTaskButton = new QPushButton("Удалить выбранное упражнение", primaryTab);
    primaryLayout->addWidget(primaryRemoveTaskButton);

    tabWidget->addTab(primaryTab, "Тренировка");

    // Создаем вкладку "Питание"
    secondaryTab = new QWidget(this);
    secondaryLayout = new QVBoxLayout(secondaryTab);

    secondaryTaskInput = new QLineEdit(secondaryTab);
    secondaryTaskInput->setPlaceholderText("Введите блюдо...");
    secondaryLayout->addWidget(secondaryTaskInput);

    secondaryAddTaskButton = new QPushButton("Добавить блюдо", secondaryTab);
    secondaryLayout->addWidget(secondaryAddTaskButton);

    secondaryTaskList = new QListWidget(secondaryTab);
    secondaryLayout->addWidget(secondaryTaskList);

    secondaryRemoveTaskButton = new QPushButton("Удалить выбранное блюдо", secondaryTab);
    secondaryLayout->addWidget(secondaryRemoveTaskButton);

    tabWidget->addTab(secondaryTab, "Питание");

    // Кнопка возврата
    backButton = new QPushButton("Назад", this);

    // Основной компоновщик
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);

    // Подключение сигналов для вкладки "Тренировка"
    connect(primaryAddTaskButton, &QPushButton::clicked, this, &DayWindow::addTaskToPrimary);
    connect(primaryRemoveTaskButton, &QPushButton::clicked, this, &DayWindow::removeSelectedTaskFromPrimary);

    // Подключение сигналов для вкладки "Питание"
    connect(secondaryAddTaskButton, &QPushButton::clicked, this, &DayWindow::addTaskToSecondary);
    connect(secondaryRemoveTaskButton, &QPushButton::clicked, this, &DayWindow::removeSelectedTaskFromSecondary);

    // Подключение кнопки возврата
    connect(backButton, &QPushButton::clicked, this, &DayWindow::backToCalendar);
}

void DayWindow::addTaskToPrimary() {
    QString task = primaryTaskInput->text().trimmed();
    if (!task.isEmpty()) {
        primaryTaskList->addItem(task);
        primaryTaskInput->clear();
    }
}

void DayWindow::addTaskToSecondary() {
    QString task = secondaryTaskInput->text().trimmed();
    if (!task.isEmpty()) {
        secondaryTaskList->addItem(task);
        secondaryTaskInput->clear();
    }
}

void DayWindow::removeSelectedTaskFromPrimary() {
    QListWidgetItem *selectedItem = primaryTaskList->currentItem();
    if (selectedItem) {
        delete selectedItem;
    }
}

void DayWindow::removeSelectedTaskFromSecondary() {
    QListWidgetItem *selectedItem = secondaryTaskList->currentItem();
    if (selectedItem) {
        delete selectedItem;
    }
}
