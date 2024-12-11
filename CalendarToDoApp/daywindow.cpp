#include "daywindow.h"
#include <QVBoxLayout>
#include <QTabWidget>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QInputDialog>
#include <QMessageBox>

DayWindow::DayWindow(const QDate &date, QWidget *parent)
    : QDialog(parent), selectedDate(date)
{
    setWindowTitle("Tasks for " + date.toString());
    resize(400, 300);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QTabWidget *tabWidget = new QTabWidget(this);

    QWidget *todoList = createTodoList("todo");
    tabWidget->addTab(todoList, "ToDo List 1");

    QWidget *todoList2 = createTodoList("todo2");
    tabWidget->addTab(todoList2, "ToDo List 2");

    mainLayout->addWidget(tabWidget);
}

QWidget *DayWindow::createTodoList(const QString &fileSuffix)
{
    QWidget *todoWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(todoWidget);

    QListWidget *listWidget = new QListWidget(todoWidget);
    loadTasksFromFile(listWidget, fileSuffix);

    QHBoxLayout *inputLayout = new QHBoxLayout;
    QLineEdit *taskInput = new QLineEdit(todoWidget);
    QPushButton *addButton = new QPushButton("Add", todoWidget);
    QPushButton *editButton = new QPushButton("Edit", todoWidget);
    QPushButton *deleteButton = new QPushButton("Delete", todoWidget);

    inputLayout->addWidget(taskInput);
    inputLayout->addWidget(addButton);
    inputLayout->addWidget(editButton);
    inputLayout->addWidget(deleteButton);

    layout->addWidget(listWidget);
    layout->addLayout(inputLayout);

    connect(addButton, &QPushButton::clicked, this, [=]()
            {
        if (!taskInput->text().isEmpty()) {
            listWidget->addItem(taskInput->text());
            saveTasksToFile(listWidget, fileSuffix);
            taskInput->clear();
        } });

    connect(editButton, &QPushButton::clicked, this, [=]()
            {
        QListWidgetItem *currentItem = listWidget->currentItem();
        if (currentItem) {
            bool ok;
            QString newText = QInputDialog::getText(this, "Edit Task", "Task:", QLineEdit::Normal, currentItem->text(), &ok);
            if (ok && !newText.isEmpty()) {
                currentItem->setText(newText);
                saveTasksToFile(listWidget, fileSuffix);
            }
        } });

    connect(deleteButton, &QPushButton::clicked, this, [=]()
            {
        QListWidgetItem *currentItem = listWidget->currentItem();
        if (currentItem) {
            delete listWidget->takeItem(listWidget->row(currentItem));
            saveTasksToFile(listWidget, fileSuffix);
        } });

    return todoWidget;
}

void DayWindow::loadTasksFromFile(QListWidget *listWidget, const QString &fileSuffix)
{
    QString fileName = selectedDate.toString("yyyyMMdd") + "_" + fileSuffix + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            listWidget->addItem(line);
        }
        file.close();
    }
}

void DayWindow::saveTasksToFile(QListWidget *listWidget, const QString &fileSuffix)
{
    QString fileName = selectedDate.toString("yyyyMMdd") + "_" + fileSuffix + ".txt";
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        for (int i = 0; i < listWidget->count(); ++i)
        {
            out << listWidget->item(i)->text() << "\n";
        }
        file.close();
    }
}
