#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "shapecalculator.h"
#include <QMessageBox>
#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowIcon(QIcon(":/pic/img/window_icon.png"));
    setWindowTitle("Shape area calculator");

    ui->label_a->hide();
    ui->lineEdit_a->hide();

    ui->label_b->hide();
    ui->lineEdit_b->hide();

    ui->label_h->hide();
    ui->lineEdit_h->hide();

    ui->label_r->hide();
    ui->lineEdit_r->hide();

    ui->label_alpha->hide();
    ui->lineEdit_alpha->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rb_square_toggled(bool checked)
{
    if (checked) {
        shapeID = 0;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = a^2");
        //show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->result->clear();
        //hide
        ui->label_b->hide();
        ui->lineEdit_b->hide();

        ui->label_h->hide();
        ui->lineEdit_h->hide();

        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_square.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }

}

void MainWindow::on_rb_rectangle_toggled(bool checked)
{
    if (checked) {
        shapeID = 1;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = a * b");
        //show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->label_b->show();
        ui->lineEdit_b->show();
        ui->lineEdit_b->clear();

        ui->result->clear();
        //hide
        ui->label_h->hide();
        ui->lineEdit_h->hide();

        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_rectangle.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_parallelogram_toggled(bool checked)
{
    if (checked) {
        shapeID = 2;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = a * h");
        // show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->label_h->show();
        ui->lineEdit_h->show();
        ui->lineEdit_h->clear();

        ui->result->clear();
        //hide
        ui->label_b->hide();
        ui->lineEdit_b->hide();

        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_parallelogram.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_rhombus_toggled(bool checked)
{
    if (checked) {
        shapeID = 3;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = 0.5 * a * b");
        // show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->label_b->show();
        ui->lineEdit_b->show();
        ui->lineEdit_b->clear();

        ui->result->clear();
        //hide
        ui->label_h->hide();
        ui->lineEdit_h->hide();

        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_rhombus.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_triangle_toggled(bool checked)
{
    if (checked) {
        shapeID = 4;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = 0.5 * a * h");
        // show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->label_h->show();
        ui->lineEdit_h->show();
        ui->lineEdit_h->clear();

        ui->result->clear();
        //hide
        ui->label_b->hide();
        ui->lineEdit_b->hide();

        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_triangle.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_trapezoid_toggled(bool checked)
{
    if (checked) {
        shapeID = 5;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = (a + b) * h * 0.5");
        // show
        ui->label_a->show();
        ui->lineEdit_a->show();
        ui->lineEdit_a->clear();

        ui->label_b->show();
        ui->lineEdit_b->show();
        ui->lineEdit_b->clear();

        ui->label_h->show();
        ui->lineEdit_h->show();
        ui->lineEdit_h->clear();

        ui->result->clear();
        //hide
        ui->label_r->hide();
        ui->lineEdit_r->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_trapezoid.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_circle_toggled(bool checked)
{
    if (checked) {
        shapeID = 6;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = π * r^2");
        //show
        ui->label_r->show();
        ui->lineEdit_r->show();
        ui->lineEdit_r->clear();

        ui->result->clear();
        //hide
        ui->label_b->hide();
        ui->lineEdit_b->hide();

        ui->label_h->hide();
        ui->lineEdit_h->hide();

        ui->label_a->hide();
        ui->lineEdit_a->hide();

        ui->label_alpha->hide();
        ui->lineEdit_alpha->hide();

        QPixmap shape_pic(":/pic/img/shape_circle.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_rb_sector_toggled(bool checked)
{
    if (checked) {
        shapeID = 7;
        ui->statusBar->showMessage(QString::number(shapeID));
        ui->formula->setText("S = π * r^2 * (α / 360)");
        //show
        ui->label_r->show();
        ui->lineEdit_r->show();
        ui->lineEdit_r->clear();

        ui->label_alpha->show();
        ui->lineEdit_alpha->show();
        ui->lineEdit_alpha->clear();

        ui->result->clear();
        //hide
        ui->label_b->hide();
        ui->lineEdit_b->hide();

        ui->label_h->hide();
        ui->lineEdit_h->hide();

        ui->label_a->hide();
        ui->lineEdit_a->hide();

        QPixmap shape_pic(":/pic/img/shape_sector.png");
        int w = ui->picture->width();
        int h = ui->picture->height();
        ui->picture->setPixmap(shape_pic.scaled(w, h, Qt::KeepAspectRatio));
    }
}

void MainWindow::on_pushButton_clicked()
{
    ui->statusBar->showMessage("Calculate");
    // get the shape via shapeID
    switch(shapeID)
    {
    case 0:
        ui->result->setText(QString::number(ShapeCalculator::calculateSquareArea(ui->lineEdit_a->text().toDouble())));
        break;
    case 1:
        ui->result->setText(QString::number(ShapeCalculator::calculateRectangleArea(ui->lineEdit_a->text().toDouble(), ui->lineEdit_b->text().toDouble())));
        break;
    case 2:
        ui->result->setText(QString::number(ShapeCalculator::calculateParallelogramArea(ui->lineEdit_a->text().toDouble(), ui->lineEdit_h->text().toDouble())));
        break;
    case 3:
        ui->result->setText(QString::number(ShapeCalculator::calculateRhombusArea(ui->lineEdit_a->text().toDouble(), ui->lineEdit_b->text().toDouble())));
        break;
    case 4:
        ui->result->setText(QString::number(ShapeCalculator::calculateTriangleArea(ui->lineEdit_a->text().toDouble(), ui->lineEdit_h->text().toDouble())));
        break;
    case 5:
        ui->result->setText(QString::number(ShapeCalculator::calculateTrapezoidArea(ui->lineEdit_a->text().toDouble(), ui->lineEdit_b->text().toDouble(), ui->lineEdit_h->text().toDouble())));
        break;
    case 6:
        ui->result->setText(QString::number(ShapeCalculator::calculateCircleArea(ui->lineEdit_r->text().toDouble())));
        break;
    case 7:
        ui->result->setText(QString::number(ShapeCalculator::calculateSectorArea(ui->lineEdit_r->text().toDouble(), ui->lineEdit_alpha->text().toDouble())));
        break;
    }
}

void MainWindow::on_actionAuthors_triggered()
{
    QMessageBox::information(this, "Authors", "This application was created by Andrey Latsuk from IP-211 group\n\nGitHub: https://github.com/danyowoj");
}

void MainWindow::on_actionExit_triggered()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit confirmation", "Are you sure you want to close the app?",
                          QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
        QApplication::quit();
}

void MainWindow::on_actionClear_triggered()
{
    ui->result->clear();
    ui->formula->clear();
    ui->picture->clear();

    ui->label_a->hide();
    ui->lineEdit_a->hide();
    ui->lineEdit_a->clear();

    ui->label_b->hide();
    ui->lineEdit_b->hide();
    ui->lineEdit_b->clear();

    ui->label_h->hide();
    ui->lineEdit_h->hide();
    ui->lineEdit_h->clear();

    ui->label_r->hide();
    ui->lineEdit_r->hide();
    ui->lineEdit_r->clear();

    ui->label_alpha->hide();
    ui->lineEdit_alpha->hide();
    ui->lineEdit_alpha->clear();
}
