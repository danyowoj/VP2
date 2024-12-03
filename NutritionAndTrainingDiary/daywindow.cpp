#include "daywindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DayWindow::DayWindow(const QString &date, QSqlDatabase db, QWidget *parent)
    : QWidget(parent), date(date), db(db) {
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
    secondaryTaskInput->setPlaceholderText("Введите прием пищи...");
    secondaryLayout->addWidget(secondaryTaskInput);

    secondaryAddTaskButton = new QPushButton("Добавить прием пищи", secondaryTab);
    secondaryLayout->addWidget(secondaryAddTaskButton);

    secondaryTaskList = new QListWidget(secondaryTab);
    secondaryLayout->addWidget(secondaryTaskList);

    secondaryRemoveTaskButton = new QPushButton("Удалить выбранный прием пищи", secondaryTab);
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

    // Загрузка задач
    loadTasks("primary", primaryTaskList);
    loadTasks("secondary", secondaryTaskList);
}

void DayWindow::addTaskToPrimary() {
    QString task = primaryTaskInput->text().trimmed();
    if (!task.isEmpty()) {
        primaryTaskList->addItem(task);
        saveTask("primary", task);
        primaryTaskInput->clear();
    }
}

void DayWindow::addTaskToSecondary() {
    QString task = secondaryTaskInput->text().trimmed();
    if (!task.isEmpty()) {
        secondaryTaskList->addItem(task);
        saveTask("secondary", task);
        secondaryTaskInput->clear();
    }
}

void DayWindow::removeSelectedTaskFromPrimary() {
    QListWidgetItem *selectedItem = primaryTaskList->currentItem();
    if (selectedItem) {
        QString task = selectedItem->text();
        delete selectedItem;
        deleteTask("primary", task);
    }
}

void DayWindow::removeSelectedTaskFromSecondary() {
    QListWidgetItem *selectedItem = secondaryTaskList->currentItem();
    if (selectedItem) {
        QString task = selectedItem->text();
        delete selectedItem;
        deleteTask("secondary", task);
    }
}

void DayWindow::loadTasks(const QString &listName, QListWidget *taskList) {
    QSqlQuery query(db);
    query.prepare("SELECT task FROM tasks WHERE date = :date AND listName = :listName");
    query.bindValue(":date", date);
    query.bindValue(":listName", listName);

    if (query.exec()) {
        while (query.next()) {
            QString task = query.value(0).toString();
            taskList->addItem(task);
        }
    } else {
        qDebug() << "Ошибка загрузки задач:" << query.lastError().text();
    }
}

void DayWindow::saveTask(const QString &listName, const QString &task) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO tasks (date, listName, task) VALUES (:date, :listName, :task)");
    query.bindValue(":date", date);
    query.bindValue(":listName", listName);
    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "Ошибка сохранения задачи:" << query.lastError().text();
    }
}

void DayWindow::deleteTask(const QString &listName, const QString &task) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM tasks WHERE date = :date AND listName = :listName AND task = :task");
    query.bindValue(":date", date);
    query.bindValue(":listName", listName);
    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления задачи:" << query.lastError().text();
    }
}
