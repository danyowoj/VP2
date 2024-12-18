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

    // Инициализация базы данных
   if (!openDatabase()) {
       QMessageBox::critical(this, "Ошибка", "Не удалось подключиться к базе данных.");
       return;
   }

   // Устанавливаем иконку приложения
   QIcon appIcon("C:/Users/latsu/GitHub_projects/VP2/CalendarToDoApp/icons/app.png");
   this->setWindowIcon(appIcon);

   // Задаём минимальные и максимальные размеры окна
   setMinimumSize(670, 450);

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

    loadDesiredWeight();
    adjustTabWidths();
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
    calendarLayout->setContentsMargins(5, 5, 20, 5);

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
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM training WHERE date = :date");
    query.bindValue(":date", date);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    query.next();
    return query.value(0).toInt() > 0;
}

bool CalendarWindow::hasFoodData(const QString &date) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM nutrition WHERE date = :date");
    query.bindValue(":date", date);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    query.next();
    return query.value(0).toInt() > 0;
}

bool CalendarWindow::hasWeightData(const QString &date) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM weight WHERE date = :date");
    query.bindValue(":date", date);
    if (!query.exec()) {
        qDebug() << query.lastError();
        return false;
    }
    query.next();
    return query.value(0).toInt() > 0;
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
    axisY->setRange(0, 200); // Изначально ось Y установлена на 0-100 кг
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
    desiredWeightSeries->clear();  // Clear desired weight series

    int selectedYear = yearComboBox->currentData().toInt();
    int selectedMonth = monthComboBox->currentData().toInt();

    QSqlQuery query;
    query.prepare("SELECT date, weight FROM weight WHERE strftime('%Y', date) = :year AND strftime('%m', date) = :month ORDER BY date ASC");  // Добавлено сортировка по дате
    query.bindValue(":year", QString::number(selectedYear));
    query.bindValue(":month", QString::number(selectedMonth, 10).rightJustified(2, '0'));
    if (!query.exec()) {
        qDebug() << query.lastError();
        return;
    }

    double minWeight = std::numeric_limits<double>::max();
    double maxWeight = std::numeric_limits<double>::min();
    while (query.next()) {
        QDate date = QDate::fromString(query.value(0).toString(), "yyyy-MM-dd");
        double weight = query.value(1).toDouble();
        weightSeries->append(date.day(), weight);
        minWeight = std::min(minWeight, weight);
        maxWeight = std::max(maxWeight, weight);
    }

    // Добавляем желаемый вес
    double desiredWeight = desiredWeightInput->value();
    desiredWeightSeries->append(1, desiredWeight);
    desiredWeightSeries->append(QDate(selectedYear, selectedMonth, 1).daysInMonth(), desiredWeight);

    // Обновляем ось Y с динамическим диапазоном
    QValueAxis *axisY = static_cast<QValueAxis *>(chartView->chart()->axisY(weightSeries));
    axisY->setRange(std::min(minWeight, desiredWeight) - 2, std::max(maxWeight, desiredWeight) + 2);

    // Обновляем ось X, чтобы она отражала дни месяца
    QValueAxis *axisX = static_cast<QValueAxis *>(chartView->chart()->axisX(weightSeries));
    int daysInMonth = QDate(selectedYear, selectedMonth, 1).daysInMonth();
    axisX->setRange(1, daysInMonth);
}

bool CalendarWindow::openDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data.sqlite");

    if (!db.open()) {
        qWarning() << "Не удалось подключиться к базе данных:" << db.lastError().text();
        return false;
    }

    // Создаем таблицу если она не существует
    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS weight ("
               "date TEXT PRIMARY KEY, "
               "weight REAL, "
               "dream_weight REAL)");

    return true;
}

void CalendarWindow::loadLastWeight() {
    QSqlQuery query;
    query.prepare("SELECT weight FROM weight ORDER BY date DESC LIMIT 1");

    if (query.exec() && query.next()) {
        double lastWeight = query.value(0).toDouble();
        weightInput->setValue(lastWeight); // Устанавливаем последний вес как значение по умолчанию для поля weight
    } else {
        weightInput->setValue(0); // Если данных нет, устанавливаем значение по умолчанию (например, 0)
    }
}

void CalendarWindow::onDesiredWeightChanged() {
    saveDesiredWeight();  // Сохраняем желаемый вес
    updateWeightChart();   // Обновляем график
}

void CalendarWindow::saveDesiredWeight() {
    double desiredWeight = desiredWeightInput->value();

    // Проверяем, если желаемый вес равен 0, не сохраняем его
    if (desiredWeight == 0.0) {
        QMessageBox::warning(this, "Ошибка", "Вес не может быть равен нулю.");
        return; // Прерываем выполнение, чтобы не добавить запись в базу данных
    }

    // Получаем последний вес из базы данных
    double lastWeight = 0.0;
    QSqlQuery query;
    query.prepare("SELECT weight FROM weight ORDER BY date DESC LIMIT 1");
    if (query.exec() && query.next()) {
        lastWeight = query.value(0).toDouble(); // Последний вес
    }

    // Вставляем желаемый вес и последний вес в таблицу
    query.prepare("INSERT OR REPLACE INTO weight (date, dream_weight, weight) VALUES (?, ?, ?)");
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));
    query.addBindValue(desiredWeight);
    query.addBindValue(lastWeight); // Вставляем последний вес

    if (!query.exec()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось сохранить желаемый вес.");
    }
}

void CalendarWindow::loadDesiredWeight() {
    QSqlQuery query;
    query.prepare("SELECT dream_weight FROM weight WHERE date = ?");
    query.addBindValue(QDate::currentDate().toString("yyyy-MM-dd"));

    if (query.exec() && query.next()) {
        double savedWeight = query.value(0).toDouble();
        desiredWeightInput->setValue(savedWeight); // Устанавливаем сохраненный желаемый вес
    }
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
