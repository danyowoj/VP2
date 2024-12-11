#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QDialog>
#include <QDate>
#include <QListWidget>
#include <QPushButton>

class DayWindow : public QDialog
{
    Q_OBJECT

public:
    explicit DayWindow(const QDate &date, QWidget *parent = nullptr);

private:
    QDate selectedDate;
    QWidget *createTodoList(const QString &fileSuffix);
    void loadTasksFromFile(QListWidget *listWidget, const QString &fileSuffix);
    void saveTasksToFile(QListWidget *listWidget, const QString &fileSuffix);
};

#endif // DAYWINDOW_H