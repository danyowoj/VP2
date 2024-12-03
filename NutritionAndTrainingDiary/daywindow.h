#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QWidget>
#include <QSqlDatabase>
#include <QTabWidget>
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
    void addTaskToPrimary();
    void addTaskToSecondary();
    void removeSelectedTaskFromPrimary();
    void removeSelectedTaskFromSecondary();

private:
    QString date;                   // Уникальный идентификатор дня (например, "2024-12-03")
    QSqlDatabase db;                // Подключение к базе данных

    QTabWidget *tabWidget;          // Виджет вкладок

    // Первая вкладка (основной список задач)
    QWidget *primaryTab;
    QVBoxLayout *primaryLayout;
    QLineEdit *primaryTaskInput;
    QPushButton *primaryAddTaskButton;
    QListWidget *primaryTaskList;
    QPushButton *primaryRemoveTaskButton;

    // Вторая вкладка (дополнительный список задач)
    QWidget *secondaryTab;
    QVBoxLayout *secondaryLayout;
    QLineEdit *secondaryTaskInput;
    QPushButton *secondaryAddTaskButton;
    QListWidget *secondaryTaskList;
    QPushButton *secondaryRemoveTaskButton;

    // Кнопка для возврата
    QPushButton *backButton;

    // Методы для загрузки и сохранения задач
    void loadTasks(const QString &listName, QListWidget *taskList);
    void saveTask(const QString &listName, const QString &task);
    void deleteTask(const QString &listName, const QString &task);
};

#endif // DAYWINDOW_H
