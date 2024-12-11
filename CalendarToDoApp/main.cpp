#include "calendarwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CalendarWindow window;
    window.show();

    return app.exec();
}