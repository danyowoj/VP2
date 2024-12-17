#ifndef CALENDARWINDOW_H
#define CALENDARWINDOW_H

#include <QWidget>
#include <QDate>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include "daywindow.h"
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

// Включаем необходимые библиотеки
QT_CHARTS_USE_NAMESPACE

class CalendarWindow : public QWidget {
    Q_OBJECT

public:
    explicit CalendarWindow(QWidget *parent = nullptr);

private:
    QTabWidget *tabWidget;

    QWidget *calendarTab;
    QVBoxLayout *mainLayout;
    QGridLayout *calendarLayout;
    QComboBox *monthComboBox;
    QComboBox *yearComboBox;
    QPushButton *updateButton;
    QDate currentDate;

    // Вкладка прогресса
    QWidget *progressTab;
    QChartView *chartView;
    QLineSeries *weightSeries;
    QLineSeries *desiredWeightSeries;
    QDoubleSpinBox *desiredWeightInput;
    QLabel *desiredWeightLabel;

    void createCalendar();
    void createProgressTab();
    void initWeightChart();
    void updateWeightChart();
    void loadProgressData();
    void loadDesiredWeight();
    void saveDesiredWeight();

    void clearCalendar();
    void adjustCalendarCellSizes();
    void openDayWindow(int day);
    void showEvent(QShowEvent *event) override;

    void adjustTabWidths();

    bool hasTrainingData(const QString &date);
    bool hasFoodData(const QString &date);
    bool hasWeightData(const QString &date);

private slots:
    void updateCalendar();
    void onDesiredWeightChanged();

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // CALENDARWINDOW_H
