#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QWidget>
#include <QDate>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include "daywindow.h"

class CalendarWindow : public QWidget {
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);

private:
    QVBoxLayout *mainLayout;
    QGridLayout *calendarLayout;
    QComboBox *monthComboBox;
    QComboBox *yearComboBox;
    QPushButton *updateButton;
    QDate currentDate;

    void createCalendar();
    void clearCalendar();
    void adjustCalendarCellSizes();
    void openDayWindow(int day);
    void showEvent(QShowEvent *event) override;

    void applyTemplateToDay(const QString &date);

private slots:
    void updateCalendar();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CALENDARWINDOW_H
