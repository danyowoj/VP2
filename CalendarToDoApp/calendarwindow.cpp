#include "calendarwindow.h"
#include <QResizeEvent>
#include <QTextStream>
#include <QFormLayout>
#include <QDir>
#include <QMessageBox>
#include <QDebug>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QWidget(parent), currentDate(QDate::currentDate()) {
    setWindowTitle("Журнал питания и тренировок");

    // Задаём минимальные и максимальные размеры окна
    setMinimumSize(640, 360);

    resize(960, 720);

    // Загружаем стили из файла styles.qss
    QFile styleFile("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/styles.qss");
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&styleFile);
        QString styleSheet = stream.readAll();
        this->setStyleSheet(styleSheet);
        styleFile.close();
    } else {
        qDebug() << "Не удалось загрузить файл стилей.";
    }

    // Основной макет
    mainLayout = new QVBoxLayout(this);

    // Создаём виджет вкладок
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // Создаём вкладку календаря
    calendarTab = new QWidget(this);
    QVBoxLayout *calendarLayoutWrapper = new QVBoxLayout(calendarTab);

    // Элементы выбора месяца и года
    QHBoxLayout *controlsLayout = new QHBoxLayout();
    monthComboBox = new QComboBox(this);
    yearComboBox = new QComboBox(this);
    updateButton = new QPushButton("Обновить", this);

    // Заполняем месяцы
    for (int i = 1; i <= 12; ++i) {
        monthComboBox->addItem(QDate::longMonthName(i), i);
    }
    monthComboBox->setCurrentIndex(currentDate.month() - 1);

    // Заполняем годы
    for (int year = 2000; year <= 2100; ++year) {
        yearComboBox->addItem(QString::number(year), year);
    }
    yearComboBox->setCurrentText(QString::number(currentDate.year()));

    // Добавляем элементы в макет
    controlsLayout->addWidget(monthComboBox);
    controlsLayout->addWidget(yearComboBox);
    controlsLayout->addWidget(updateButton);
    mainLayout->addLayout(controlsLayout);

    // Макет календаря
    calendarLayout = new QGridLayout();
    calendarLayoutWrapper->addLayout(calendarLayout);

    // Обновляем график и календарь при изменении месяца или года
    connect(monthComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::updateCalendar);
    connect(yearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::updateCalendar);

    // Добавляем вкладку календаря в QTabWidget
    tabWidget->addTab(calendarTab, "Календарь");

    // Создаем календарь
    createCalendar();

    // Подключаем сигнал обновления
    connect(updateButton, &QPushButton::clicked, this, &CalendarWindow::updateCalendar);
    connect(this, &CalendarWindow::resizeEvent, this, &CalendarWindow::adjustCalendarCellSizes);

    // Принудительное обновление размера при запуске
    adjustCalendarCellSizes();

    // Создаём вкладку прогресса
    createProgressTab();
    connect(desiredWeightInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &CalendarWindow::onDesiredWeightChanged);

    adjustTabWidths();

    loadDesiredWeight();
}

void CalendarWindow::createCalendar() {
    // Получаем выбранные месяц и год
    int month = monthComboBox->currentData().toInt();
    int year = yearComboBox->currentData().toInt();
    QDate firstDayOfMonth(year, month, 1);

    // Определяем количество дней и день недели первого числа
    int daysInMonth = firstDayOfMonth.daysInMonth();
    int startDayOfWeek = firstDayOfMonth.dayOfWeek();

    // Удаляем все предыдущие виджеты, чтобы избежать наложения
    clearCalendar();

    // Настройка сетки
    for (int i = 0; i < 7; ++i) {
        calendarLayout->setColumnStretch(i, 1); // Равномерное распределение ширины
    }
    for (int i = 0; i < 6; ++i) {
        calendarLayout->setRowStretch(i, 1); // Равномерное распределение высоты
    }
    calendarLayout->setSpacing(5);
    calendarLayout->setContentsMargins(5, 5, 20, 5); // Отступы от краев макета

    // Создаем кнопки для каждого дня
    for (int day = 1; day <= daysInMonth; ++day) {
        int row = (startDayOfWeek + day - 2) / 7; // Определяем строку
        int col = (startDayOfWeek + day - 2) % 7; // Определяем колонку

        QPushButton *dayButton = new QPushButton(QString::number(day), this);

        // Устанавливаем минимальные размеры кнопок
        dayButton->setMinimumSize(20, 20);

        // Политика размера: кнопки будут расширяться
        dayButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Проверяем наличие данных о тренировке и питании
        QString date = QString("%1-%2-%3")
                           .arg(year, 4, 10, QChar('0'))
                           .arg(month, 2, 10, QChar('0'))
                           .arg(day, 2, 10, QChar('0'));

        bool T = hasTrainingData(date);
        bool F = hasFoodData(date);
        bool W = hasWeightData(date);

        if ((T + F + W) == 1) {
            dayButton->setStyleSheet("background-color: #BD613C;"); // Есть только одна запись
        } else if ((T + F + W) == 2) {
            dayButton->setStyleSheet("background-color: #BCAF4D;"); // Есть две записи
        } else if ((T + F + W) == 3) {
            dayButton->setStyleSheet("background-color: #6D8C00;"); //Есть все три записи
        } else {
            dayButton->setStyleSheet("background-color: #AA1803;"); // Данных нет
        }

        // Добавляем кнопку в сетку
        calendarLayout->addWidget(dayButton, row, col);

        // Подключаем сигнал нажатия
        connect(dayButton, &QPushButton::clicked, this, [this, day]() {
            openDayWindow(day);
        });
    }
}

bool CalendarWindow::hasTrainingData(const QString &date) {
    QString filePath = QString("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/%1.txt").arg(date);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    bool foundTrainingSection = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line == "[Training]") {
            foundTrainingSection = true;
            continue;
        }

        if (foundTrainingSection) {
            if (!line.isEmpty() && line != "[Nutrition]") {
                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    return false;
}

bool CalendarWindow::hasFoodData(const QString &date) {
    QString filePath = QString("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/%1.txt").arg(date);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    bool foundNutritionSection = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line == "[Nutrition]") {
            foundNutritionSection = true;
            continue;
        }

        if (foundNutritionSection) {
            if (!line.isEmpty() && line != "[Weight]") {
                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    return false;
}

bool CalendarWindow::hasWeightData(const QString &date) {
    QString filePath = QString("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/%1.txt").arg(date);
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream in(&file);
    bool foundNutritionSection = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        if (line == "[Weight]") {
            foundNutritionSection = true;
            continue;
        }

        if (foundNutritionSection) {
            if (line != "0") {
                file.close();
                return true;
            }
            break;
        }
    }

    file.close();
    return false;
}

void CalendarWindow::clearCalendar() {
    while (QLayoutItem *item = calendarLayout->takeAt(0)) {
        delete item->widget();
        delete item;
    }
}

void CalendarWindow::updateCalendar() {
    clearCalendar();
    createCalendar();
    adjustCalendarCellSizes();
    updateWeightChart();
}

void CalendarWindow::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    // Обновляем календарь только при первом показе окна
    static bool firstShow = true;
    if (firstShow) {
        updateCalendar();
        firstShow = false;
    }
}

void CalendarWindow::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event); // Обрабатываем стандартное событие
    updateCalendar();            // Перестраиваем календарь
}

void CalendarWindow::adjustCalendarCellSizes() {
    int cellWidth = this->width() / 7;  // 7 дней в неделе
    int cellHeight = this->height() / 7;

    for (int i = 0; i < calendarLayout->count(); ++i) {
        QLayoutItem *item = calendarLayout->itemAt(i);
        if (item && item->widget()) {
            item->widget()->setFixedSize(cellWidth, cellHeight);
        }
    }
}

void CalendarWindow::openDayWindow(int day) {
    QString date = QString("%1-%2-%3")
                       .arg(yearComboBox->currentText())
                       .arg(monthComboBox->currentIndex() + 1, 2, 10, QChar('0'))
                       .arg(day, 2, 10, QChar('0'));

    auto *dayWindow = new DayWindow(date);
    connect(dayWindow, &DayWindow::backToCalendarRequested, this, [this, dayWindow]() {
            dayWindow->close();
            this->show(); // Показываем окно календаря
            dayWindow->deleteLater();
        });
        dayWindow->show();
        this->hide();

    connect(dayWindow, &DayWindow::backToCalendar, this, [this, dayWindow]() {
        dayWindow->close();
        this->show();
        delete dayWindow;
    });
}

void CalendarWindow::createProgressTab() {
    progressTab = new QWidget(this);
    QVBoxLayout *progressLayout = new QVBoxLayout(progressTab);

    // Поле для ввода желаемого веса
    QHBoxLayout *weightInputLayout = new QHBoxLayout();
    desiredWeightLabel = new QLabel("Желаемый вес (кг):", this);
    desiredWeightInput = new QDoubleSpinBox(this);
    desiredWeightInput->setRange(0.0, 200.0); // Диапазон веса
    desiredWeightInput->setDecimals(1);       // Один знак после запятой
    desiredWeightInput->setValue(0.0);       // Значение по умолчанию

    weightInputLayout->addWidget(desiredWeightLabel);
    weightInputLayout->addWidget(desiredWeightInput);
    progressLayout->addLayout(weightInputLayout);

    // Инициализируем график
    initWeightChart();
    progressLayout->addWidget(chartView);

    // Обновляем график при изменении желаемого веса
    connect(desiredWeightInput, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &CalendarWindow::onDesiredWeightChanged);

    tabWidget->addTab(progressTab, "Прогресс");

    // Загружаем данные о прогрессе
    loadProgressData();
}

void CalendarWindow::initWeightChart() {
    // Инициализация графика
    QChart *chart = new QChart();
    chart->setTitle("Динамика изменения веса");
    chart->setTitleBrush(QBrush(QColor("#AA1803"))); // Цвет заголовка
    chart->setBackgroundBrush(QBrush(QColor("#F1BAA1"))); // Цвет фона графика
    chart->setDropShadowEnabled(false); // Убираем тень

    // Создаем серию для фактического веса
    weightSeries = new QLineSeries();
    weightSeries->setName("Фактический вес");
    weightSeries->setColor(QColor("#AA1803")); // Цвет линии для фактического веса
    weightSeries->setPen(QPen(QColor("#AA1803"), 2)); // Толщина и цвет линии

    // Создаем серию для желаемого веса
    desiredWeightSeries = new QLineSeries();
    desiredWeightSeries->setName("Желаемый вес");
    desiredWeightSeries->setColor(QColor("#6D8C00")); // Цвет линии для желаемого веса
    desiredWeightSeries->setPen(QPen(QColor("#6D8C00"), 2, Qt::DashLine)); // Пунктирная линия

    // Добавляем серии на график
    chart->addSeries(weightSeries);
    chart->addSeries(desiredWeightSeries);

    // Создаем ось X
    QValueAxis *axisX = new QValueAxis();
    axisX->setTitleText("День месяца");
    axisX->setLabelsColor(QColor("#6D8C00")); // Цвет подписей оси X
    axisX->setLinePenColor(QColor("#BD613C")); // Цвет линии оси X
    axisX->setGridLineColor(QColor("#BCAF4D")); // Цвет сетки оси X
    axisX->setLabelFormat("%d");
    chart->addAxis(axisX, Qt::AlignBottom);

    // Создаем ось Y
    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Вес (кг)");
    axisY->setLabelsColor(QColor("#6D8C00")); // Цвет подписей оси Y
    axisY->setLinePenColor(QColor("#BD613C")); // Цвет линии оси Y
    axisY->setGridLineColor(QColor("#BCAF4D")); // Цвет сетки оси Y
    axisY->setLabelFormat("%.1f");
    axisY->setRange(0, 100); // Изначально ось Y установлена на 0-100 кг
    chart->addAxis(axisY, Qt::AlignLeft);

    // Привязываем оси к сериям
    weightSeries->attachAxis(axisX);
    weightSeries->attachAxis(axisY);
    desiredWeightSeries->attachAxis(axisX);
    desiredWeightSeries->attachAxis(axisY);

    // Стилизация легенды
    chart->legend()->setVisible(true);
    chart->legend()->setLabelBrush(QBrush(QColor("#6D8C00"))); // Цвет текста в легенде
    chart->legend()->setBrush(QBrush(QColor("#F1BAA1"))); // Фон легенды
    chart->legend()->setPen(QPen(QColor("#BD613C"))); // Рамка легенды

    // Создаем QChartView и устанавливаем график
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing); // Сглаживание линий
    chartView->setStyleSheet("border: 2px solid #BD613C; border-radius: 8px;"); // Стили рамки

    // Размещение графика в окне
    chartView->setGeometry(20, 20, 760, 400);
    chartView->setParent(this);

    // Загрузка данных для графика
    updateWeightChart();
}


void CalendarWindow::updateWeightChart() {
    weightSeries->clear();
    desiredWeightSeries->clear();  // Очищаем серию для желаемого веса

    int selectedYear = yearComboBox->currentData().toInt();
    int selectedMonth = monthComboBox->currentData().toInt();

    QFileInfoList files = QDir("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data").entryInfoList({"*.txt"}, QDir::Files);

    int day = 1;
    double minWeight = std::numeric_limits<double>::max();
    double maxWeight = std::numeric_limits<double>::min();

    for (const QFileInfo &fileInfo : files) {
        QString fileName = fileInfo.baseName(); // Извлекаем имя файла без расширения
        QDate fileDate = QDate::fromString(fileName, "yyyy-MM-dd");

        if (!fileDate.isValid() || fileDate.year() != selectedYear || fileDate.month() != selectedMonth) {
            continue; // Пропускаем файлы, которые не относятся к текущему месяцу
        }

        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }

        QTextStream in(&file);
        QString currentSection;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line == "[Weight]") {
                currentSection = "Weight";
                continue;
            }

            if (currentSection == "Weight") {
                bool ok;
                double weight = line.toDouble(&ok);
                if (ok) {
                    weightSeries->append(fileDate.day(), weight);
                    minWeight = std::min(minWeight, weight);
                    maxWeight = std::max(maxWeight, weight);
                }
            }
        }
        file.close();
    }

    // Добавляем серию для желаемого веса
    if (desiredWeightInput) {
        double desiredWeight = desiredWeightInput->value();
        desiredWeightSeries->append(1, desiredWeight);
        desiredWeightSeries->append(QDate(selectedYear, selectedMonth, 1).daysInMonth(), desiredWeight);
    }

    // Обновляем ось Y с динамическим диапазоном, включая желаемый вес
    QValueAxis *axisY = static_cast<QValueAxis *>(chartView->chart()->axisY(weightSeries));
    axisY->setRange(std::min(minWeight, desiredWeightInput->value()) - 2, std::max(maxWeight, desiredWeightInput->value()) + 2);

    // Обновляем ось X с днями месяца
    QValueAxis *axisX = static_cast<QValueAxis *>(chartView->chart()->axisX(weightSeries));
    int daysInMonth = QDate(selectedYear, selectedMonth, 1).daysInMonth();
    axisX->setRange(1, daysInMonth);
}

void CalendarWindow::loadProgressData() {
    weightSeries->clear();

    // Очищаем данные о весе
    QFileInfoList files = QDir("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data").entryInfoList({"*.txt"}, QDir::Files);

    int day = 1;
    double minWeight = std::numeric_limits<double>::max();
    double maxWeight = std::numeric_limits<double>::min();

    for (const QFileInfo &fileInfo : files) {
        QFile file(fileInfo.absoluteFilePath());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            continue;
        }

        QTextStream in(&file);
        QString currentSection;

        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line == "[Weight]") {
                currentSection = "Weight";
                continue;
            }

            if (currentSection == "Weight") {
                bool ok;
                double weight = line.toDouble(&ok);
                if (ok) {
                    weightSeries->append(day++, weight);
                    minWeight = std::min(minWeight, weight);
                    maxWeight = std::max(maxWeight, weight);
                }
            }
        }
        file.close();
    }

    // Обновляем ось Y с динамическим диапазоном
    QValueAxis *axisY = static_cast<QValueAxis *>(chartView->chart()->axisY(weightSeries));
    axisY->setRange(minWeight - 2, maxWeight + 2); // Добавляем немного пространства сверху и снизу

    // Обновляем ось X с днями месяца
    QValueAxis *axisX = static_cast<QValueAxis *>(chartView->chart()->axisX(weightSeries));
    int daysInMonth = QDate(yearComboBox->currentText().toInt(), monthComboBox->currentIndex() + 1, 1).daysInMonth();
    axisX->setRange(1, daysInMonth);
}

void CalendarWindow::onDesiredWeightChanged() {
    saveDesiredWeight();  // Сохраняем желаемый вес
    updateWeightChart();   // Обновляем график
}

void CalendarWindow::saveDesiredWeight() {
    QFile file("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/desired_weight.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить желаемый вес.");
        return;
    }

    QTextStream out(&file);
    out << desiredWeightInput->value();
    file.close();
}

void CalendarWindow::loadDesiredWeight() {
    QFile file("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/data/desired_weight.txt");
    if (!file.exists()) {
        return;  // Если файл не существует, значит желаемый вес не был установлен
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось загрузить желаемый вес.");
        return;
    }

    QTextStream in(&file);
    double savedWeight = in.readLine().toDouble();
    desiredWeightInput->setValue(savedWeight);  // Устанавливаем сохраненный желаемый вес
    file.close();
}

void CalendarWindow::adjustTabWidths() {
    // Получаем доступ к QTabBar
    QTabBar *tabBar = tabWidget->tabBar();

    // Устанавливаем политику размера
    tabBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    // Используем QFontMetrics для расчёта ширины текста
    QFontMetrics metrics(tabBar->font());
    for (int i = 0; i < tabBar->count(); ++i) {
        QString tabText = tabBar->tabText(i);
        int textWidth = metrics.horizontalAdvance(tabText) * 8; // ширина текста + отступы
        tabBar->setTabData(i, QSize(textWidth, 30));            // Устанавливаем размер
        tabBar->setMinimumWidth(textWidth);                     // Минимальная ширина
    }
}

