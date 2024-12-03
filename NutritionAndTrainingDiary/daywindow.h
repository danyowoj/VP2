#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class DayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DayWindow(int day, QWidget *parent = nullptr);

signals:
    void backToCalendar();

private:
    QVBoxLayout *layout;
    QPushButton *backButton;
};

#endif // DAYWINDOW_H
