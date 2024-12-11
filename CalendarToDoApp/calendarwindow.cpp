#include "calendarwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDate>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>

CalendarWindow::CalendarWindow() {
    setWindowTitle("Calendar");
    resize(800, 600);

    // Инициализация центрального виджета и его макета
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    calendarTable = new QTableWidget(6, 7, this);
    calendarTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    calendarTable->horizontalHeader()->setVisible(false);
    calendarTable->verticalHeader()->setVisible(false);
    calendarTable->setGridStyle(Qt::SolidLine);
    calendarTable->setSelectionMode(QAbstractItemView::NoSelection);

    // Создаем выпадающие списки для месяца и года
    monthComboBox = new QComboBox(this);
    yearComboBox = new QComboBox(this);

    for (int month = 1; month <= 12; ++month) {
        monthComboBox->addItem(QDate::longMonthName(month), month);
    }

    int currentYear = QDate::currentDate().year();
    for (int year = currentYear - 50; year <= currentYear + 50; ++year) {
        yearComboBox->addItem(QString::number(year), year);
    }

    monthComboBox->setCurrentIndex(QDate::currentDate().month() - 1);
    yearComboBox->setCurrentText(QString::number(currentYear));

    // Создаем заголовок и настраиваем календарь
    createHeader();
    setupCalendar(currentYear, QDate::currentDate().month());

    mainLayout->addWidget(calendarTable);

    // Подключаем сигналы
    connect(calendarTable, &QTableWidget::cellClicked, this, [=](int row, int column) {
        QTableWidgetItem *item = calendarTable->item(row, column);
        if (item && !item->text().isEmpty()) {
            int day = item->text().toInt();
            int month = monthComboBox->currentData().toInt();
            int year = yearComboBox->currentData().toInt();
            QDate selectedDate(year, month, day);

            DayWindow *dayWindow = new DayWindow(selectedDate, this);
            dayWindow->exec();
        }
    });

    connect(monthComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::updateCalendar);
    connect(yearComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWindow::updateCalendar);
}

void CalendarWindow::createHeader() {
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(centralWidget()->layout());
    if (!mainLayout) {
        mainLayout = new QVBoxLayout;
        centralWidget()->setLayout(mainLayout);
    }

    QHBoxLayout *headerLayout = new QHBoxLayout;

    QPushButton *prevButton = new QPushButton("<<", this);
    QPushButton *nextButton = new QPushButton(">>", this);

    headerLayout->addWidget(prevButton);
    headerLayout->addWidget(monthComboBox);
    headerLayout->addWidget(yearComboBox);
    headerLayout->addWidget(nextButton);

    QWidget *headerWidget = new QWidget(this);
    headerWidget->setLayout(headerLayout);

    mainLayout->insertWidget(0, headerWidget);

    connect(prevButton, &QPushButton::clicked, this, [=]() {
        int currentMonth = monthComboBox->currentIndex();
        int currentYear = yearComboBox->currentText().toInt();

        if (currentMonth == 0) {
            currentMonth = 11;
            currentYear--;
        } else {
            currentMonth--;
        }

        monthComboBox->setCurrentIndex(currentMonth);
        yearComboBox->setCurrentText(QString::number(currentYear));
    });

    connect(nextButton, &QPushButton::clicked, this, [=]() {
        int currentMonth = monthComboBox->currentIndex();
        int currentYear = yearComboBox->currentText().toInt();

        if (currentMonth == 11) {
            currentMonth = 0;
            currentYear++;
        } else {
            currentMonth++;
        }

        monthComboBox->setCurrentIndex(currentMonth);
        yearComboBox->setCurrentText(QString::number(currentYear));
    });
}


void CalendarWindow::setupCalendar(int year, int month) {
    QDate firstDayOfMonth(year, month, 1);
    int startColumn = firstDayOfMonth.dayOfWeek() - 1;
    int daysInMonth = firstDayOfMonth.daysInMonth();

    calendarTable->clearContents();

    int day = 1;
    for (int row = 0; row < 6; ++row) {
        for (int column = 0; column < 7; ++column) {
            if ((row == 0 && column < startColumn) || day > daysInMonth) {
                calendarTable->setItem(row, column, new QTableWidgetItem());
            } else {
                QTableWidgetItem *item = new QTableWidgetItem(QString::number(day));
                item->setTextAlignment(Qt::AlignCenter);
                calendarTable->setItem(row, column, item);
                ++day;
            }
        }
    }

    calendarTable->resizeRowsToContents();
    calendarTable->resizeColumnsToContents();
}

void CalendarWindow::updateCalendar() {
    int year = yearComboBox->currentData().toInt();
    int month = monthComboBox->currentData().toInt();
    setupCalendar(year, month);
}
