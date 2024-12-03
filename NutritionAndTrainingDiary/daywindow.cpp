#include "daywindow.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DayWindow::DayWindow(const QString &date, QSqlDatabase db, QWidget *parent)
    : QWidget(parent), date(date), db(db) {
    setWindowTitle(QString("День %1").arg(date));
    resize(400, 300);

    layout = new QVBoxLayout(this);

    // Поле для ввода задачи
    taskInput = new QLineEdit(this);
    taskInput->setPlaceholderText("Введите задачу...");
    layout->addWidget(taskInput);

    // Кнопка добавления задачи
    addTaskButton = new QPushButton("Добавить задачу", this);
    layout->addWidget(addTaskButton);

    // Список задач
    taskList = new QListWidget(this);
    layout->addWidget(taskList);

    // Кнопка удаления выбранной задачи
    removeTaskButton = new QPushButton("Удалить выбранную задачу", this);
    layout->addWidget(removeTaskButton);

    // Кнопка возврата к календарю
    backButton = new QPushButton("Назад", this);
    layout->addWidget(backButton);

    // Подключение сигналов
    connect(addTaskButton, &QPushButton::clicked, this, &DayWindow::addTask);
    connect(removeTaskButton, &QPushButton::clicked, this, &DayWindow::removeSelectedTask);
    connect(backButton, &QPushButton::clicked, this, &DayWindow::backToCalendar);

    // Загрузка задач из базы данных
    loadTasks();
}

void DayWindow::addTask() {
    QString task = taskInput->text().trimmed();
    if (!task.isEmpty()) {
        taskList->addItem(task);   // Добавляем задачу в список
        saveTask(task);           // Сохраняем задачу в базу данных
        taskInput->clear();       // Очищаем поле ввода
    }
}

void DayWindow::removeSelectedTask() {
    QListWidgetItem *selectedItem = taskList->currentItem();
    if (selectedItem) {
        QString task = selectedItem->text();
        delete selectedItem;       // Удаляем задачу из списка
        deleteTask(task);          // Удаляем задачу из базы данных
    }
}

void DayWindow::loadTasks() {
    QSqlQuery query(db);
    query.prepare("SELECT task FROM tasks WHERE date = :date");
    query.bindValue(":date", date);

    if (query.exec()) {
        while (query.next()) {
            QString task = query.value(0).toString();
            taskList->addItem(task);
        }
    } else {
        qDebug() << "Ошибка загрузки задач:" << query.lastError().text();
    }
}

void DayWindow::saveTask(const QString &task) {
    QSqlQuery query(db);
    query.prepare("INSERT INTO tasks (date, task) VALUES (:date, :task)");
    query.bindValue(":date", date);
    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "Ошибка сохранения задачи:" << query.lastError().text();
    }
}

void DayWindow::deleteTask(const QString &task) {
    QSqlQuery query(db);
    query.prepare("DELETE FROM tasks WHERE date = :date AND task = :task");
    query.bindValue(":date", date);
    query.bindValue(":task", task);

    if (!query.exec()) {
        qDebug() << "Ошибка удаления задачи:" << query.lastError().text();
    }
}
