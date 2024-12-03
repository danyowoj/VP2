#include "calendarwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    CalendarWindow calendarWindow;
    calendarWindow.show();

    return app.exec();
}
