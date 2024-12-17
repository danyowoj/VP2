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
#include <QLabel>
#include <QDoubleSpinBox>

struct FoodInfo {
    int calories;
    float proteins;
    float fats;
    float carbs;
};

class DayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DayWindow(const QString &date, QWidget *parent = nullptr);
    void backToCalendar();
    ~DayWindow();

private slots:
    void addTaskToPrimary();
    void removeSelectedTaskFromPrimary();
    void addFoodToTable();
    void removeSelectedFood();
    void applyTemplate(const QString &templateName);
    void saveSummaryData();
    void updateSummaryTab();

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

    // Вкладка "Итог дня"
    QWidget *summaryTab;
    QVBoxLayout *summaryLayout;
    QLabel *caloriesSumLabel;
    QLabel *proteinsSumLabel;
    QLabel *fatsSumLabel;
    QLabel *carbsSumLabel;
    QDoubleSpinBox *weightInput;

    // Кнопка возврата
    QPushButton *backButton;

    // Данные о КБЖУ
    QMap<QString, FoodInfo> foodData;

    void loadFoodData();
    void saveFoodData();
    void updateFoodTable();
    void setupFoodTable();
    void calculateMacros();
    void calculateDailySummary();

    // Методы сохранения и загрузки пользовательских данных
    void saveDayData();
    void loadDayData();

    void adjustTabWidths();
};

#endif // DAYWINDOW_H
