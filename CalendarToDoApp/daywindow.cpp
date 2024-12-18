#include "daywindow.h"
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <QTextCodec>
#include <QDoubleSpinBox>
#include <QDebug>

DayWindow::DayWindow(const QString &date, QWidget *parent)
    : QWidget(parent), date(date)
{ 
    // Устанавливаем иконку приложения
    QIcon appIcon("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/icons/app.png");
    this->setWindowIcon(appIcon);

    setWindowTitle(QString("День: %1").arg(date));

    setMinimumSize(670, 450);
    resize(960, 720);

    tabWidget = new QTabWidget(this);

    // Загружаем стили из файла styles.qss
    QFile styleFile("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/styles.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream stream(&styleFile);
        QString styleSheet = stream.readAll();
        this->setStyleSheet(styleSheet);
        styleFile.close();
    }
    else
    {
        qDebug() << "Не удалось загрузить файл стилей.";
    }

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

    secondaryFoodTable = new QTableWidget(0, 5, secondaryTab);
    secondaryFoodTable->setHorizontalHeaderLabels({"Блюдо", "Калории", "Белки (г)", "Жиры (г)", "Углеводы (г)"});
    secondaryFoodTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    secondaryFoodTable->verticalHeader()->setVisible(false);
    secondaryFoodTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    secondaryFoodTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    setupFoodTable();

    secondaryFoodInput = new QLineEdit(secondaryTab);
    secondaryAddFoodButton = new QPushButton("Добавить блюдо", secondaryTab);
    secondaryFoodInput->setPlaceholderText("Введите название блюда...");
    secondaryRemoveFoodButton = new QPushButton("Удалить выбранное блюдо", secondaryTab);

    secondaryLayout->addWidget(secondaryFoodInput);
    secondaryLayout->addWidget(secondaryAddFoodButton);
    secondaryLayout->addWidget(secondaryFoodTable);
    secondaryLayout->addWidget(secondaryRemoveFoodButton);

    tabWidget->addTab(secondaryTab, "Питание");

    // Вкладка "Итог дня"
    summaryTab = new QWidget(this);
    summaryLayout = new QVBoxLayout(summaryTab);

    caloriesSumLabel = new QLabel("Калории: 0", summaryTab);
    proteinsSumLabel = new QLabel("Белки: 0 г", summaryTab);
    fatsSumLabel = new QLabel("Жиры: 0 г", summaryTab);
    carbsSumLabel = new QLabel("Углеводы: 0 г", summaryTab);

    // Поле для ввода веса
    weightInput = new QDoubleSpinBox(summaryTab);
    weightInput->setRange(0, 500); // Диапазон веса
    weightInput->setDecimals(1);   // 1 знак после запятой
    weightInput->setPrefix("Вес: ");
    weightInput->setValue(0.0);

    summaryLayout->addWidget(caloriesSumLabel);
    summaryLayout->addWidget(proteinsSumLabel);
    summaryLayout->addWidget(fatsSumLabel);
    summaryLayout->addWidget(carbsSumLabel);
    summaryLayout->addWidget(weightInput);

    tabWidget->addTab(summaryTab, "Итог дня");

    // Вкладка "Готовые блюда"
    readyMealsTab = new QWidget(this);
    readyMealsLayout = new QVBoxLayout(readyMealsTab);

    // Таблица "Готовые блюда"
    readyMealsTable = new QTableWidget(0, 5, readyMealsTab);
    readyMealsTable->setHorizontalHeaderLabels({"Название", "Калории", "Белки (г)", "Жиры (г)", "Углеводы (г)"});
    readyMealsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    readyMealsTable->verticalHeader()->setVisible(false);
    readyMealsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    readyMealsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // Устанавливаем политику растягивания колонок
    QHeaderView *header = readyMealsTable->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch);          // Колонка с названием блюда занимает оставшееся место
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Калории
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Белки
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Жиры
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Углеводы

    // Запрещаем пользователю менять размер колонок вручную
    header->setSectionsMovable(false);

    // Минимальная высота строки для лучшего отображения
    readyMealsTable->verticalHeader()->setDefaultSectionSize(25);

    // Поля ввода для нового блюда
    readyMealNameInput = new QLineEdit(readyMealsTab);
    readyMealCaloriesInput = new QLineEdit(readyMealsTab);
    readyMealProteinsInput = new QLineEdit(readyMealsTab);
    readyMealFatsInput = new QLineEdit(readyMealsTab);
    readyMealCarbsInput = new QLineEdit(readyMealsTab);

    // Метка для каждого поля
    readyMealNameInput->setPlaceholderText("Название блюда...");
    readyMealCaloriesInput->setPlaceholderText("Калории");
    readyMealProteinsInput->setPlaceholderText("Белки (г)");
    readyMealFatsInput->setPlaceholderText("Жиры (г)");
    readyMealCarbsInput->setPlaceholderText("Углеводы (г)");

    // Создаем горизонтальный layout для полей ввода
    QHBoxLayout *inputsLayout = new QHBoxLayout();
    inputsLayout->addWidget(readyMealCaloriesInput);
    inputsLayout->addWidget(readyMealProteinsInput);
    inputsLayout->addWidget(readyMealFatsInput);
    inputsLayout->addWidget(readyMealCarbsInput);

    // Кнопка добавления нового блюда
    addNewReadyMealButton = new QPushButton("Добавить новое блюдо", readyMealsTab);

    // Кнопка добавления блюда в Питание
    addReadyMealButton = new QPushButton("Добавить блюдо в Питание", readyMealsTab);

    // Кнопка удаления блюда из базы
    readyMealRemoveButton = new QPushButton("Удалить выбранное блюдо", readyMealsTab);

    // Добавление виджетов на вкладку
    readyMealsLayout->addWidget(readyMealNameInput);
    readyMealsLayout->addLayout(inputsLayout);
    readyMealsLayout->addWidget(addNewReadyMealButton);
    readyMealsLayout->addWidget(addReadyMealButton);
    readyMealsLayout->addWidget(readyMealsTable);
    readyMealsLayout->addWidget(readyMealRemoveButton);
    tabWidget->addTab(readyMealsTab, "Готовые блюда");



    // Кнопка возврата
    backButton = new QPushButton("Назад", this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(backButton);

    setLayout(mainLayout);

    // Подключение сигналов

    connect(primaryAddTaskButton, &QPushButton::clicked, this, &DayWindow::addTaskToPrimary);
    connect(primaryRemoveTaskButton, &QPushButton::clicked, this, &DayWindow::removeSelectedTaskFromPrimary);
    connect(secondaryAddFoodButton, &QPushButton::clicked, this, &DayWindow::addFoodToTable);
    connect(secondaryRemoveFoodButton, &QPushButton::clicked, this, &DayWindow::removeSelectedFood);
    connect(backButton, &QPushButton::clicked, this, &DayWindow::backToCalendar);
    connect(tabWidget, &QTabWidget::currentChanged, this, &DayWindow::updateSummaryTab);
    connect(weightInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &DayWindow::saveDayData);
    connect(addReadyMealButton, &QPushButton::clicked, this, &DayWindow::addReadyMealToNutrition);
    connect(addNewReadyMealButton, &QPushButton::clicked, this, &DayWindow::addNewReadyMeal);
    connect(readyMealRemoveButton, &QPushButton::clicked, this, &DayWindow::removeSelectedReadyMeal);

    setupDatabase();
    // Загрузка данных о продуктах
    loadDayData(); // Загружаем данные
    loadNutritionData();
    loadReadyMealsData();

    adjustTabWidths();
}

void DayWindow::loadReadyMealsData()
{
    QSqlQuery query;
    if (!query.exec("SELECT name, calories, proteins, fats, carbs FROM ready_meals"))
    {
        qDebug() << "Ошибка загрузки готовых блюд:" << query.lastError().text();
        return;
    }

    readyMealsTable->setRowCount(0);

    while (query.next())
    {
        int row = readyMealsTable->rowCount();
        readyMealsTable->insertRow(row);

        readyMealsTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));

        // Форматируем значения с 1 знаком после запятой для всех числовых данных
        readyMealsTable->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toFloat(), 'f', 1)));
        readyMealsTable->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toFloat(), 'f', 1)));
        readyMealsTable->setItem(row, 3, new QTableWidgetItem(QString::number(query.value(3).toFloat(), 'f', 1)));
        readyMealsTable->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toFloat(), 'f', 1)));
    }
}

void DayWindow::addNewReadyMeal()
{
    // Считываем данные из полей ввода
    QString name = readyMealNameInput->text().trimmed();
    QString caloriesStr = readyMealCaloriesInput->text().trimmed();
    QString proteinsStr = readyMealProteinsInput->text().trimmed();
    QString fatsStr = readyMealFatsInput->text().trimmed();
    QString carbsStr = readyMealCarbsInput->text().trimmed();

    // Проверяем, что все поля заполнены
    if (name.isEmpty() || caloriesStr.isEmpty() || proteinsStr.isEmpty() || fatsStr.isEmpty() || carbsStr.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, заполните все поля.");
        return;
    }

    // Преобразуем строки в соответствующие типы данных
    bool okCalories, okProteins, okFats, okCarbs;
    int calories = caloriesStr.toInt(&okCalories);
    float proteins = proteinsStr.toFloat(&okProteins);
    float fats = fatsStr.toFloat(&okFats);
    float carbs = carbsStr.toFloat(&okCarbs);

    if (!okCalories || !okProteins || !okFats || !okCarbs)
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите корректные числовые значения.");
        return;
    }

    // Добавляем блюдо в таблицу
    int row = readyMealsTable->rowCount();
    readyMealsTable->insertRow(row);
    readyMealsTable->setItem(row, 0, new QTableWidgetItem(name));
    readyMealsTable->setItem(row, 1, new QTableWidgetItem(QString::number(calories)));
    readyMealsTable->setItem(row, 2, new QTableWidgetItem(QString::number(proteins)));
    readyMealsTable->setItem(row, 3, new QTableWidgetItem(QString::number(fats)));
    readyMealsTable->setItem(row, 4, new QTableWidgetItem(QString::number(carbs)));

    // Вставляем блюдо в базу данных
    QSqlQuery query;
    query.prepare("INSERT INTO ready_meals (name, calories, proteins, fats, carbs) "
                  "VALUES (:name, :calories, :proteins, :fats, :carbs)");

    query.bindValue(":name", name);
    query.bindValue(":calories", calories);
    query.bindValue(":proteins", proteins);
    query.bindValue(":fats", fats);
    query.bindValue(":carbs", carbs);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось добавить блюдо в базу данных: " + query.lastError().text());
        return;
    }

    // Очистим поля ввода
    readyMealNameInput->clear();
    readyMealCaloriesInput->clear();
    readyMealProteinsInput->clear();
    readyMealFatsInput->clear();
    readyMealCarbsInput->clear();

    // Перезагружаем данные для отображения
    loadReadyMealsData();
}

void DayWindow::addReadyMealToNutrition()
{
    QList<QTableWidgetItem *> selectedItems = readyMealsTable->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Выберите блюдо из списка.");
        return;
    }

    // Получаем данные о блюде
    QString foodName = selectedItems[0]->text();
    float calories = selectedItems[1]->text().toFloat();
    float proteins = selectedItems[2]->text().toFloat();
    float fats = selectedItems[3]->text().toFloat();
    float carbs = selectedItems[4]->text().toFloat();

    // Форматируем данные с 1 знаком после запятой
    QString formattedCalories = QString::number(calories, 'f', 1);
    QString formattedProteins = QString::number(proteins, 'f', 1);
    QString formattedFats = QString::number(fats, 'f', 1);
    QString formattedCarbs = QString::number(carbs, 'f', 1);

    // Добавляем данные в таблицу "Питание"
    int row = secondaryFoodTable->rowCount();
    secondaryFoodTable->insertRow(row);

    secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(foodName));
    secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(formattedCalories));
    secondaryFoodTable->setItem(row, 2, new QTableWidgetItem(formattedProteins));
    secondaryFoodTable->setItem(row, 3, new QTableWidgetItem(formattedFats));
    secondaryFoodTable->setItem(row, 4, new QTableWidgetItem(formattedCarbs));

    // Вставляем данные в базу данных
    QSqlQuery query;
    query.prepare("INSERT INTO nutrition (date, name, calories, proteins, fats, carbs) "
                  "VALUES (:date, :name, :calories, :proteins, :fats, :carbs)");

    query.bindValue(":date", date);                   // Привязываем выбранную дату
    query.bindValue(":name", foodName);               // Привязываем имя блюда
    query.bindValue(":calories", formattedCalories.toFloat());  // Привязываем калории
    query.bindValue(":proteins", formattedProteins.toFloat()); // Привязываем белки
    query.bindValue(":fats", formattedFats.toFloat());         // Привязываем жиры
    query.bindValue(":carbs", formattedCarbs.toFloat());       // Привязываем углеводы

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось добавить блюдо в базу данных: " + query.lastError().text());
    }
}

DayWindow::~DayWindow()
{
    saveDayData();
}

void DayWindow::addTaskToPrimary()
{
    QString task = primaryTaskInput->text().trimmed();
    if (!task.isEmpty())
    {
        primaryTaskList->addItem(task);

        QSqlQuery query;
        query.prepare("INSERT INTO training (date, exercise) VALUES (:date, :exercise)");
        query.bindValue(":date", date);
        query.bindValue(":exercise", task);
        query.exec();

        primaryTaskInput->clear();
    }
}

void DayWindow::removeSelectedTaskFromPrimary()
{
    QListWidgetItem *selectedItem = primaryTaskList->currentItem();
    if (selectedItem)
    {
        QSqlQuery query;
        query.prepare("DELETE FROM training WHERE date = :date AND exercise = :exercise");
        query.bindValue(":date", date);
        query.bindValue(":exercise", selectedItem->text());
        query.exec();

        delete selectedItem;
    }
}

void DayWindow::addFoodToTable()
{
    // Получаем название блюда из поля ввода
    QString foodName = secondaryFoodInput->text().trimmed();

    // Проверяем, что имя не пустое
    if (foodName.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Пожалуйста, введите название блюда.");
        return;
    }

    // Запрос в базу данных для получения информации о блюде
    QSqlQuery query;
    query.prepare("SELECT calories, proteins, fats, carbs FROM ready_meals WHERE name = :name");
    query.bindValue(":name", foodName);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось найти информацию о блюде: " + query.lastError().text());
        return;
    }

    if (!query.next())
    {
        QMessageBox::warning(this, "Блюдо не найдено", "Блюдо с таким названием не найдено в базе данных.");
        return;
    }

    // Получаем данные из базы данных
    int calories = query.value(0).toInt();
    float proteins = query.value(1).toFloat();
    float fats = query.value(2).toFloat();
    float carbs = query.value(3).toFloat();

    // Добавляем блюдо в таблицу питания
    int row = secondaryFoodTable->rowCount();
    secondaryFoodTable->insertRow(row);

    secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(foodName));
    secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(calories)));
    secondaryFoodTable->setItem(row, 2, new QTableWidgetItem(QString::number(proteins)));
    secondaryFoodTable->setItem(row, 3, new QTableWidgetItem(QString::number(fats)));
    secondaryFoodTable->setItem(row, 4, new QTableWidgetItem(QString::number(carbs)));

    // Вставляем данные в таблицу питания в базе данных
    query.prepare("INSERT INTO nutrition (date, name, calories, proteins, fats, carbs) "
                  "VALUES (:date, :name, :calories, :proteins, :fats, :carbs)");

    query.bindValue(":date", date);                   // Привязываем выбранную дату
    query.bindValue(":name", foodName);               // Привязываем имя блюда
    query.bindValue(":calories", calories);          // Привязываем калории
    query.bindValue(":proteins", proteins);          // Привязываем белки
    query.bindValue(":fats", fats);                  // Привязываем жиры
    query.bindValue(":carbs", carbs);                // Привязываем углеводы

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось добавить блюдо в базу данных: " + query.lastError().text());
        return;
    }

    // Очищаем поле ввода
    secondaryFoodInput->clear();
}

void DayWindow::removeSelectedFood()
{
    auto selectedItems = secondaryFoodTable->selectedItems();
    if (selectedItems.isEmpty())
        return;

    QString foodName = secondaryFoodTable->item(selectedItems[0]->row(), 0)->text();
    QSqlQuery query;
    query.prepare("DELETE FROM nutrition WHERE date = :date AND name = :name");
    query.bindValue(":date", date);
    query.bindValue(":name", foodName);
    query.exec();

    secondaryFoodTable->removeRow(selectedItems[0]->row());
}

void DayWindow::backToCalendar()
{
    emit backToCalendarRequested();
}

void DayWindow::loadNutritionData()
{
    QSqlQuery query;
    if (!query.exec("SELECT name, calories, proteins, fats, carbs FROM nutrition"))
    {
        qDebug() << "Ошибка загрузки Nutrition:" << query.lastError().text();
        return;
    }

    secondaryFoodTable->setRowCount(0);

    while (query.next())
    {
        int row = secondaryFoodTable->rowCount();
        secondaryFoodTable->insertRow(row);

        secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));

        // Форматируем значения с 1 знаком после запятой для всех числовых данных
        secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toFloat(), 'f', 1)));
        secondaryFoodTable->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toFloat(), 'f', 1)));
        secondaryFoodTable->setItem(row, 3, new QTableWidgetItem(QString::number(query.value(3).toFloat(), 'f', 1)));
        secondaryFoodTable->setItem(row, 4, new QTableWidgetItem(QString::number(query.value(4).toFloat(), 'f', 1)));
    }
}

void DayWindow::setupFoodTable()
{
    // Устанавливаем политику растягивания колонок
    QHeaderView *header = secondaryFoodTable->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch);          // Колонка с названием блюда занимает оставшееся место
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Калории
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Белки
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Жиры
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Углеводы

    // Запрещаем пользователю менять размер колонок вручную
    header->setSectionsMovable(false);

    // Минимальная высота строки для лучшего отображения
    secondaryFoodTable->verticalHeader()->setDefaultSectionSize(25);
}

void DayWindow::saveDayData()
{
    double weight = weightInput->value();
    QSqlQuery query;
    query.prepare("INSERT OR REPLACE INTO weight (date, weight) VALUES (:date, :weight)");
    query.bindValue(":date", date);
    query.bindValue(":weight", weight);
    query.exec();
}

void DayWindow::loadDayData()
{
    primaryTaskList->clear();
    secondaryFoodTable->setRowCount(0);

    QSqlQuery query;

    // Загрузка упражнений
    query.prepare("SELECT exercise FROM training WHERE date = :date");
    query.bindValue(":date", date);
    query.exec();
    while (query.next())
    {
        primaryTaskList->addItem(query.value(0).toString());
    }

    // Загрузка данных о питании
    query.prepare("SELECT name, calories, proteins, fats, carbs FROM nutrition WHERE date = :date");
    query.bindValue(":date", date);
    query.exec();
    while (query.next())
    {
        int row = secondaryFoodTable->rowCount();
        secondaryFoodTable->insertRow(row);
        for (int col = 0; col < 5; ++col)
        {
            secondaryFoodTable->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
    }

    // Загрузка веса
    query.prepare("SELECT weight FROM weight WHERE date = :date");
    query.bindValue(":date", date);
    query.exec();
    if (query.next())
    {
        weightInput->setValue(query.value(0).toDouble());
    }
}

void DayWindow::saveSummaryData()
{
    double weight = weightInput->value();
    QString weightStr = QString::number(weight, 'f', 1);

    // Сохраняем информацию о весе и макросах
    QFile file(QString("data/%1_summary.txt").arg(date));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить данные.");
        return;
    }

    QTextStream out(&file);

    out << "Вес: " << weightStr << "\n";
    out << "Калории: " << caloriesSumLabel->text().split(": ")[1] << "\n";
    out << "Белки: " << proteinsSumLabel->text().split(": ")[1] << "\n";
    out << "Жиры: " << fatsSumLabel->text().split(": ")[1] << "\n";
    out << "Углеводы: " << carbsSumLabel->text().split(": ")[1] << "\n";

    // Сохраняем также данные о тренировках и питании
    // Сохранение информации о тренировках и пищи будет реализовано в других функциях сохранения

    file.close();

    QMessageBox::information(this, "Успех", "Данные успешно сохранены.");
}

void DayWindow::setupDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.sqlite");

    if (!db.open())
    {
        QMessageBox::critical(this, "Ошибка", "Не удалось открыть базу данных!");
        return;
    }

    QSqlQuery query;
    // Создаём таблицу для упражнений
    query.exec("CREATE TABLE IF NOT EXISTS training ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "date TEXT, "
               "exercise TEXT)");

    // Создаём таблицу для питания
    query.exec("CREATE TABLE IF NOT EXISTS nutrition ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "date TEXT, "
               "name TEXT, "
               "calories INTEGER, "
               "proteins REAL, "
               "fats REAL, "
               "carbs REAL)");

    // Таблица для веса
    query.exec("CREATE TABLE IF NOT EXISTS weight ("
               "date TEXT PRIMARY KEY, "
               "weight REAL)");
}

void DayWindow::calculateDailySummary()
{
    // Рассчитываем итоговые значения по калориям и макроэлементам
    double totalCalories = 0.0;
    double totalProteins = 0.0;
    double totalFats = 0.0;
    double totalCarbs = 0.0;

    // Считаем итоги на основе данных таблицы
    for (int row = 0; row < secondaryFoodTable->rowCount(); ++row)
    {
        totalCalories += secondaryFoodTable->item(row, 1)->text().toDouble();
        totalProteins += secondaryFoodTable->item(row, 2)->text().toDouble();
        totalFats += secondaryFoodTable->item(row, 3)->text().toDouble();
        totalCarbs += secondaryFoodTable->item(row, 4)->text().toDouble();
    }

    caloriesSumLabel->setText(QString("Калории: %1").arg(totalCalories));
    proteinsSumLabel->setText(QString("Белки: %1 г").arg(totalProteins));
    fatsSumLabel->setText(QString("Жиры: %1 г").arg(totalFats));
    carbsSumLabel->setText(QString("Углеводы: %1 г").arg(totalCarbs));
}

void DayWindow::updateSummaryTab()
{
    if (tabWidget->currentIndex() == 2)
    { // Вкладка "Итог дня"
        calculateDailySummary();
    }
}

void DayWindow::adjustTabWidths()
{
    // Получаем доступ к QTabBar
    QTabBar *tabBar = tabWidget->tabBar();

    // Устанавливаем политику размера
    tabBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Используем QFontMetrics для расчёта ширины текста
    QFontMetrics metrics(tabBar->font());
    for (int i = 0; i < tabBar->count(); ++i)
    {
        QString tabText = tabBar->tabText(i);
        int textWidth = metrics.horizontalAdvance(tabText) * 8; // ширина текста + отступы
        tabBar->setTabData(i, QSize(textWidth, 30));            // Устанавливаем размер
        tabBar->setMinimumWidth(textWidth);                     // Минимальная ширина
    }
}

void DayWindow::removeSelectedReadyMeal()
{
    // Проверяем, выбрана ли строка
    QList<QTableWidgetItem *> selectedItems = readyMealsTable->selectedItems();
    if (selectedItems.isEmpty())
    {
        QMessageBox::warning(this, "Ошибка", "Выберите блюдо для удаления.");
        return;
    }

    // Получаем название блюда из первой колонки
    QString mealName = selectedItems[0]->text();

    // Подтверждение удаления
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Удаление блюда", "Вы уверены, что хотите удалить выбранное блюдо?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No)
    {
        return;
    }

    // Удаляем из базы данных
    QSqlQuery query;
    query.prepare("DELETE FROM ready_meals WHERE name = :name");
    query.bindValue(":name", mealName);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Ошибка базы данных", "Не удалось удалить блюдо: " + query.lastError().text());
        return;
    }

    // Удаляем строку из таблицы
    int row = readyMealsTable->currentRow();
    readyMealsTable->removeRow(row);
}
