#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QComboBox>
#include "daywindow.h"

class CalendarWindow : public QMainWindow
{
    Q_OBJECT

public:
    CalendarWindow();

private:
    QTableWidget *calendarTable;
    QComboBox *monthComboBox;
    QComboBox *yearComboBox;

    void setupCalendar(int year, int month);
    void createHeader();

private slots:
    void updateCalendar();
};

#endif // CALENDARWINDOW_H