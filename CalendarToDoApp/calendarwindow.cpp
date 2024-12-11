#include "calendarwindow.h"
#include <QResizeEvent>

CalendarWindow::CalendarWindow(QWidget *parent)
    : QWidget(parent), currentDate(QDate::currentDate()) {
    setWindowTitle("Календарь");

    // Задаём минимальные и максимальные размеры окна
    setMinimumSize(450, 250);
    setMaximumSize(900, 500);

    resize(450, 250);

    // Основной макет
    mainLayout = new QVBoxLayout(this);

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
    mainLayout->addLayout(calendarLayout);

    // Создаем календарь
    createCalendar();

    // Подключаем сигнал обновления
    connect(updateButton, &QPushButton::clicked, this, &CalendarWindow::updateCalendar);
    connect(this, &CalendarWindow::resizeEvent, this, &CalendarWindow::adjustCalendarCellSizes);

    // Принудительное обновление размера при запуске
    adjustCalendarCellSizes();
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
    calendarLayout->setSpacing(5); // Установка промежутков между кнопками
    calendarLayout->setContentsMargins(5, 5, 5, 5); // Отступы от краев макета

    // Создаем кнопки для каждого дня
    for (int day = 1; day <= daysInMonth; ++day) {
        int row = (startDayOfWeek + day - 2) / 7; // Определяем строку
        int col = (startDayOfWeek + day - 2) % 7; // Определяем колонку

        QPushButton *dayButton = new QPushButton(QString::number(day), this);

        // Устанавливаем минимальные размеры кнопок
        dayButton->setMinimumSize(20, 20);

        // Политика размера: кнопки будут расширяться
        dayButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

        // Добавляем кнопку в сетку
        calendarLayout->addWidget(dayButton, row, col);

        // Подключаем сигнал нажатия
        connect(dayButton, &QPushButton::clicked, this, [this, day]() {
            openDayWindow(day);
        });
    }
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
    dayWindow->show();
    this->hide();

    connect(dayWindow, &DayWindow::backToCalendar, this, [this, dayWindow]() {
        dayWindow->close();
        this->show();
        delete dayWindow;
    });
}
