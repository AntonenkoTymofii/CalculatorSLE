#include "solveslar.h"
#include "ui_solveslar.h"
#include <QVector>
#include <QString>
#include <QtMath>
#include <QProcess>
#include <algorithm>

using namespace std;

SolveSLAR::SolveSLAR(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SolveSLAR)
{
    ui->setupUi(this);
}

SolveSLAR::~SolveSLAR()
{
    delete ui;
}

QVector<double> LUP(const QVector<QVector<double>>& Matrix, const QVector<double>& Constants)
{
    int n = Matrix.size();

    QVector<QVector<double>> matrix = Matrix;
    QVector<double> constants = Constants;
    // ініціалізація матриці перестановки
    QVector<int> permutation(n);
    for (int i = 0; i < n; i++)
    {
        permutation[i] = i;
    }

    // LUP - декомпозиція
    for (int k = 0; k < n; k++)
    {
        double p = 0;
        int kp = k;

        for (int i = k; i < n; i++)
        {
            if (abs(matrix[i][k]) > p)
            {
                p = abs(matrix[i][k]);
                kp = i;
            }
        }

        if (p == 0)
        {
            return QVector<double>();
        }

        // змінюємо рядки місцями в перестановочній матриці
        int temp = permutation[k];
        permutation[k] = permutation[kp];
        permutation[kp] = temp;

        // змінюємо місцями рядки в матриці коефіцієнтів
        QVector<double> tempRow = matrix[k];
        matrix[k] = matrix[kp];
        matrix[kp] = tempRow;

        for (int i = k + 1; i < n; i++)
        {
            matrix[i][k] /= matrix[k][k];

            for (int j = k + 1; j < n; j++)
            {
                matrix[i][j] -= matrix[i][k] * matrix[k][j];
            }
        }
    }

    // forward elimination
    QVector<double> solution(n, 0.0);
    for (int k = 0; k < n; k++)
    {
        solution[k] = constants[permutation[k]];

        for (int i = 0; i < k; i++)
        {
            solution[k] -= matrix[k][i] * solution[i];
        }
    }

    // backward substitution
    for (int k = n - 1; k >= 0; k--)
    {
        for (int i = k + 1; i < n; i++)
        {
            solution[k] -= matrix[k][i] * solution[i];
        }
        solution[k] /= matrix[k][k];
    }

    return solution;
}

//-------------------------------------------------------------------

QVector<double> rotate_solution(const QVector<QVector<double>>& Matrix, const QVector<double>& Constants) {
    int n = Matrix.size();
    QVector<QVector<double>> matrix = Matrix;
    QVector<double> constants = Constants;
    QVector<double> solution(n, 0);

    for (int k = 0; k < n - 1; k++) {
        for (int i = k + 1; i < n; i++) {
            double theta = atan2(matrix[i][k], matrix[k][k]);
            double cosTheta = cos(theta);
            double sinTheta = sin(theta);

            for (int j = k; j < n; j++) {
                double temp = matrix[k][j];
                matrix[k][j] = cosTheta * temp + sinTheta * matrix[i][j];
                matrix[i][j] = -sinTheta * temp + cosTheta * matrix[i][j];
            }

            double tempConstant = constants[k];
            constants[k] = cosTheta * tempConstant + sinTheta * constants[i];
            constants[i] = -sinTheta * tempConstant + cosTheta * constants[i];
        }
    }

    for (int k = n - 1; k >= 0; k--) {
        double sum = 0;
        for (int j = k + 1; j < n; j++) {
            sum += matrix[k][j] * solution[j];
        }

        if (abs(matrix[k][k]) > 1e-6) {
            solution[k] = (constants[k] - sum) / matrix[k][k];
        } else {
            // Додайте обробку випадку, коли матриця є сингулярною (коефіцієнт при змінній дорівнює 0)
            return QVector<double>();
        }
        if(abs(solution[k]) < 1e-6){
            solution[k] = 0;
        }
    }

    return solution;
}
//-------------------------------------------------------------------------------------------

QVector<double> holetsky_solution(const QVector<QVector<double>>& Matrix, const QVector<double>& Constants) {
    int n = Matrix.size();
    QVector<QVector<double>> matrix = Matrix;
    QVector<double> constants = Constants;
    QVector<QVector<double>> L(n, QVector<double>(n, 0.0));
    QVector<double> y(n, 0.0);
    QVector<double> x(n, 0.0);

    // Декомпозиція Холецького: розкласти матрицю на L*L^T
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= i; j++) {
            double sum = 0.0;
            if (j == i) {
                // Обчислення діагональних елементів L
                for (int k = 0; k < j; k++) {
                    sum += L[j][k] * L[j][k];
                }
                double diagonalElement = matrix[j][j] - sum;
                if (diagonalElement < 0) {
                    // Уникнення від'ємних значень під коренем
                    diagonalElement = 0.0;
                }
                L[j][j] = sqrt(diagonalElement);
            }
            else {
                // Обчислення елементів поза діагоналлю L
                for (int k = 0; k < j; k++) {
                    sum += L[i][k] * L[j][k];
                }
                double denominator = L[j][j];
                if (fabs(denominator) < numeric_limits<double>::epsilon()) {
                    // Уникнення ділення на нуль
                    denominator = numeric_limits<double>::epsilon();
                }
                L[i][j] = (matrix[i][j] - sum) / denominator;
            }
        }
    }

    // Forward substitution: розв'язати L*y = b
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < i; j++) {
            sum += L[i][j] * y[j];
        }
        double denominator = L[i][i];
        if (fabs(denominator) < numeric_limits<double>::epsilon()) {
            // Уникнення ділення на нуль
            denominator = numeric_limits<double>::epsilon();
        }
        y[i] = (constants[i] - sum) / denominator;
    }

    // Backward substitution: розв'язати L^T * x = y
    for (int i = n - 1; i >= 0; i--) {
        double sum = 0.0;
        for (int j = i + 1; j < n; j++) {
            sum += L[j][i] * x[j];
        }
        double denominator = L[i][i];
        if (fabs(denominator) < numeric_limits<double>::epsilon()) {
            // Уникнення ділення на нуль
            denominator = numeric_limits<double>::epsilon();
        }
        x[i] = (y[i] - sum) / denominator;
    }

    return x;
}

//Запис на Вікні
void SolveSLAR::write_text(int i, QVector<QVector<double>> matrix, QVector<double> vector, QVector<double> x)
{
    int size = vector.size();
    text = "Ваша СЛАР виглядає так:\n";
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            text += "(" + QString::number(matrix[i][j]) + ")" + "*x"+ QString::number(j+1) + " + ";
        }
        text.remove(text.length()-2, 2);
        text += "= " + QString::number(vector[i]) + "\n";
    }
    text += "Вирішення вашої СЛАР виглядає так:\n";
    for (int i = 0; i < size; i++) {
        text += "x" + QString::number(i+1) + " = " + QString::number(x[i]) + "\n";
    }
    switch (i) {
    case 1:
        text += "\nLUP-метод побудований на розкладанні\nматриці коефіцієнтів на три матриці:\n"
                "L - нижньотрикутна матриця\n"
                "U - верхньотрикутна матриця\n"
                "P - матриця обертання\n"
                "Асимптотична складність такого алгоритму \nдорівнює n^3, де n - це розмірність СЛАР";
        break;
    case 2:
        text += "\nМетод обертання базується на розкладі матриці коефіцієнтів на Q та R матриці\n"
                "системи та використанні обернених матриць для обчислення розв'язку\n"
                "Асимптотична складність такого алгоритму \nдорівнює n^3, де n - це розмірність СЛАР";
        break;
    case 3:
        text += "\nМетод Гауса-Халецького побудований на розкладі Холецького\n"
                "та використанні корення для вирішення системи\n"
                "Цей метод призначений для вирішення симетричних матриць.\n"
                "Якщо у вас у відповідях знаходяться дуже великі числа, це означає,\n"
                "що система не має вирішень цим методом бо відбувається ділення на 0."
                "Асимптотична складність такого алгоритму \nдорівнює n^3, де n - це розмірність СЛАР";
        break;
    }
}

void writeVectorsToFile(const QVector<QVector<double>>& matrix, const QVector<double>& vector, const QVector<double>& x) {
    QFile file("data.txt");
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        for (const auto& row : matrix) {
            for (const auto& element : row) {
                out << element << " ";
            }
            out << "\n";
        }
        for (const auto& element : vector) {
            out << element << " ";
        }
        out << "\n";
        for (const auto& element : x) {
            out << element << " ";
        }
        file.close();
    }
}

//Вирішення введеної СЛАР
void SolveSLAR::slot_from_enter(int i, QVector<QVector<double>> matrix, QVector<double> vector)
{
    int size = vector.size();
    QVector<double> x(size, 0.0);
    bool problem;
    switch (i) {
    case 1:
        x = LUP(matrix, vector);
        if(x.empty()){
            SolveSLAR::close();
            problem = true;
        }else {
            write_text(1, matrix, vector, x);
            ui->solve->setText(text);
            problem = false;
        }
        break;
    case 2:
        x = rotate_solution(matrix, vector);
        write_text(2, matrix, vector, x);
        ui->solve->setText(text);
        problem = false;
        break;
    case 3:
        x = holetsky_solution(matrix, vector);
        if(x.empty()){
            SolveSLAR::close();
            problem = true;
        }else {
            write_text(3, matrix, vector, x);
            ui->solve->setText(text);
            problem = false;
        }
        break;
    }
    writeVectorsToFile(matrix, vector, x);
    if(size == 2 && !problem){
        QProcess process;
        QString pythonCommand = "python";
        QString pythonScript = "D:/homework/OP/cursova_rob/cursova_rob/make_graphic.py";

        process.start(pythonCommand, QStringList() << pythonScript);
        process.waitForFinished(-1);
    }
}

void SolveSLAR::slot_from_main(QString text1)
{
    ui->solve->setText(text1);
}


void SolveSLAR::on_repeat_try_clicked()
{
    ui->solve->clear();
    SolveSLAR::close();
}


void SolveSLAR::on_save_clicked()
{
    // Виклик файлового провідника для вибору папки
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Save File", "", "Text Files (*.txt)");

    if (!fileName.isEmpty()) {
        // Виклик функції для створення файлу з введеним ім'ям
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            QString text = this->text;
            stream << text;
            file.close();
        }
    }
}

