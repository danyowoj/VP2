#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QListWidget>

class DayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DayWindow(const QString &date, QSqlDatabase db, QWidget *parent = nullptr);

signals:
    void backToCalendar();

private slots:
    void addTask();
    void removeSelectedTask();

private:
    QString date;                // Уникальный идентификатор дня (например, "2024-12-03")
    QVBoxLayout *layout;
    QLineEdit *taskInput;
    QPushButton *addTaskButton;
    QListWidget *taskList;
    QPushButton *removeTaskButton;
    QPushButton *backButton;

    QSqlDatabase db;             // Подключение к базе данных

    void loadTasks();            // Загрузка задач из базы данных
    void saveTask(const QString &task); // Сохранение новой задачи
    void deleteTask(const QString &task); // Удаление задачи
};

#endif // DAYWINDOW_H
