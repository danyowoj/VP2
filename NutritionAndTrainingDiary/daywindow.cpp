#include "daywindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

DayWindow::DayWindow(const QString &date, QWidget *parent) : QWidget(parent), date(date) {
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

    // Загрузка задач из файла
    loadTasks();
}

void DayWindow::addTask() {
    QString task = taskInput->text().trimmed();
    if (!task.isEmpty()) {
        taskList->addItem(task); // Добавляем задачу в список
        taskInput->clear();     // Очищаем поле ввода
        saveTasks();            // Сохраняем задачи
    }
}

void DayWindow::removeSelectedTask() {
    QListWidgetItem *selectedItem = taskList->currentItem();
    if (selectedItem) {
        delete selectedItem; // Удаляем выбранную задачу
        saveTasks();         // Сохраняем изменения
    }
}

void DayWindow::loadTasks() {
    QFile file(QString("todo_%1.txt").arg(date));
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString task = in.readLine();
            if (!task.isEmpty()) {
                taskList->addItem(task); // Добавляем задачу в список
            }
        }
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл для загрузки задач:" << file.fileName();
    }
}

void DayWindow::saveTasks() const {
    QFile file(QString("todo_%1.txt").arg(date));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (int i = 0; i < taskList->count(); ++i) {
            out << taskList->item(i)->text() << "\n";
        }
        file.close();
    } else {
        qDebug() << "Не удалось открыть файл для сохранения задач:" << file.fileName();
    }
}
