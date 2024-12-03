#include "calendarwindow.h"

CalendarWindow::CalendarWindow(QWidget *parent) : QWidget(parent), currentDate(QDate::currentDate()) {
    setWindowTitle("Календарь");
    resize(500, 400);

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

    // Заполняем годы (например, с 2000 по 2100)
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
}

void CalendarWindow::createCalendar() {
    // Получаем выбранные месяц и год
    int month = monthComboBox->currentData().toInt();
    int year = yearComboBox->currentData().toInt();
    QDate firstDayOfMonth(year, month, 1);

    // Определяем количество дней и день недели первого числа
    int daysInMonth = firstDayOfMonth.daysInMonth();
    int startDayOfWeek = firstDayOfMonth.dayOfWeek();

    // Создаем кнопки для каждого дня
    for (int day = 1; day <= daysInMonth; ++day) {
        int row = (startDayOfWeek + day - 2) / 7;
        int col = (startDayOfWeek + day - 2) % 7;

        QPushButton *dayButton = new QPushButton(QString::number(day), this);
        calendarLayout->addWidget(dayButton, row, col);

        connect(dayButton, &QPushButton::clicked, this, [this, day]() {
            openDayWindow(day);
        });
    }
}

void CalendarWindow::clearCalendar() {
    QLayoutItem *child;
    while ((child = calendarLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
}

void CalendarWindow::updateCalendar() {
    clearCalendar();
    createCalendar();
}

void CalendarWindow::openDayWindow(int day) {
    // Создаем новое окно дня
    auto *dayWindow = new DayWindow(day);

    // Показываем окно дня
    dayWindow->show();

    // Скрываем текущее окно календаря
    this->hide();

    // Связываем сигнал возврата из окна дня с возвратом к календарю
    connect(dayWindow, &DayWindow::backToCalendar, this, [this, dayWindow]() {
        dayWindow->close(); // Закрываем окно дня
        this->show();       // Возвращаем окно календаря
        delete dayWindow;   // Удаляем объект окна дня
    });
}
