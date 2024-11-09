#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int shapeID = 0;

private slots:
    void on_rb_square_toggled(bool checked);

    void on_rb_rectangle_toggled(bool checked);

    void on_rb_parallelogram_toggled(bool checked);

    void on_rb_rhombus_toggled(bool checked);

    void on_rb_triangle_toggled(bool checked);

    void on_rb_trapezoid_toggled(bool checked);

    void on_rb_circle_toggled(bool checked);

    void on_rb_sector_toggled(bool checked);

    void on_pushButton_clicked();

    void on_actionAuthors_triggered();

    void on_actionExit_triggered();

    void on_actionClear_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
