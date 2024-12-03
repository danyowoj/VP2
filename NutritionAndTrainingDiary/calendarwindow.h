#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QVBoxLayout>
#include <QDate>
#include "daywindow.h"
#include <QPointer> // Для безопасного указателя
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class CalendarWindow : public QWidget {
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);

private slots:
    void updateCalendar();
    void openDayWindow(int day);

private:
    QVBoxLayout *mainLayout;
    QGridLayout *calendarLayout;
    QComboBox *monthComboBox;
    QComboBox *yearComboBox;
    QPushButton *updateButton;
    QDate currentDate;
    QPointer<DayWindow> currentDayWindow; // Указатель на текущее окно дня
    QSqlDatabase db;

    void createCalendar();
    void clearCalendar();
};

#endif // CALENDARWINDOW_H
