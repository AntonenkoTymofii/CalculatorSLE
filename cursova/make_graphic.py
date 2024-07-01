import matplotlib.pyplot as plt
import numpy as np

file_path = "D:/homework/OP/cursova_rob/build-cursova_rob-Desktop_Qt_6_5_1_MSVC2019_64bit-Debug/data.txt"

with open(file_path, 'r') as file:
    lines = file.readlines()

# Виділити коефіцієнти СЛАР та відповіді
coefficients = []
answers = []
for line in lines[:2]:
    row = list(map(float, line.strip().split()))
    coefficients.append(row)

answers = list(map(float, lines[2].strip().split()))
roots = list(map(float, lines[3].strip().split()))

up = 1000
low = -1000
step = 0.1

x1 = roots[0]
x2 = roots[1]

for i in range(2):
    x = np.arange(float(x1) - 10, float(x1) + 10, step)
    if coefficients[i][1] != 0:
        y = (answers[i] - coefficients[i][0] * x) / coefficients[i][1]
    else:
        x = [answers[i] / coefficients[i][0] for _ in range(int((up - low) / step))]
        y = np.arange(float(x2) - 10, float(x2) + 10, step)

    text = f"{round(coefficients[i][0], 3)}*x1 "

    if coefficients[i][1] < 0:
        text += f"{round(coefficients[i][1], 3)}*x2 = "
    else:
        text += f"+ {round(coefficients[i][1], 3)}*x2 = "
    text += str(round(answers[i], 3))

    if i == 0:
        plt.plot(x, y, color="b", label=text)
    else:
        plt.plot(x, y, color="g", label=text)

plt.plot([roots[0]], [roots[1]])
plt.xlabel("x1")
plt.ylabel("x2")
plt.legend(loc="best")
plt.show()
