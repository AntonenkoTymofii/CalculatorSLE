#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    enternumslar = new EnterNumSLAR;
    solveslar = new SolveSLAR;
    ui->label_min->setVisible(false);
    ui->label_max->setVisible(false);
    ui->min->setVisible(false);
    ui->max->setVisible(false);

    connect(this, &MainWindow::signal_to_enter_slar, enternumslar, &EnterNumSLAR::slot_from_main);
    connect(this, &MainWindow::signal_to_solve, solveslar, &SolveSLAR::slot_from_main);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_RandButton_toggled(bool checked)
{
    ui->label_min->setVisible(true);
    ui->label_max->setVisible(true);
    ui->min->setVisible(true);
    ui->max->setVisible(true);
}

void MainWindow::on_HandButton_toggled(bool checked)
{
    ui->label_min->setVisible(false);
    ui->label_max->setVisible(false);
    ui->min->setVisible(false);
    ui->max->setVisible(false);
}

bool MainWindow::check_max_min(QString minim, QString maxim){
    bool ok;
    if(ui->RandButton->isChecked()){
        if(minim.toDouble() < -10000){
            QMessageBox::critical(this, "Помилка", "Ви ввели замалий мінімум!");
            return false;
        } else if(maxim.toDouble() > 10000){
            QMessageBox::critical(this, "Помилка", "Ви ввели завеликий максимум!");
            return false;
        }else if(!minim.toDouble(&ok) || !maxim.toDouble(&ok)){
            QMessageBox::critical(this, "Помилка", "Ви ввели не число");
            return false;
        }else if(minim.toDouble() > maxim.toDouble()){
            QMessageBox::critical(this, "Помилка", "Ви ввели мінімум який більше за максимум!");
            return false;
        }else if(minim.toDouble() == maxim.toDouble()){
            QMessageBox::critical(this, "Помилка", "Ви ввели однакові мінімум та максимум!");
            return false;
        }
    }
    return true;
}

void MainWindow::on_CreateButton_clicked()
{
    int repit = 1;
    do {
        if(ui->sizeint->text().toInt() > 10 || ui->sizeint->text().toInt() < 1) {
            QMessageBox::critical(this, "Помилка", "Ви ввели не вірну розмірність!");
            return;
        }else {
            repit = 0;
        }
    }while(repit == 1);

    QString s = ui->sizeint->text();
    int size = s.toInt();

    if(ui->RandButton->isChecked() && (ui->sizeint->text() != "\0")) {
        QString minim = ui->min->text();
        QString maxim = ui->max->text();
        double min, max;
        if(check_max_min(minim, maxim)){
            min = minim.toDouble();
            max = maxim.toDouble();
        }else {
            return;
        }
        enternumslar->show();
        emit signal_to_enter_slar(size, min, max, true);
    } else if (ui->HandButton->isChecked() && (ui->sizeint->text() != "\0")){
        enternumslar->show();
        emit signal_to_enter_slar(size, 0, 1, false);
    }else if (!(ui->HandButton->isChecked()) || !(ui->RandButton->isChecked())
               || (ui->sizeint->text() == "\0")){
        QMessageBox::critical(this, "Помилка", "Ви не обрали яким способом створювати СЛАР "
                                                      "або не ввели розмірність!");
        return;
    }
}

void MainWindow::on_dowload_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Виберіть файл", QDir::homePath(), "Усі файли (*.*)");

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QMessageBox::critical(this, "Помилка", "Файл не відкритий для читання");
            return;
        }
        QByteArray fileData = file.readAll();
        QString text = QString::fromUtf8(fileData);
        file.close();
        solveslar->show();
        emit signal_to_solve(text);
    }
}



