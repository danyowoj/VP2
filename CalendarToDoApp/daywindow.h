#ifndef DAYWINDOW_H
#define DAYWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QListWidget>

class DayWindow : public QWidget {
    Q_OBJECT

public:
    explicit DayWindow(const QString &date, QWidget *parent = nullptr);

signals:
    void backToCalendar();

private:
    QString date;
    QTabWidget *tabWidget;

    QWidget *primaryTab;
    QVBoxLayout *primaryLayout;
    QLineEdit *primaryTaskInput;
    QPushButton *primaryAddTaskButton;
    QListWidget *primaryTaskList;
    QPushButton *primaryRemoveTaskButton;

    QWidget *secondaryTab;
    QVBoxLayout *secondaryLayout;
    QLineEdit *secondaryTaskInput;
    QPushButton *secondaryAddTaskButton;
    QListWidget *secondaryTaskList;
    QPushButton *secondaryRemoveTaskButton;

    QPushButton *backButton;

    void addTaskToPrimary();
    void addTaskToSecondary();
    void removeSelectedTaskFromPrimary();
    void removeSelectedTaskFromSecondary();
};

#endif // DAYWINDOW_H
