#ifndef SOLVESLAR_H
#define SOLVESLAR_H

#include <QWidget>
#include <QVector>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

namespace Ui {
class SolveSLAR;
}

class SolveSLAR : public QWidget
{
    Q_OBJECT

public:
    explicit SolveSLAR(QWidget *parent = nullptr);
    ~SolveSLAR();
    void write_text(int i, QVector<QVector<double>> matrix, QVector<double> vector, QVector<double> x);
    QVector<double> solveLUP(const QVector<QVector<double>>& A, const QVector<double>& b);
    QVector<double> solveUsingCholesky(const QVector<QVector<double>>& A, const QVector<double>& b);
private:
    Ui::SolveSLAR *ui;
    QString text;
public slots:
    void slot_from_enter(int i, QVector<QVector<double>> matrix, QVector<double> vector);
    void slot_from_main(QString text);
private slots:
    void on_repeat_try_clicked();
    void on_save_clicked();
};

#endif // SOLVESLAR_H
