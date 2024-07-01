#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "enternumslar.h"
#include "solveslar.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    bool check_max_min(QString minim, QString maxim);

private slots:
    void on_CreateButton_clicked();

    void on_RandButton_toggled(bool checked);

    void on_HandButton_toggled(bool checked);

    void on_dowload_triggered();

private:
    Ui::MainWindow *ui;
    EnterNumSLAR *enternumslar;
    SolveSLAR *solveslar;
signals:
    void signal_to_enter_slar(int s, double min, double max, bool a);
    void signal_to_solve(QString text);
};
#endif // MAINWINDOW_H

