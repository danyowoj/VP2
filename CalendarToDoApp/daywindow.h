#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QMap>
#include <QListWidget>

class DayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DayWindow(const QString &date, QWidget *parent = nullptr);
    void backToCalendar();

private slots:
    void addFoodToTable();
    void removeSelectedFood();
    void applyTemplate(const QString &templateName);

signals:
    void backToCalendarRequested();

private:
    QString date;

    // Вкладки
    QTabWidget *tabWidget;

    // Элементы вкладки "Тренировка"
    QWidget *primaryTab;
    QVBoxLayout *primaryLayout;
    QLineEdit *primaryTaskInput;
    QPushButton *primaryAddTaskButton;
    QListWidget *primaryTaskList;
    QPushButton *primaryRemoveTaskButton;

    // Элементы вкладки "Питание"
    QWidget *secondaryTab;
    QVBoxLayout *secondaryLayout;
    QLineEdit *secondaryFoodInput;
    QPushButton *secondaryAddFoodButton;
    QTableWidget *secondaryFoodTable;
    QPushButton *secondaryRemoveFoodButton;

    // Кнопка возврата
    QPushButton *backButton;

    // Данные о калорийности
    QMap<QString, int> foodCalories;

    void loadFoodData();
    void updateFoodTable();
};

#endif // DAYWINDOW_H
