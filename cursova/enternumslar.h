#ifndef ENTERNUMSLAR_H
#define ENTERNUMSLAR_H

#include <QWidget>
#include <QTableWidget>
#include <QMessageBox>
#include <QVector>
#include "solveslar.h"

namespace Ui {
class EnterNumSLAR;
}

class EnterNumSLAR : public QWidget
{
    Q_OBJECT

public:
    explicit EnterNumSLAR(QWidget *parent = nullptr);
    ~EnterNumSLAR();
    void check_boxes();
    QVector<QVector<double>> set_matrix();
    QVector<double> set_vector();
    double get_determinate(QVector<QVector<double>> &matrix);
    bool is_symmetrical(QVector<QVector<double>> &matrix);

private:
    Ui::EnterNumSLAR *ui;
    int size;
    SolveSLAR *solveslar;

public slots:
    void slot_from_main(int size, double min, double max, bool a);
private slots:
    void on_ReturnBack_clicked();
    void on_ClearAll_clicked();
    void on_SolveEquation_clicked();
signals:
    void signal_to_solve(int i, QVector<QVector<double>> &matrix, QVector<double> &vector);
};

#endif // ENTERNUMSLAR_H
