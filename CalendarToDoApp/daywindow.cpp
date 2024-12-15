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
    : QWidget(parent), date(date) {
    setWindowTitle(QString("День: %1").arg(date));
    resize(800, 600);

    tabWidget = new QTabWidget(this);

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
    weightInput->setDecimals(1); // 1 знак после запятой
    weightInput->setPrefix("Вес: ");
    weightInput->setValue(0.0);

    summaryLayout->addWidget(caloriesSumLabel);
    summaryLayout->addWidget(proteinsSumLabel);
    summaryLayout->addWidget(fatsSumLabel);
    summaryLayout->addWidget(carbsSumLabel);
    summaryLayout->addWidget(weightInput);

    tabWidget->addTab(summaryTab, "Итог дня");

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

    // Загрузка данных о продуктах
    loadFoodData();
    loadDayData(); // Загружаем данные
}

DayWindow::~DayWindow() {
    saveDayData();
}

void DayWindow::addTaskToPrimary() {
    QString task = primaryTaskInput->text().trimmed();
    if (!task.isEmpty()) {
        primaryTaskList->addItem(task);
        primaryTaskInput->clear();
    }
}

void DayWindow::removeSelectedTaskFromPrimary() {
    QListWidgetItem *selectedItem = primaryTaskList->currentItem();
    if (selectedItem) {
        delete selectedItem;
    }
}

void DayWindow::loadFoodData() {
    QFile file("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/foods.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл foods.txt");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList parts = line.split(';');
            if (parts.size() == 5) { // Проверяем наличие всех полей
                QString name = parts[0];
                int calories = parts[1].toInt();
                float proteins = parts[2].toFloat();
                float fats = parts[3].toFloat();
                float carbs = parts[4].toFloat();

                foodData[name] = {calories, proteins, fats, carbs};
            }
        }
    file.close();
}

void DayWindow::updateFoodTable() {
    secondaryFoodTable->setRowCount(0); // Очищаем таблицу

    for (int row = 0; row < secondaryFoodTable->rowCount(); ++row) {
            secondaryFoodTable->removeRow(row);
        }

    int row = 0;
        for (const auto &foodName : foodData.keys()) {
            const FoodInfo &info = foodData[foodName];

            secondaryFoodTable->insertRow(row);
            secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(foodName));
            secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(info.calories)));
            secondaryFoodTable->setItem(row, 2, new QTableWidgetItem(QString::number(info.proteins)));
            secondaryFoodTable->setItem(row, 3, new QTableWidgetItem(QString::number(info.fats)));
            secondaryFoodTable->setItem(row, 4, new QTableWidgetItem(QString::number(info.carbs)));

            row++;
        }
}

void DayWindow::addFoodToTable() {
    QString foodName = secondaryFoodInput->text().trimmed();
        if (foodName.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Введите название блюда в поле ввода.");
            return;
        }

        if (foodData.contains(foodName)) {
            // Если блюдо уже есть в foodData, добавляем его в таблицу
            const FoodInfo &info = foodData[foodName];

            int row = secondaryFoodTable->rowCount();
            secondaryFoodTable->insertRow(row);

            secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(foodName));
            secondaryFoodTable->setItem(row, 1, new QTableWidgetItem(QString::number(info.calories)));
            secondaryFoodTable->setItem(row, 2, new QTableWidgetItem(QString::number(info.proteins)));
            secondaryFoodTable->setItem(row, 3, new QTableWidgetItem(QString::number(info.fats)));
            secondaryFoodTable->setItem(row, 4, new QTableWidgetItem(QString::number(info.carbs)));
        } else {
            // Если блюдо не найдено, добавляем его с пустыми данными
            int row = secondaryFoodTable->rowCount();
            secondaryFoodTable->insertRow(row);

            secondaryFoodTable->setItem(row, 0, new QTableWidgetItem(foodName));
            secondaryFoodTable->setItem(row, 1, new QTableWidgetItem("0")); // Калории
            secondaryFoodTable->setItem(row, 2, new QTableWidgetItem("0")); // Белки
            secondaryFoodTable->setItem(row, 3, new QTableWidgetItem("0")); // Жиры
            secondaryFoodTable->setItem(row, 4, new QTableWidgetItem("0")); // Углеводы
        }

        secondaryFoodInput->clear(); // Очищаем поле ввода
}

void DayWindow::removeSelectedFood() {
    auto selectedItems = secondaryFoodTable->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Выберите блюдо для удаления.");
        return;
    }

    secondaryFoodTable->removeRow(selectedItems[0]->row());
}

void DayWindow::backToCalendar() {
    emit backToCalendarRequested();
}

void DayWindow::setupFoodTable() {
    // Устанавливаем политику растягивания колонок
    QHeaderView *header = secondaryFoodTable->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Stretch); // Колонка с названием блюда занимает оставшееся место
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Калории
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Белки
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Жиры
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Углеводы

    // Запрещаем пользователю менять размер колонок вручную
    header->setSectionsMovable(false);

    // Минимальная высота строки для лучшего отображения
    secondaryFoodTable->verticalHeader()->setDefaultSectionSize(25);
}

void DayWindow::saveDayData() {
    // Проверяем, есть ли данные о тренировке, питании и весе
    bool hasTrainingData = primaryTaskList->count() > 0; // Есть ли задачи в тренировке
    bool hasFoodData = secondaryFoodTable->rowCount() > 0; // Есть ли записи в таблице питания
    bool hasWeightData = weightInput->value() > 0.0; // Есть ли вес, не равный 0

    // Если нет данных по тренировке, питанию или весу, не сохраняем
    if (!hasTrainingData && !hasFoodData && !hasWeightData) {
        return; // Не сохраняем данные, если все пусто или вес равен 0
    }

    QDir dir;
    if (!dir.exists("data")) {
        dir.mkpath("data");
    }

    QString filePath = QString("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/%1.txt").arg(date);

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Не удалось сохранить данные для дня";
        return;
    }

    QTextStream out(&file);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if (codec) {
        out.setCodec(codec);
    }
#endif

    // Сохраняем задачи тренировки
    if (hasTrainingData) {
        out << "[Training]\n";
        for (int i = 0; i < primaryTaskList->count(); ++i) {
            out << primaryTaskList->item(i)->text() << "\n";
        }
    }

    // Сохраняем данные питания
    if (hasFoodData) {
        out << "[Nutrition]\n";
        for (int row = 0; row < secondaryFoodTable->rowCount(); ++row) {
            QString foodName = secondaryFoodTable->item(row, 0)->text();
            QString calories = secondaryFoodTable->item(row, 1)->text();
            QString proteins = secondaryFoodTable->item(row, 2)->text();
            QString fats = secondaryFoodTable->item(row, 3)->text();
            QString carbs = secondaryFoodTable->item(row, 4)->text();
            out << QString("%1;%2;%3;%4;%5\n").arg(foodName, calories, proteins, fats, carbs);
        }
    }

    // Сохраняем данные о весе, если вес больше 0
    if (hasWeightData) {
        double weight = weightInput->value();
        out << "[Weight]\n" << weight << "\n";
    }

    file.close();
}

void DayWindow::loadDayData() {
    QString filePath = QString("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/%1.txt").arg(date);

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Не удалось загрузить данные для дня";
        return;
    }

    QTextStream in(&file);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    if (codec) {
        in.setCodec(codec);
    }
#endif

    QString currentSection;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty())
            continue;

        if (line == "[Training]") {
            currentSection = "Training";
            continue;
        } else if (line == "[Nutrition]") {
            currentSection = "Nutrition";
            continue;
        } else if (line == "[Weight]") {
            currentSection = "Weight";
            continue;
        }

        if (currentSection == "Training") {
            primaryTaskList->addItem(line);
        } else if (currentSection == "Nutrition") {
            QStringList parts = line.split(';');
            if (parts.size() == 5) {
                int row = secondaryFoodTable->rowCount();
                secondaryFoodTable->insertRow(row);
                for (int col = 0; col < parts.size(); ++col) {
                    secondaryFoodTable->setItem(row, col, new QTableWidgetItem(parts[col]));
                }
            }
        } else if (currentSection == "Weight") {
            bool ok;
            double savedWeight = line.toDouble(&ok);
            if (ok) {
                weightInput->setValue(savedWeight);  // Устанавливаем сохраненный вес
            }
        }
    }

    calculateDailySummary();

    file.close();
}


void DayWindow::saveSummaryData() {
    double weight = weightInput->value();
    QString weightStr = QString::number(weight, 'f', 1);

    // Сохраняем информацию о весе и макросах
    QFile file(QString("data/%1_summary.txt").arg(date));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
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

void DayWindow::calculateDailySummary() {
    // Рассчитываем итоговые значения по калориям и макроэлементам
    double totalCalories = 0.0;
    double totalProteins = 0.0;
    double totalFats = 0.0;
    double totalCarbs = 0.0;

    // Считаем итоги на основе данных таблицы
    for (int row = 0; row < secondaryFoodTable->rowCount(); ++row) {
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

void DayWindow::updateSummaryTab() {
    if (tabWidget->currentIndex() == 2) {  // Вкладка "Итог дня"
        calculateDailySummary();
    }
}
