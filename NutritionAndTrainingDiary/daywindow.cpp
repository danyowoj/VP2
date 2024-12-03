#include "daywindow.h"

DayWindow::DayWindow(int day, QWidget *parent) : QWidget(parent) {
    setWindowTitle(QString("День %1").arg(day));
    resize(300, 200);

    layout = new QVBoxLayout(this);

    // Кнопка "Назад"
    backButton = new QPushButton("Назад", this);
    layout->addWidget(backButton);

    connect(backButton, &QPushButton::clicked, this, &DayWindow::backToCalendar);
}
