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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

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

    void addFoodToTable();
    void setupDatabase();
    void loadReadyMealsData();
    void addReadyMealToNutrition();
    void loadNutritionData();
    void addMealToNutrition(const QString &mealName);
    void addMealToNutritionTable(const QString &name, int calories, int proteins, int fats, int carbs);
    void editMealNutrition(int row, int column, const QString &newValue);
private slots:
    void addTaskToPrimary();
    void removeSelectedTaskFromPrimary();
    void removeSelectedFood();
    void applyTemplate(const QString &templateName);
    void saveSummaryData();
    void updateSummaryTab();
    void addNewReadyMeal();

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

    //Для готовых блюд
    // Вкладка "Готовые блюда"
    QWidget *readyMealsTab;            // Добавьте это объявление!
    QVBoxLayout *readyMealsLayout;     // Layout для вкладки "Готовые блюда"
    QTableWidget *readyMealsTable;     // Таблица для готовых блюд
    QPushButton *addReadyMealButton;   // Кнопка для добавления блюда в Питание
    QPushButton *readyMealRemoveButton; // Кнопка удаления блюда из базы

    QLineEdit *readyMealNameInput;
    QLineEdit *readyMealCaloriesInput;
    QLineEdit *readyMealProteinsInput;
    QLineEdit *readyMealFatsInput;
    QLineEdit *readyMealCarbsInput;
    QPushButton *addNewReadyMealButton;


    // Кнопка возврата
    QPushButton *backButton;

    // Данные о КБЖУ
    QMap<QString, FoodInfo> foodData;

    void saveFoodData();
    void updateFoodTable();
    void setupFoodTable();
    void calculateMacros();
    void calculateDailySummary();

    // Методы сохранения и загрузки пользовательских данных
    void saveDayData();
    void loadDayData();

    void adjustTabWidths();
    void removeSelectedReadyMeal();
};

#endif // DAYWINDOW_H
