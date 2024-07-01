#include "enternumslar.h"
#include "ui_enternumslar.h"
#include <iostream>
#include <cmath>
#include <random>


using namespace std;

EnterNumSLAR::EnterNumSLAR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EnterNumSLAR)
{
    ui->setupUi(this);
    solveslar = new SolveSLAR;
    connect(this, &EnterNumSLAR::signal_to_solve, solveslar, &SolveSLAR::slot_from_enter);
}

EnterNumSLAR::~EnterNumSLAR()
{
    delete ui;
}

QVector<QVector<double>> getMatrix(int s, double min, double max) {
    QVector<QVector<double>> matrix(s, QVector<double>(s));

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(min, max);

    // Генеруємо збіжну матрицю, використовуючи діагональне переважання
    for (int i = 0; i < s; i++) {
        double sum = 0.0;
        for (int j = 0; j < s; j++) {
            if (i != j) {
                matrix[i][j] = dis(gen);
                sum += abs(matrix[i][j]);
            }
        }
        matrix[i][i] = sum + 1.0;
    }

    return matrix;
}

QVector<double> getVector(int s, double min, double max) {
    QVector<double> vector(s);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(min, max);

    // Генеруємо вектор відповідей
    for (int i = 0; i < s; i++) {
        vector[i] = dis(gen);
    }

    return vector;
}

double calculateDeterminant(const QVector<QVector<double>>& matrix) {
    int size = matrix.size();

    // Перевірити, чи матриця є квадратною
    if (size == 0 || matrix[0].size() != size) {
        // Повернути 0, як помилкове значення
        return 0.0;
    }

    // Базовий випадок для матриці розміром 1x1
    if (size == 1) {
        return matrix[0][0];
    }

    // Ініціалізувати змінну для збереження визначника
    double determinant = 0.0;

    // Рекурсивно обчислити визначник матриці
    for (int i = 0; i < size; i++) {
        // Отримати підматрицю без першого рядка та i-го стовпця
        QVector<QVector<double>> submatrix;
        for (int j = 1; j < size; j++) {
            QVector<double> row;
            for (int k = 0; k < size; k++) {
                if (k != i) {
                    row.push_back(matrix[j][k]);
                }
            }
            submatrix.push_back(row);
        }

        // Обчислити визначник підматриці та додати його до загального визначника з відповідним знаком
        double subDeterminant = calculateDeterminant(submatrix);
        determinant += (i % 2 == 0) ? matrix[0][i] * subDeterminant : -matrix[0][i] * subDeterminant;
    }

    return determinant;
}

void EnterNumSLAR::slot_from_main(int s, double min, double max, bool a)
{
    size = s;
    if(a){
        ui->MatrixCoef->setRowCount(s);
        ui->MatrixCoef->setColumnCount(s);
        ui->MatrixCoef->horizontalHeader()->setDefaultSectionSize(50);

        ui->VectorCoef->setRowCount(s);
        ui->VectorCoef->setColumnCount(1);
        ui->VectorCoef->horizontalHeader()->setDefaultSectionSize(50);

        QVector<QVector<double>>matrix(size);
        QVector<double>vector(size);
        matrix = getMatrix(s, min, max);
        vector = getVector(s, min, max);
        for (int i = 0; i < s; i++) {
            for (int j = 0; j < s; j++) {
                QTableWidgetItem *tbl = new QTableWidgetItem(QString::number(matrix[i][j]));
                ui->MatrixCoef->setItem(i, j, tbl);
            }
        }
        for (int i = 0; i < s; i++) {
            for (int j = 0; j < 1; j++) {
                QTableWidgetItem *tbl = new QTableWidgetItem(QString::number(vector[i]));
                ui->VectorCoef->setItem(i, j, tbl);
            }
        }
    } else {
        ui->MatrixCoef->setRowCount(s);
        ui->MatrixCoef->setColumnCount(s);
        ui->MatrixCoef->horizontalHeader()->setDefaultSectionSize(50);

        ui->VectorCoef->setRowCount(s);
        ui->VectorCoef->setColumnCount(1);
        ui->VectorCoef->horizontalHeader()->setDefaultSectionSize(50);
    }
}

QVector<QVector<double>> EnterNumSLAR::set_matrix(){
    QVector<QVector<double>> matrix1(size, QVector<double>(size));

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            QTableWidgetItem* item = ui->MatrixCoef->item(i, j);
            matrix1[i][j] = item->text().toDouble();
        }
    }
    return matrix1;
}

QVector<double> EnterNumSLAR::set_vector(){
    QVector<double> vector1(size);

    for (int i = 0; i < size; i++) {
        QTableWidgetItem* item = ui->VectorCoef->item(i, 0);
        vector1[i] = item->text().toDouble();
    }
    return vector1;
}

void EnterNumSLAR::check_boxes(){
    bool ok;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            QTableWidgetItem* item_m = ui->MatrixCoef->item(i, j);
            QString value_m = item_m->text();
            if(value_m.toDouble() > 10000 || value_m.toDouble() < -10000 || !value_m.toDouble(&ok)){
                QMessageBox::critical(this, "Помилка", "У вас є помилка у введені числа в матрицю");
                return;
            }
        }
        QTableWidgetItem* item_v = ui->VectorCoef->item(i, 0);
        QString value_v = item_v->text();
        if(value_v.toDouble() > 10000 || value_v.toDouble() < -10000 || !value_v.toDouble(&ok)){
            QMessageBox::critical(this, "Помилка", "У вас є помилка у введені числа в матрицю");
            return;
        }
    }
}

void EnterNumSLAR::on_ReturnBack_clicked()
{
    ui->MatrixCoef->clear();
    ui->VectorCoef->clear();
    EnterNumSLAR::close();
}

void EnterNumSLAR::on_ClearAll_clicked()
{
    ui->MatrixCoef->clear();
    ui->VectorCoef->clear();
}

double EnterNumSLAR::get_determinate(QVector<QVector<double>> &matrix) {
    if (size != 2) {
        return calculateDeterminant(matrix);
    } else {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
}

bool EnterNumSLAR::is_symmetrical(QVector<QVector<double>> &matrix) {
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < i; j++) {
            if (matrix[i][j] != matrix[j][i]) {
                return false;
            }
        }
    }
    return true;
}

void EnterNumSLAR::on_SolveEquation_clicked()
{
    QVector<QVector<double>>matrix(size);
    QVector<double>vector(size);
    check_boxes();
    matrix = set_matrix();
    vector = set_vector();
    if(get_determinate(matrix) != 0){
        QVector<QVector<double>> matrix1 (size);
        QVector<double> vector1 (size);
        matrix1 = matrix;
        vector1 = vector;
        if(ui->Methods->currentIndex() == 0) {
            emit signal_to_solve(1, matrix1, vector1);
        } else if(ui->Methods->currentIndex() == 1) {
            emit signal_to_solve(2, matrix1, vector1);
        } else if(ui->Methods->currentIndex() == 2){
            if(is_symmetrical(matrix1)){
                emit signal_to_solve(3, matrix1, vector1);
            }else{
                QMessageBox::critical(this, "Помилка", "Ваша матриця коефіцієнтів не симетрична!");
                return;
            }
        }
        solveslar->show();
        EnterNumSLAR::close();
    }else{
        QMessageBox::critical(this, "Помилка", "Визначник вашої матриці дорівнює 0!");
        return;
    }

}

